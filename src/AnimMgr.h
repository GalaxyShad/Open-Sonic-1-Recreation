#pragma once

#include "Geometry.h"
#include "Screen.h"

class AnimMgr {
    public:
		void create(uint8_t _tex);
		void set(int _firstFrame, int _lastFrame, float _spd, bool _isReversive = false);
        void tick();
		
        uint8_t getTexture() const  { return tex; }
        bool getFlip() const { return flip; }
        bool getReversive() const { return isReversive; }

        float getCurFrame()  const  { return curFrame; }
        int   getLastFrame() const  { return lFrameIndex; };
    private:
		uint8_t tex;
        int fFrameIndex = 0;
        int lFrameIndex = 0;
        float curFrame = 0.0;
        float spd = 0.0;
		bool flip = false;
		bool isReversive = false;
};