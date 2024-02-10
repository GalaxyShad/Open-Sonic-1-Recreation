#ifndef PLAYER_COLLIDER_HPP
#define PLAYER_COLLIDER_HPP

#include "core/GameMath.h"
#include "new-terrain.hpp"
#include "player-sensor.hpp"
#include "terrain-sensor.hpp"
#include <functional>

class PlayerCollider {
public:
    PlayerCollider(v2f& playerPosition, v2f& playerSpeed, float& playerGsp, terrain::Terrain& terrain) 
        : m_playerPosition(playerPosition)
        , m_playerSpeed(playerSpeed)
        , m_playerGroundSpeed(playerGsp)
        , m_sensor(playerPosition, v2i(9, 20), v2i(10, 0), terrain)
    {}

    void update() {
        if (m_grounded) groundedSensorActivation();
        else            airboneSensorActivation();

        m_sensor.setPosition(m_playerPosition);

        collideWalls();

        m_sensor.setPosition(m_playerPosition);

        if (m_grounded) collideGround();
        else            collideGroundAirbone();

        if (!m_grounded) collideCelling();

        determineMode();           
    }

    void setLayer(terrain::TerrainLayer layer) {
        m_sensor.setLayer(layer);
    }

    void draw(Camera& cam) {
        m_sensor.draw(cam);
    }

    bool isPlayerCanJump() {
        bool stateLeft = m_sensor.getSensorState(PlayerSensorTag::CELLING_LEFT);
        bool stateRight = m_sensor.getSensorState(PlayerSensorTag::CELLING_RIGHT);

        m_sensor.setSensorState(PlayerSensorTag::CELLING_LEFT, true);
        m_sensor.setSensorState(PlayerSensorTag::CELLING_RIGHT, true);

        auto scanRes = m_sensor.scanCelling();

        m_sensor.setSensorState(PlayerSensorTag::CELLING_LEFT, stateLeft);
        m_sensor.setSensorState(PlayerSensorTag::CELLING_RIGHT, stateRight);
        
        return (scanRes.distance < 6) == false;
    }

    HexAngle getAngle() { return m_angle; }
    
    PlayerSensorMode getMode() { return m_sensor.getMode(); }

    void forceToAir() { m_grounded = false; }
    bool isGrounded() const { return m_grounded; }

    void onLanding(std::function<void(HexAngle)> function) {
        m_callback_landing = function;
    }

private: 
    v2f&  m_playerPosition;
    v2f&  m_playerSpeed;
    float& m_playerGroundSpeed;

    HexAngle m_angle = {0};
    bool m_grounded = false;

    PlayerSensor m_sensor;

    std::function<void(HexAngle)> m_callback_landing;

private:
    void determineMode() {
        if (!m_grounded) {
            m_sensor.setMode(PlayerSensorMode::FLOOR);
            m_sensor.setModePush(PlayerSensorMode::FLOOR);

            return;
        }

        m_sensor.setMode(
            m_angle.inRange(161, 223) ? PlayerSensorMode::RIGHT_WALL :
            m_angle.inRange( 96, 160) ? PlayerSensorMode::CELLING    :
            m_angle.inRange( 33,  95) ? PlayerSensorMode::LEFT_WALL  :
                                             PlayerSensorMode::FLOOR
        );

        m_sensor.setModePush(
            m_angle.inRange(160, 224) ? PlayerSensorMode::RIGHT_WALL :
            m_angle.inRange( 97, 159)      ? PlayerSensorMode::CELLING    :
            m_angle.inRange( 32,  96)      ? PlayerSensorMode::LEFT_WALL  :
                                                  PlayerSensorMode::FLOOR
        );
    }

    void groundedSensorActivation() {
        if (!m_grounded) return;

        m_sensor.setSensorState(PlayerSensorTag::GROUND_LEFT, true);
        m_sensor.setSensorState(PlayerSensorTag::GROUND_RIGHT, true);

        bool inActivePushAngleRange = (m_angle.degrees() > 90 && m_angle.degrees() < 270) == false;

        m_sensor.setSensorState(PlayerSensorTag::PUSH_LEFT,  (m_playerGroundSpeed < 0) && inActivePushAngleRange);
        m_sensor.setSensorState(PlayerSensorTag::PUSH_RIGHT, (m_playerGroundSpeed > 0) && inActivePushAngleRange);

        // TODO In S3K Push Sensors will also appear when the Player's Ground Angle is a multiple of 90° (64) in addition to the normal angle ranges.

        m_sensor.setSensorState(PlayerSensorTag::CELLING_LEFT, false);
        m_sensor.setSensorState(PlayerSensorTag::CELLING_RIGHT, false);
    }

