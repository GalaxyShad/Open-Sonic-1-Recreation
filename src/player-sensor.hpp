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
    PlayerSensor(v2f position, v2i radius, v2i pushRadius, terrain::Terrain& terrain)
        : m_position(position)
        , m_radius(radius)
        , m_radiusPush(pushRadius)
        , m_sensors {
            Sensor { terrain::Sensor(v2f(position.x - radius.x, position.y + radius.y), terrain::SensorDirection::DOWN, terrain), true },
            Sensor { terrain::Sensor(v2f(position.x + radius.x, position.y + radius.y), terrain::SensorDirection::DOWN, terrain), true },

            Sensor { terrain::Sensor(v2f(position.x - radius.x, position.y - radius.y), terrain::SensorDirection::UP, terrain), true },
            Sensor { terrain::Sensor(v2f(position.x + radius.x, position.y - radius.y), terrain::SensorDirection::UP, terrain), true },

            Sensor { terrain::Sensor(v2f(position.x - pushRadius.x, position.y + pushRadius.y), terrain::SensorDirection::LEFT,  terrain), true },
            Sensor { terrain::Sensor(v2f(position.x + pushRadius.x, position.y + pushRadius.y), terrain::SensorDirection::RIGHT, terrain), true },
        }
    {}


    PlayerSensorMode getMode() const { return m_mode; }
    PlayerSensorMode getModePush() const { return m_modePush; }

    void setModePush(PlayerSensorMode mode) {
         float angle = (float)mode * 90.0f;

        float angle_sin = sinf(radians(angle));
        float angle_cos = cosf(radians(angle));

        for (int i = 4; i < 6; i++) {
            float sign_x = (i % 2 != 0) ? 1 : -1;

            m_sensors[i].tsensor.setPosition(v2f(
                m_position.x + (m_radiusPush.x * sign_x *  angle_cos) + (m_radiusPush.y * angle_sin),
                m_position.y + (m_radiusPush.x * sign_x * -angle_sin) + (m_radiusPush.y * angle_cos)
            ));

            terrain::SensorDirection sensorDir;

            PlayerSensorTag sensorTag = ((PlayerSensorTag)i);

            sensorDir = (sensorTag == PlayerSensorTag::PUSH_LEFT) 
                ? terrain::SensorDirection::LEFT
                : terrain::SensorDirection::RIGHT;

            auto dir = (terrain::SensorDirection)(((int)mode + (int)sensorDir) % 4);

            m_sensors[i].tsensor.setDirection(dir);
        }

        m_modePush = mode;
    }

    void setMode(PlayerSensorMode mode) {
        float angle = (float)mode * 90.0f;

        float angle_sin = sinf(radians(angle));
        float angle_cos = cosf(radians(angle));

        for (int i = 0; i < 4; i++) {
            float sign_x = (i % 2 != 0) ? 1 : -1;
            float sign_y = (i / 2 == 0) ? 1 : -1;

            v2i radius = (i < 4) ? m_radius : m_radiusPush;

            m_sensors[i].tsensor.setPosition(v2f(
                m_position.x + (radius.x * sign_x *  angle_cos) + (radius.y * sign_y * angle_sin),
                m_position.y + (radius.x * sign_x * -angle_sin) + (radius.y * sign_y * angle_cos)
            ));

            terrain::SensorDirection sensorDir;
            
            auto sensorTag = (PlayerSensorTag)i;

            if ((sensorTag == PlayerSensorTag::GROUND_LEFT) || (sensorTag == PlayerSensorTag::GROUND_RIGHT))
                sensorDir = terrain::SensorDirection::DOWN;
            else 
                sensorDir = terrain::SensorDirection::UP;

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

    void setPosition(v2f position) {
        for (auto& s : m_sensors) {
            s.tsensor.move(v2f(position.x - m_position.x, position.y - m_position.y));
        }

        m_position = position;
    }

    void move(v2f delta) {
        setPosition(v2f(m_position.x + delta.x, m_position.y + delta.y));
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
            if (m_sensors[i].isEnabled)
                m_sensors[i].tsensor.draw(cam, colors[i]);
        }
    }

private:
    struct Sensor {
        terrain::Sensor tsensor;
        bool            isEnabled;
    };

private:
    v2f              m_position;
    PlayerSensorMode m_mode      = PlayerSensorMode::FLOOR;
    PlayerSensorMode m_modePush  = PlayerSensorMode::FLOOR;
    v2i              m_radius;
    v2i              m_radiusPush;
    Sensor           m_sensors[6];
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
            if (tagLeft == PlayerSensorTag::CELLING_LEFT || tagLeft == PlayerSensorTag::CELLING_RIGHT)
                res.angle = HexAngle::fromDegrees((float)(((int)m_mode + 2) % 4) * 90.f); 
            else 
                res.angle = HexAngle::fromDegrees((float)m_mode * 90.f); 
        }

        return res;
    }
};