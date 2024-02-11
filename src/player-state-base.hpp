#pragma once

struct PlayerStateBase {
    virtual void onInit() {}
    virtual void onUpdate() {}
    virtual void onEnd() {}
    virtual void onLanding() {}
};
