#pragma once

#include "terrain-sensor.hpp"

enum class PlayerSensorLetterTag {
    A, B, C, D, E, F
};

enum class PlayerSensorTag {
    GROUND_LEFT,
    GROUND_RIGHT,
    
    CELLING_LEFT,
    CELLING_RIGHT,

    PUSH_LEFT,
    PUSH_RIGHT
};

enum class PlayerSensorMode {
    FLOOR,
    RIGHT_WALL,
    CELLING,
    LEFT_WALL
};

class PlayerSensor {
public:
    PlayerSensor(Vector2f position, Vector2i radius, Vector2i pushRadius, terrain::Terrain& terrain)
        : m_position(position)
        , m_radius(radius)
        , m_radiusPush(pushRadius)
        , m_sensors {
            Sensor { terrain::Sensor(Vector2f(position.x - radius.x, position.y + radius.y), terrain::SensorDirection::DOWN, terrain), true },
            Sensor { terrain::Sensor(Vector2f(position.x + radius.x, position.y + radius.y), terrain::SensorDirection::DOWN, terrain), true },

            Sensor { terrain::Sensor(Vector2f(position.x - radius.x, position.y - radius.y), terrain::SensorDirection::UP, terrain), true },
            Sensor { terrain::Sensor(Vector2f(position.x + radius.x, position.y - radius.y), terrain::SensorDirection::UP, terrain), true },

            Sensor { terrain::Sensor(Vector2f(position.x - pushRadius.x, position.y + pushRadius.y), terrain::SensorDirection::LEFT,  terrain), true },
            Sensor { terrain::Sensor(Vector2f(position.x + pushRadius.x, position.y + pushRadius.y), terrain::SensorDirection::RIGHT, terrain), true },
        }
    {}

    void setMode(PlayerSensorMode mode) {
        float angle = (float)mode * 90.0f;

        float angle_sin = sin(radians(angle));
        float angle_cos = cos(radians(angle));

        for (int i = 0; i < 6; i++) {
            float sign_x = (i % 2 != 0) ? 1 : -1;
            float sign_y = (i / 2 == 0) ? 1 : -1;

            Vector2i radius = (i < 4) ? m_radius : m_radiusPush;

            m_sensors[i].tsensor.setPosition(Vector2f(
                m_position.x + (radius.x * sign_x *  angle_cos) + (radius.y * sign_y * angle_sin),
                m_position.y + (radius.x * sign_x * -angle_sin) + (radius.y * sign_y * angle_cos)
            ));

            terrain::SensorDirection sensorDir;

            switch ((PlayerSensorTag)i) {
                case PlayerSensorTag::GROUND_LEFT:
                case PlayerSensorTag::GROUND_RIGHT:
                    sensorDir = terrain::SensorDirection::DOWN;
                    break;

                case PlayerSensorTag::CELLING_LEFT:
                case PlayerSensorTag::CELLING_RIGHT:
                    sensorDir = terrain::SensorDirection::UP;
                    break;

                case PlayerSensorTag::PUSH_LEFT:
                    sensorDir = terrain::SensorDirection::LEFT;
                    break;

                case PlayerSensorTag::PUSH_RIGHT:
                    sensorDir = terrain::SensorDirection::RIGHT;
                    break;
            }

            auto dir = (terrain::SensorDirection)(((int)mode + (int)sensorDir) % 4);

            m_sensors[i].tsensor.setDirection(dir);
        }

        m_mode = mode;
    }

    terrain::SensorResult scanWallLeft() {
        auto& sen = sensor(PlayerSensorTag::PUSH_LEFT);
        auto  res = sen.tsensor.scan();

        if (!sen.isEnabled || res.solidity == terrain::BlockSolidity::ONLY_TOP || res.solidity == terrain::BlockSolidity::EMPTY)
            return terrain::SensorResult::Empty();
        
        return res;
    }

