#pragma once 

#include "core/Geometry.h"
#include "core/game_enviroment/InputMgr.h"

class PlayerMover {
public:
    PlayerMover(v2f& pos, v2f& spd, float& gsp, IInputMgr& input) 
        : m_pos(pos) 
        , m_spd(spd)
        , m_gsp(gsp)
        , m_input(input)
    {}

private: 
    v2f&    m_pos;
    v2f&    m_spd;
    float&  m_gsp;

    IInputMgr& m_input;

    int     m_horizontalLockTimer = 0;
    bool    m_horizontalMovementAllowed = true;

    float   m_friction;
    float   m_acceleration;
    float   m_accelerationAir;
    float   m_decceleration;
    float   m_topSpeed;

public:
    
    float getFriction() const { return m_friction; }
    void setFriction(float friction) { m_friction = friction; }

    float getAcceleration() const { return m_acceleration; }
    void setAcceleration(float acceleration) { m_acceleration = acceleration; }

    float getAccelerationAir() const { return m_accelerationAir; }
    void setAccelerationAir(float accelerationAir) { m_accelerationAir = accelerationAir; }

    float getDecceleration() const { return m_decceleration; }
    void setDecceleration(float decceleration) { m_decceleration = decceleration; }

    float getTopSpeed() const { return m_topSpeed; }
    void setTopSpeed(float topSpeed) { m_topSpeed = topSpeed; }
};