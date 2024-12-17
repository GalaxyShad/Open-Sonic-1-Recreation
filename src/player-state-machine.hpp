#pragma once

#include "core/Geometry.h"
#include "core/game_enviroment/InputMgr.h"
#include "player-collider.hpp"
#include "player-state-base.hpp"
#include <assert.h>
#include <cstddef>
#include <functional>
#include <vector>

enum class PlayerStateID {
    NORMAL,
    JUMP,
    ROLL, 
    SKID, 
    SPINDASH,
    SPRING,
    HURT,
    DIE,
};

struct PlayerStateProps {
    v2f&  pos;
    v2f&  spd; 
    float& gsp;
    IInputMgr& input;
    Audio& audio;
    AnimMgr& anim;

    PlayerCollider& colliderTerrain;
    class PlayerStateMachine& stateMachine;

};


class PlayerStateMachine {
public:
    PlayerStateMachine(PlayerStateProps& props) : m_props(props) 
    {
        // m_stateList.push_back(PlayerStateNormal());
    }

    void add(PlayerStateBase* state) {
        m_stateList.push_back(state);

        if (m_stateList.size() == 1) {
            m_currentID = 0;
            m_stateList[m_currentID]->onInit();
        }
    }

    void update() {
        m_stateList[m_currentID]->onUpdate();
    }

    void pushLanding() {
        m_stateList[m_currentID]->onLanding();
    }

    bool isCurling() {
        switch (currentId()) {
            case PlayerStateID::JUMP:
            case PlayerStateID::ROLL:
            case PlayerStateID::SPINDASH:
                return true;

            default:
                return false;
        }
    }

    void changeTo(PlayerStateID toId) {
        if (m_currentID == (int)toId)
            return;

        m_stateList[m_currentID]->onEnd();

        m_currentID = (int)toId;

        m_stateList[m_currentID]->onInit();
    }

    PlayerStateID currentId() {
        return (PlayerStateID)m_currentID;
    }

private: 
    PlayerStateProps&            m_props;
    int                          m_currentID;
    std::vector<PlayerStateBase*> m_stateList;
};