    terrain::SensorResult scanWallRight() {
        auto& sen = sensor(PlayerSensorTag::PUSH_RIGHT);
        auto  res = sen.tsensor.scan();

        if (!sen.isEnabled || res.solidity == terrain::BlockSolidity::ONLY_TOP || res.solidity == terrain::BlockSolidity::EMPTY)
            return terrain::SensorResult::Empty();
        
        return res;
    }

    terrain::SensorResult scanGround() {
        return scanBoth(PlayerSensorTag::GROUND_LEFT, PlayerSensorTag::GROUND_RIGHT);
    }

    terrain::SensorResult scanCelling() {
        return scanBoth(PlayerSensorTag::CELLING_LEFT, PlayerSensorTag::CELLING_RIGHT);
    }

    void setPosition(Vector2f position) {
        for (auto& s : m_sensors) {
            s.tsensor.move(Vector2f(position.x - m_position.x, position.y - m_position.y));
        }

        m_position = position;
    }

    void move(Vector2f delta) {
        setPosition(Vector2f(m_position.x + delta.x, m_position.y + delta.y));
    }

    void setSensorState(PlayerSensorLetterTag tag, bool state)  { sensor(tag).isEnabled = state; } 
    void setSensorState(PlayerSensorTag tag, bool state)        { sensor(tag).isEnabled = state; } 

    void draw(Camera& cam) {
        sf::Color colors[] = {
            sf::Color(0x00FF00FF),
            sf::Color(0x00FFFFFF),
            sf::Color(0x0000FFFF),
            sf::Color(0xFFFF00FF),
            sf::Color(0xFF00FFFF),
            sf::Color(0xFF0000FF),
        };

        for (int i = 0; i < 6; i++) {
            m_sensors[i].tsensor.draw(cam, colors[i]);
        }
    }

private:
    struct Sensor {
        terrain::Sensor tsensor;
        bool            isEnabled;
    };

private:
    Vector2f         m_position;
    PlayerSensorMode m_mode      = PlayerSensorMode::FLOOR;

    Vector2i m_radius;
    Vector2i m_radiusPush;

    Sensor   m_sensors[6];
private:
    Sensor& sensor(PlayerSensorLetterTag tag) { return m_sensors[(int)tag]; }
    Sensor& sensor(PlayerSensorTag tag)       { return m_sensors[(int)tag]; }

    terrain::SensorResult scanBoth(PlayerSensorTag tagLeft, PlayerSensorTag tagRight) {
        auto& sleft  = sensor(tagLeft);
        auto& sright = sensor(tagRight);

        if (!sleft.isEnabled && !sright.isEnabled) {
            return terrain::SensorResult::Empty();
        }

        terrain::SensorResult sleftRes  = sleft.tsensor.scan();
        terrain::SensorResult srightRes = sright.tsensor.scan();

        terrain::SensorResult res;

        bool aEmpty = (!sleft.isEnabled 
            || (tagLeft == PlayerSensorTag::GROUND_LEFT  && sleftRes.solidity == terrain::BlockSolidity::ONLY_LEFT_BOTTOM_RIGHT)
            || (tagLeft == PlayerSensorTag::CELLING_LEFT && sleftRes.solidity == terrain::BlockSolidity::ONLY_TOP)
            || sleftRes.solidity == terrain::BlockSolidity::EMPTY);

        bool bEmpty = (!sright.isEnabled 
            || (tagRight == PlayerSensorTag::GROUND_RIGHT  && srightRes.solidity == terrain::BlockSolidity::ONLY_LEFT_BOTTOM_RIGHT)
            || (tagRight == PlayerSensorTag::CELLING_RIGHT && srightRes.solidity == terrain::BlockSolidity::ONLY_TOP)
            || srightRes.solidity == terrain::BlockSolidity::EMPTY);

        res = (aEmpty)                                  ? srightRes : 
              (bEmpty)                                  ? sleftRes  :
              (sleftRes.distance <= srightRes.distance) ? sleftRes  : srightRes;
        
        if (res.isFlagged()) {
            res.angle = HexAngle::fromDegrees((float)m_mode * 90.f); 
        }

        return res;
    }
};