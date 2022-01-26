#include "AnimMgr.h"


void AnimMgr::create(uint8_t _tex) {
	tex = _tex;
}

void AnimMgr::set(int _firstFrame, int _lastFrame, float _spd, bool _isReversive) {
	isReversive = _isReversive;
    spd = _spd;

	if (!isReversive)
		flip = false;

	if (fFrameIndex != _firstFrame)
		curFrame = _firstFrame;

    fFrameIndex = _firstFrame;
    lFrameIndex = _lastFrame;

    if (curFrame < fFrameIndex)
        curFrame = fFrameIndex;
	if (curFrame >= lFrameIndex+1)
		curFrame = fFrameIndex;
}

void AnimMgr::tick() {
	
	if (!flip)
		curFrame += spd;
	else
		curFrame -= spd;

	if (!isReversive) {
		if (curFrame >= lFrameIndex+1)
			curFrame = fFrameIndex;
		if (curFrame < fFrameIndex)
			curFrame = lFrameIndex;
	} else {
		if (curFrame >= lFrameIndex+1) {
			flip = true;
			curFrame = lFrameIndex;
		}
		if (curFrame < fFrameIndex) {
			flip = false;
			curFrame = fFrameIndex;
		}
	}
}
