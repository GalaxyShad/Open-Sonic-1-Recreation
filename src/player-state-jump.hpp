#pragma once

#include "AudioMappings.h"
#include "player-sensor.hpp"
#include "player-state-base.hpp"
#include "player-state-machine.hpp"

class PlayerStateJump : public PlayerStateBase {
public:
    PlayerStateJump(PlayerStateProps &player, float jumpHeight,
                    dj::Sound &sndJump_)
        : m_player(player), m_jumpHeight(jumpHeight), sndJump_(sndJump_) {}

    void onInit() override {
        m_player.colliderTerrain.forceToAir();

        auto angle = m_player.colliderTerrain.getAngle();

        m_player.spd.x -= m_jumpHeight * -HexAngle::sin(angle);
        m_player.spd.y -= m_jumpHeight * HexAngle::cos(angle);

        m_player.audio.dj().playSound(sndJump_);
    }

    void onUpdate() override {
        if ((m_player.spd.y < -4.0) && (!m_player.input.isKeyAction()))
            m_player.spd.y = -4.0;

        m_player.anim.set(15, 22, 1.0 / int(fmax(1, 4.0 - abs(m_player.gsp))));
    }

    void onLanding() override {
        m_player.stateMachine.changeTo(PlayerStateID::NORMAL);
    }

private:
    dj::Sound &sndJump_;
    PlayerStateProps &m_player;
    float m_jumpHeight;
};

class PlayerStateDie : public PlayerStateBase {
public:
    PlayerStateDie(PlayerStateProps &player, dj::Sound &sndHurt)
        : m_player(player), sndHurt_(sndHurt) {}

    void onInit() override {
        m_player.spd.x = 0;
        m_player.spd.y = -7;

        m_player.colliderTerrain.forceToAir();
        m_player.audio.dj().playSound(sndHurt_);

        m_player.anim.set(37, 37, 0);
    }

    void onUpdate() override {
        if (m_player.spd.y < 16.0)
            m_player.spd.y += 0.21875; // TODO move gravity

        m_player.pos.y += m_player.spd.y;
    }

private:
    dj::Sound &sndHurt_;
    PlayerStateProps &m_player;
};

class PlayerStateSkid : public PlayerStateBase {
public:
    PlayerStateSkid(PlayerStateProps &player, dj::Sound &sndSkid) : m_player(player), sndSkid_(sndSkid) {}

    void onInit() override {
        m_player.audio.dj().playSound(sndSkid_);
    }

    void onUpdate() override {
        if (m_player.gsp > 0 && !m_player.input.isKeyLeft())
            m_player.stateMachine.changeTo(PlayerStateID::NORMAL);

        else if (m_player.gsp < 0 && !m_player.input.isKeyRight())
            m_player.stateMachine.changeTo(PlayerStateID::NORMAL);

        else if (m_player.gsp == 0 ||
                 m_player.colliderTerrain.getMode() != PlayerSensorMode::FLOOR)
            m_player.stateMachine.changeTo(PlayerStateID::NORMAL);

        if (int(m_player.anim.getCurFrame()) == 25) {
            m_player.anim.set(25, 25, 0);
        } else {
            m_player.anim.set(23, 25, 0.125);
        }
    }

private:
    PlayerStateProps &m_player;
    dj::Sound &sndSkid_;
};

class PlayerStateSpindashCD : public PlayerStateBase {
public:
    PlayerStateSpindashCD(PlayerStateProps &player, dj::Sound& sndRoll) : m_player(player), sndRoll_(sndRoll) {}

    void onInit() override {
        m_timer = 45;

        m_player.pos.y += 5;

        m_player.audio.dj().playSound(sndRoll_);
    }

    void onUpdate() override {
        if (m_timer != 0) {
            m_player.stateMachine.changeTo(PlayerStateID::NORMAL);
            return;
        }

        bool isDirRight = true;

        if (isDirRight) {
            m_player.gsp = 12.f;
        } else {
            m_player.gsp = -12.f;
        }
        // TODO Camera Lag
        // camLagTimer = 16

        if (m_timer > 0)
            m_timer--;

        m_player.anim.set(15, 22, (float(45 - m_timer) / 30));
    }

private:
    PlayerStateProps &m_player;
    dj::Sound &sndRoll_;

    int m_timer;
};

class PlayerStateRoll : public PlayerStateBase {
public:
    PlayerStateRoll(PlayerStateProps &player, dj::Sound &sndRoll) : m_player(player), sndRoll_(sndRoll) {}

    void onInit() override { m_player.audio.dj().playSound(sndRoll_); }

    void onUpdate() override {
        m_player.anim.set(15, 22, 1.0 / int(fmax(1, 4.0 - abs(m_player.gsp))));

        if (m_player.gsp == 0)
            m_player.stateMachine.changeTo(PlayerStateID::NORMAL);
    }

    void onLanding() override {
        m_player.stateMachine.changeTo(PlayerStateID::NORMAL);
    }

private:
    PlayerStateProps &m_player;
    dj::Sound &sndRoll_;
};

class PlayerStateHurt : public PlayerStateBase {
public:
    PlayerStateHurt(PlayerStateProps &player, dj::Sound &sndHurt) : m_player(player), sndHurt_(sndHurt) {}

    void onInit() override {
        m_player.audio.dj().playSound(sndHurt_);

        m_player.spd.y = -4;
        m_player.spd.x = 2 * -gmath::sign(m_player.spd.x);
        if (m_player.spd.x == 0)
            m_player.spd.x = -2;

        m_player.anim.set(36, 36, 0);
    }

    void onLanding() override {
        m_player.gsp = 0;
        m_player.spd.x = 0;
        m_player.stateMachine.changeTo(PlayerStateID::NORMAL);
    }

private:
    PlayerStateProps &m_player;
    dj::Sound &sndHurt_;
};

class PlayerStateSpring : public PlayerStateBase {
public:
    PlayerStateSpring(PlayerStateProps &player) : m_player(player) {}

    void onLanding() override {
        m_player.stateMachine.changeTo(PlayerStateID::NORMAL);
    }

    void onUpdate() override {
        if (m_player.spd.y < 0.0)
            m_player.anim.set(51, 53, (0.125 + fabs(m_player.spd.y) / 25),
                              true);
        else
            m_player.anim.set(54, 55, 0.25, false);
    }

private:
    PlayerStateProps &m_player;
};