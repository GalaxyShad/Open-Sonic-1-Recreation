#pragma once

#include "AudioMappings.h"
#include "core/game_enviroment/artist/Animator.h"
#include "core/game_enviroment/artist/ArtistStructs.h"
#include "player-sensor.hpp"
#include "player-state-base.hpp"
#include "player-state-machine.hpp"

struct PlayerStateJumpAnimations {
    artist_api::Animation &roll;
};
class PlayerStateJump : public PlayerStateBase {
public:
    PlayerStateJump(PlayerStateProps &player, PlayerStateJumpAnimations &anims, artist_api::Animator &animator, dj::Sound &sndJump_, float jumpHeight)
        : m_player(player), anims_(anims), animator_(animator), sndJump_(sndJump_), m_jumpHeight(jumpHeight) {}

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

        animator_.changeTo(anims_.roll);
        animator_.setSpeed(1.0 / int(fmax(1, 4.0 - abs(m_player.gsp))));
    }

    void onLanding() override {
        m_player.stateMachine.changeTo(PlayerStateID::NORMAL);
    }

private:
    PlayerStateProps &m_player;
    PlayerStateJumpAnimations anims_;
    artist_api::Animator &animator_;
    dj::Sound &sndJump_;
    float m_jumpHeight;
};

struct PlayerStateDieAnimations {
    artist_api::Animation &die;
};
class PlayerStateDie : public PlayerStateBase {
public:
    PlayerStateDie(PlayerStateProps &player, PlayerStateDieAnimations &anims, artist_api::Animator &animator, dj::Sound &sndHurt)
        : m_player(player), anims_(anims), animator_(animator), sndHurt_(sndHurt) {}

    void onInit() override {
        m_player.spd.x = 0;
        m_player.spd.y = -7;

        m_player.colliderTerrain.forceToAir();
        m_player.audio.dj().playSound(sndHurt_);
    }

    void onUpdate() override {
        animator_.changeTo(anims_.die);
        animator_.setSpeed(0);
        if (m_player.spd.y < 16.0)
            m_player.spd.y += 0.21875; // TODO move gravity

        m_player.pos.y += m_player.spd.y;
    }

private:
    PlayerStateProps &m_player;
    PlayerStateDieAnimations anims_;
    artist_api::Animator &animator_;
    dj::Sound &sndHurt_;
};

struct PlayerStateSkidAnimations {
    artist_api::Animation &skidStart;
    artist_api::Animation &skid;
};
class PlayerStateSkid : public PlayerStateBase {
public:
    PlayerStateSkid(PlayerStateProps &player, PlayerStateSkidAnimations &anims, artist_api::Animator &animator, dj::Sound &sndSkid) : m_player(player), anims_(anims), animator_(animator), sndSkid_(sndSkid) {}

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

        if (int(animator_.getCurrentFrameIndex()) == 25) {
            animator_.changeTo(anims_.skid);
            animator_.setSpeed(0.125f);
        }
        else{
            animator_.changeTo(anims_.skidStart);
        }
    }

private:
    PlayerStateProps &m_player;
    PlayerStateSkidAnimations anims_;
    artist_api::Animator &animator_;
    dj::Sound &sndSkid_;
};

struct PlayerStateSpindashCDAnimations {
    artist_api::Animation &dash;
};
class PlayerStateSpindashCD : public PlayerStateBase {
public:
    PlayerStateSpindashCD(PlayerStateProps &player, PlayerStateSpindashCDAnimations &anims, artist_api::Animator &animator, dj::Sound& sndRoll) : m_player(player), anims_(anims), animator_(animator), sndRoll_(sndRoll) {}

    void onInit() override {
        m_timer = 45;
        m_player.pos.y += 5;
        m_player.audio.dj().playSound(sndRoll_);
    }

    void onUpdate() override {
        animator_.changeTo(anims_.dash);
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

        animator_.setSpeed((float(45 - m_timer) / 30));
    }

private:
    PlayerStateProps &m_player;
    PlayerStateSpindashCDAnimations anims_;
    artist_api::Animator &animator_;
    dj::Sound &sndRoll_;

    int m_timer;
};

struct PlayerStateRollAnimations {
    artist_api::Animation &roll;
};
class PlayerStateRoll : public PlayerStateBase {
public:
    PlayerStateRoll(PlayerStateProps &player, PlayerStateRollAnimations &anims, artist_api::Animator &animator, dj::Sound &sndRoll) : m_player(player), anims_(anims), animator_(animator), sndRoll_(sndRoll) {}

    void onInit() override {
        m_player.audio.dj().playSound(sndRoll_);
    }

    void onUpdate() override {
        animator_.changeTo(anims_.roll);
        animator_.setSpeed(1.0 / int(fmax(1, 4.0 - abs(m_player.gsp))));
        
        if (m_player.gsp == 0)
            m_player.stateMachine.changeTo(PlayerStateID::NORMAL);
        // animator_.tick();
    }

    void onLanding() override {
        m_player.stateMachine.changeTo(PlayerStateID::NORMAL);
    }

private:
    PlayerStateProps &m_player;
    PlayerStateRollAnimations anims_;
    artist_api::Animator &animator_;
    dj::Sound &sndRoll_;
};

struct PlayerStateHurtAnimations {
    artist_api::Animation &hurt;
};
class PlayerStateHurt : public PlayerStateBase {
public:
    PlayerStateHurt(PlayerStateProps &player, PlayerStateHurtAnimations &anims, artist_api::Animator &animator, dj::Sound &sndHurt) : m_player(player), anims_(anims), animator_(animator), sndHurt_(sndHurt) {}

    void onInit() override {
        animator_.changeTo(anims_.hurt);
        animator_.setSpeed(0);

        m_player.audio.dj().playSound(sndHurt_);

        m_player.spd.y = -4;
        m_player.spd.x = 2 * -gmath::sign(m_player.spd.x);
        if (m_player.spd.x == 0)
            m_player.spd.x = -2;
    }

    void onLanding() override {
        m_player.gsp = 0;
        m_player.spd.x = 0;
        m_player.stateMachine.changeTo(PlayerStateID::NORMAL);
    }

private:
    PlayerStateProps &m_player;
    PlayerStateHurtAnimations anims_;
    artist_api::Animator &animator_;
    dj::Sound &sndHurt_;
};

struct PlayerStateSpringAnimations {
    artist_api::Animation &spring;
    artist_api::Animation &fall;
};
class PlayerStateSpring : public PlayerStateBase {
public:
    PlayerStateSpring(PlayerStateProps &player, PlayerStateSpringAnimations &anims, artist_api::Animator &animator) : m_player(player), anims_(anims), animator_(animator) {}

    void onLanding() override {
        m_player.stateMachine.changeTo(PlayerStateID::NORMAL);
    }

    void onUpdate() override {
        if (m_player.spd.y < 0.0){
            animator_.changeTo(anims_.fall);
            // m_player.anim.set(51, 53, (0.125 + fabs(m_player.spd.y) / 25), true);
            animator_.setSpeed((0.125 + fabs(m_player.spd.y) / 25));
        }
        else{
            animator_.changeTo(anims_.spring);
            // m_player.anim.set(54, 55, 0.25, false);
            animator_.setSpeed(0.25f);
        }
        // animator_.tick();
    }

private:
    PlayerStateProps &m_player;
    PlayerStateSpringAnimations anims_;
    artist_api::Animator &animator_;
};