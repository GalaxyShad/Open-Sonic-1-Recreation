#pragma once

#include "AnimMgr.h"
#include "core/game_enviroment/artist/Animator.h"
#include "core/game_enviroment/artist/ArtistStructs.h"
#include "player-state-base.hpp"
#include "player-state-machine.hpp"

struct PlayerStateNormalAnimations {
    artist_api::Animation &idle;
    artist_api::Animation &boredStart;
    artist_api::Animation &bored;
    artist_api::Animation &walk;
    artist_api::Animation &run;
    artist_api::Animation &dash;
    artist_api::Animation &sit;
    artist_api::Animation &push;
    artist_api::Animation &lookUp;
};

class PlayerStateNormal : public PlayerStateBase {
public:
    PlayerStateNormal(PlayerStateProps &player, PlayerStateNormalAnimations &anims, artist_api::Animator &animator, dj::Sound &sndSkid)
        : m_player(player), anims_(anims), animator_(animator), sndSkid_(sndSkid) {}

    void onInit() override {}

    void onUpdate() override {
        animation();

        if (m_player.input.isKeyAction() &&
            m_player.colliderTerrain.isGrounded() &&
            m_player.colliderTerrain.isPlayerCanJump()) {
            m_player.stateMachine.changeTo(PlayerStateID::JUMP);
        }

        if (fabs(m_player.gsp) > 2.5 && m_player.colliderTerrain.isGrounded() &&
            m_player.colliderTerrain.getMode() == PlayerSensorMode::FLOOR) {
            if (m_player.gsp > 0 && m_player.input.isKeyLeft() ||
                m_player.gsp < 0 && m_player.input.isKeyRight()) {
                m_player.stateMachine.changeTo(PlayerStateID::SKID);

                auto &store = m_player.audio.store();

                m_player.audio.dj().playSound(sndSkid_);
            }
        }

        if (m_player.colliderTerrain.isGrounded()) {
            if (fabs(m_player.gsp) <= 1.0 && m_player.input.isKeySpindash()) {
                m_player.stateMachine.changeTo(PlayerStateID::SPINDASH);
            } else if (m_player.input.isKeyDown() && m_player.gsp != 0.0) {
                m_player.stateMachine.changeTo(PlayerStateID::ROLL);
            }
        }

    }

private:
    PlayerStateProps &m_player;
    PlayerStateNormalAnimations anims_;
    artist_api::Animator &animator_;
    dj::Sound& sndSkid_;
    int m_idleTimer;

    void animation() {
        // auto &anim = m_player.anim;
        auto &gsp = m_player.gsp;
        bool diaAnim = false;
        bool ground = m_player.colliderTerrain.isGrounded();

        if (!ground || gsp != 0.0)
            m_idleTimer = 288;

        if (ground) {
            if (fabs(gsp) == 0.0) {
                if (m_idleTimer > 0) {
                    animator_.changeTo(anims_.idle);
                    animator_.setSpeed(0);
                    m_idleTimer--;
                } else {
                    m_idleTimer--;
                    if (m_idleTimer < -72){
                        animator_.changeTo(anims_.bored);
                        animator_.setSpeed(0.042);
                    }
                    else{
                        animator_.changeTo(anims_.boredStart);
                        animator_.setSpeed(0);
                    }
                }
            }
        }

        if ((fabs(gsp) > 0.0) && (fabs(gsp) < 6.0)) {
            // if (!diaAnim)
            animator_.changeTo(anims_.walk);
            animator_.setSpeed(1.0 / int(fmax(3, 8.0 - abs(gsp))));
        } else if ((fabs(gsp) >= 6.0) && (fabs(gsp) < 12.0)) {
            // if (!diaAnim) 
            animator_.changeTo(anims_.run);
            animator_.setSpeed(1.0 / int(fmax(2, 10.0 - abs(gsp))));
        } else if (fabs(gsp) >= 12.0) {
            // if (!diaAnim) 
            animator_.changeTo(anims_.dash);
            animator_.setSpeed(1.0 / int(fmax(2, 10.0 - abs(gsp))));
        }
    }
};
