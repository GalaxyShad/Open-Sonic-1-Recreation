#ifndef __CLIONPROJECTS_OPEN_SONIC_1_RECREATION_SRC_CORE_GAME_ENVIROMENT_ARTIST_ANIMATOR_H_
#define __CLIONPROJECTS_OPEN_SONIC_1_RECREATION_SRC_CORE_GAME_ENVIROMENT_ARTIST_ANIMATOR_H_

#include "ArtistStructs.h"

namespace artist_api {

class Animator {
private:
    Animation* anim_;
    float currentFrame_ = 0.f;
    float speed_ = 1.f;

public:
    explicit Animator(Animation& baseAnimation) : anim_(&baseAnimation) {}

public:
    void changeTo(Animation& animation) { 
        anim_ = &animation;

        if (&animation != anim_) {
            currentFrame_ = 0.f;
        } 
    }

    void setSpeed(float spd) { speed_ = spd; }

    void tick() {
        currentFrame_ += speed_;

        while (currentFrame_ < 0) {
            currentFrame_ += anim_->frames.size();
        }

        while (currentFrame_ >= anim_->frames.size()) {
            currentFrame_ -= anim_->frames.size();
        }
    }

    void setCurrentFrame(float indexCurrentFrame){
        currentFrame_=indexCurrentFrame;
    }

    const Sprite &getCurrentFrame() {
        return anim_->frames[static_cast<int>(currentFrame_)];
    }

    const int getCurrentFrameIndex() {
        return static_cast<int>(currentFrame_);
    }
};

}; // namespace artist_api

#endif // __CLIONPROJECTS_OPEN_SONIC_1_RECREATION_SRC_CORE_GAME_ENVIROMENT_ARTIST_ANIMATOR_H_
