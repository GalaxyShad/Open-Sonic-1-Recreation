#ifndef PLAYER_COLLIDER_HPP
#define PLAYER_COLLIDER_HPP

#include "new-terrain.hpp"
#include "player-sensor.hpp"
#include "terrain-sensor.hpp"
#include <functional>

class PlayerCollider {
public:
    PlayerCollider(v2f& playerPosition, v2f& playerSpeed, terrain::Terrain& terrain) 
       : m_playerPosition(playerPosition)
       , m_playerSpeed(playerSpeed)
       , m_sensor(playerPosition, v2i(9, 20), v2i(10, 0), terrain)
    {}

    void update() {
        m_sensor.setPosition(m_playerPosition);

        if (m_grounded) collideGround();
        else            collideGroundAirbone();

        // if ((m_angle.hex >= 224 && m_angle.hex <= 255) || (m_angle.hex >= 0 && m_angle.hex <= 32))
            
        if (m_angle.hex >= 161 && m_angle.hex <= 223) 
            m_sensor.setMode(PlayerSensorMode::RIGHT_WALL);
        else if (m_angle.hex >= 96 && m_angle.hex <= 160)
            m_sensor.setMode(PlayerSensorMode::CELLING);
        else if (m_angle.hex >= 33 && m_angle.hex <= 95)
            m_sensor.setMode(PlayerSensorMode::LEFT_WALL);
        else 
            m_sensor.setMode(PlayerSensorMode::FLOOR);

    }

    void draw(Camera& cam) {
        m_sensor.draw(cam);
    }

    HexAngle getAngle() { return m_angle; }

    void forceToAir() { m_grounded = false; }
    bool isGrounded() const { return m_grounded; }

    void onLanding(std::function<void(HexAngle)> function) {
        m_callback_landing = function;
    }

private: 
    v2f& m_playerPosition;
    v2f& m_playerSpeed;

    HexAngle m_angle = {0};
    bool m_grounded = false;

    PlayerSensor m_sensor;

    std::function<void(HexAngle)> m_callback_landing;

private:
    void collideGround() {
        auto scanRes = m_sensor.scanGround();

        if (!shouldCollide(scanRes)) {
            m_grounded = false;
            m_angle.hex = 0;
            return;
        }

        switch (m_sensor.getMode()) {
            case PlayerSensorMode::FLOOR:      m_playerPosition.y += scanRes.distance; break; 
            case PlayerSensorMode::RIGHT_WALL: m_playerPosition.x += scanRes.distance; break;
            case PlayerSensorMode::CELLING:    m_playerPosition.y -= scanRes.distance; break;
            case PlayerSensorMode::LEFT_WALL:  m_playerPosition.x -= scanRes.distance; break;
        }        

        m_angle = scanRes.angle;

        m_grounded = true;
    }

    void collideGroundAirbone() {
        auto scanRes = m_sensor.scanGround();

        if (scanRes.distance >= 0)
            return;

        // shortcuts
        float& xsp = m_playerSpeed.x;
        float& ysp = m_playerSpeed.y;

        bool movingMostlyVertical = (abs(ysp) > abs(xsp));
        bool movingMostlyDown     = movingMostlyVertical && ysp > 0;

        if ((movingMostlyDown && scanRes.distance >= -(ysp + 8)) ||
            (!movingMostlyVertical && ysp >= 0)
        ) {
            collideGround();
            m_callback_landing(m_angle);
        }
    }

    bool shouldCollide(terrain::SensorResult sensorResult) {
        // Sonic 1
        if ((sensorResult.distance < -14) || (sensorResult.distance > 14))
            return false;

        return true; //sensorResult.solidity != terrain::BlockSolidity::EMPTY;
        // TODO Sonic 2
    }

};

#endif // PLAYER_COLLIDER_HPP
