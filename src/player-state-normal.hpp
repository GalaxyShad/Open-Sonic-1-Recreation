#pragma once

#include "AnimMgr.h"
#include "player-state-base.hpp"
#include "player-state-machine.hpp"

class PlayerStateNormal : public PlayerStateBase {
public:
    PlayerStateNormal(PlayerStateProps& player) : m_player(player) {}

    void onInit() override {
        
    }

    void onUpdate() override {
        if (
            m_player.input.isKeyAction() 
            && m_player.colliderTerrain.isGrounded() 
            && m_player.colliderTerrain.isPlayerCanJump()
        ) {
            m_player.stateMachine.changeTo(PlayerStateID::JUMP);
        }

        if (fabs(m_player.gsp) > 2.5 
            && m_player.colliderTerrain.isGrounded() 
            && m_player.colliderTerrain.getMode() == PlayerSensorMode::FLOOR
        ) {
            if (m_player.gsp > 0 && m_player.input.isKeyLeft() ||
                m_player.gsp < 0 && m_player.input.isKeyRight()
            ) {
                m_player.stateMachine.changeTo(PlayerStateID::SKID);
                m_player.audio.playSound(SND_SKID);
            }
        } 

        if (m_player.colliderTerrain.isGrounded()) {
            if (fabs(m_player.gsp) <= 1.0 && m_player.input.isKeySpindash()) {
                m_player.stateMachine.changeTo(PlayerStateID::SPINDASH);
            } else if (m_player.input.isKeyDown() && m_player.gsp != 0.0) {
                m_player.stateMachine.changeTo(PlayerStateID::ROLL);
            }
        }

        animation();
    }
private:
    PlayerStateProps& m_player;
    int               m_idleTimer;

    void animation() {
        auto& anim = m_player.anim;
        auto& gsp = m_player.gsp; 
        bool diaAnim = false;
        bool ground = m_player.colliderTerrain.isGrounded();

        if (!ground || gsp != 0.0)
            m_idleTimer = 288;

        if (ground) {
            if (fabs(gsp) == 0.0) {
                if (m_idleTimer > 0) {
                    anim.set(0, 0, 0);
                    m_idleTimer--;
                } else  {
                    m_idleTimer--;
                    if (m_idleTimer < -72)
                        anim.set(2, 3, 0.042);
                    else 
                        anim.set(1, 1, 0);
                }
            } 
        }

        if ((fabs(gsp) > 0.0) && (fabs(gsp) < 6.0)) {
            if (diaAnim) 
                anim.set(26, 31, 1.0 / int(fmax(3, 8.0-abs(gsp))) );
            else 
                anim.set(4, 9, 1.0 / int(fmax(3, 8.0-abs(gsp))) );
        } else if ((fabs(gsp) >= 6.0) && (fabs(gsp) < 12.0)) {
            if (diaAnim) 
                anim.set(32, 35, 1.0 / int(fmax(2, 8.0-abs(gsp))) );
            else
                anim.set(11, 14, 1.0 / int(fmax(2, 10.0-abs(gsp))) );
        } else if (fabs(gsp) >= 12.0) {
            if (diaAnim)
                anim.set(47, 50, 1.0 / int(fmax(2, 8.0-abs(gsp))) );
            else 
                anim.set(43, 46, 1.0 / int(fmax(2, 10.0-abs(gsp))));
        }
    }

};