    void airboneSensorActivation() {
        if (m_grounded) return;

        // TODO ? check the angle of your motion (X Speed and Y Speed) through the air.

        // shortcuts
        float& xsp = m_playerSpeed.x;
        float& ysp = m_playerSpeed.y;

        bool movingMostlyVertical = (abs(ysp) > abs(xsp));

        bool mostlyRight = !movingMostlyVertical && xsp > 0;
        bool mostlyLeft  = !movingMostlyVertical && xsp < 0;
        bool mostlyUp    =  movingMostlyVertical && ysp < 0;
        bool mostlyDown  =  movingMostlyVertical && ysp > 0;

        if (!movingMostlyVertical) {
            m_sensor.setSensorState(PlayerSensorTag::GROUND_LEFT, true);
            m_sensor.setSensorState(PlayerSensorTag::GROUND_RIGHT, true);

            m_sensor.setSensorState(PlayerSensorTag::CELLING_LEFT, true);
            m_sensor.setSensorState(PlayerSensorTag::CELLING_RIGHT, true);

            m_sensor.setSensorState(PlayerSensorTag::PUSH_LEFT, mostlyLeft);
            m_sensor.setSensorState(PlayerSensorTag::PUSH_RIGHT, mostlyRight);
        } else {
            m_sensor.setSensorState(PlayerSensorTag::PUSH_LEFT, true);
            m_sensor.setSensorState(PlayerSensorTag::PUSH_RIGHT, true);

            m_sensor.setSensorState(PlayerSensorTag::GROUND_LEFT, mostlyDown);
            m_sensor.setSensorState(PlayerSensorTag::GROUND_RIGHT, mostlyDown);

            m_sensor.setSensorState(PlayerSensorTag::CELLING_LEFT, mostlyUp);
            m_sensor.setSensorState(PlayerSensorTag::CELLING_RIGHT, mostlyUp);
        }
    }

    void collideWalls() {
        m_sensor.setRadiusPush(v2i(10, (m_angle.hex == 0 && m_grounded) ? 8 : 0));

        collideWallRight();
        collideWallLeft();
    }

    void collideWallLeft() {
        auto scanRes = m_sensor.scanWallLeft();

        if (scanRes.distance > 0)
            return;

        if (m_grounded && m_playerGroundSpeed < 0)
            m_playerGroundSpeed = 0.0;

        if (!m_grounded && m_playerSpeed.x < 0)
            m_playerSpeed.x = 0.0;

        if (m_sensor.getMode() == PlayerSensorMode::CELLING || m_sensor.getMode() == PlayerSensorMode::FLOOR)
            m_playerPosition.x -= scanRes.distance;
        else 
            m_playerPosition.y -= scanRes.distance;
        //m_playerSpeed.x += (m_playerSpeed.x > 0) ? scanRes.distance : -scanRes.distance;
    }

    void collideWallRight() {
        auto scanRes = m_sensor.scanWallRight();

        if (scanRes.distance > 0)
            return;

        if (m_grounded && m_playerGroundSpeed > 0)
            m_playerGroundSpeed = 0.0;

        if (!m_grounded && m_playerSpeed.x > 0)
            m_playerSpeed.x = 0.0;

        if (m_sensor.getMode() == PlayerSensorMode::CELLING || m_sensor.getMode() == PlayerSensorMode::FLOOR)
            m_playerPosition.x += scanRes.distance;
        else 
            m_playerPosition.y += scanRes.distance;
        //m_playerSpeed.x += (m_playerSpeed.x > 0) ? scanRes.distance : -scanRes.distance;
    }

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
            landing();
        }
    }

    void collideCelling() {
        auto scanRes = m_sensor.scanCelling();

        if (scanRes.distance >= 0)
            return;

        // shortcuts
        float& xsp = m_playerSpeed.x;
        float& ysp = m_playerSpeed.y;

        bool movingMostlyVertical = (abs(ysp) > abs(xsp));
        bool movingMostlyUp       = movingMostlyVertical && ysp < 0;

        if ((movingMostlyUp && scanRes.distance <= -(ysp + 8)) ||
            (!movingMostlyVertical && ysp <= 0)
        ) {
            m_playerPosition.y -= scanRes.distance;
            
            landingCelling(scanRes.angle);
        }
    }

    bool shouldCollide(terrain::SensorResult sensorResult) {
        // Sonic 1
        if ((sensorResult.distance < -14) || (sensorResult.distance > 14))
            return false;

        return sensorResult.solidity != terrain::BlockSolidity::EMPTY;
        // TODO Sonic 2
    }

    void landingCelling(HexAngle foundAngle) {
        auto& spd = m_playerSpeed;

        bool flat = (foundAngle.degrees() >= 136 && foundAngle.degrees() <= 225);
        bool movingMostlyUp = (abs(spd.y) > abs(spd.x)) && spd.y < 0;

        if (flat || !movingMostlyUp) {
            spd.y = 0;

            return;
        } 

        m_angle = foundAngle;
        m_playerGroundSpeed = spd.y * gmath::sign(HexAngle::sin(m_angle));
        m_grounded = true;

        m_callback_landing(m_angle);
    }

    void landing() {
        auto& spd = m_playerSpeed;
        auto& gsp = m_playerGroundSpeed;

        bool flat  = m_angle.inRange(240, 255) || m_angle.inRange(0, 15);
        bool slope = m_angle.inRange(224, 255) || m_angle.inRange(0, 31);

        if (flat) {
            gsp = spd.x;
        } else {
            bool movingHorizontal = abs(spd.x) > abs(spd.y);

            gsp = movingHorizontal 
                ? spd.x
                : spd.y * gmath::sign(HexAngle::sin(m_angle)) * (slope ? 0.5 : 1);
        } 

        m_callback_landing(m_angle);
    }

};

#endif // PLAYER_COLLIDER_HPP
