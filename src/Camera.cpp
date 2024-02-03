#include "Camera.h"

#include <string> 

void Camera::create(Vector2f _pos, Size _levelSize, bool free) {
    levelSize = _levelSize;
	bottomBorder = levelSize.height * 256;
	rightBorder  = levelSize.width * 256;
	pos = _pos;
	size = scr.getSize();
	m_free = free;
}

void Camera::update() {
	if (m_free) return;

    if (pos.x < 0.0) 
        pos.x = 0.0;
    if (pos.y < 0.0) 
        pos.y = 0.0;
    if (pos.x + size.width > levelSize.width  * 256)
        pos.x = levelSize.width * 256 - size.width;
    if (pos.y + size.height > levelSize.height * 256)
        pos.y = levelSize.height * 256 - size.height;

	if (pos.x + size.width > rightBorder)
        pos.x = rightBorder - size.width;
	if (pos.y + size.height > bottomBorder)
        pos.y = bottomBorder - size.height;
}


void Camera::draw(uint8_t tex, IntRect texRect, Vector2f _pos, Vector2i offset,
			      float angle, bool horFlip, bool verFlip) {
	scr.drawTextureRect(tex, texRect, Vector2f(_pos.x - pos.x, _pos.y - pos.y),
				 		offset, angle, horFlip, verFlip);
}

void Camera::draw(uint8_t tex, Frame frame, Vector2f _pos, float angle, bool horFlip,
                 bool verFlip) {
	scr.drawTextureRect(tex, frame, Vector2f(_pos.x - pos.x, _pos.y - pos.y),
				 		angle, horFlip, verFlip);
}

void Camera::draw(const AnimMgr& anim, Vector2f _pos, float angle, bool horFlip,
			      bool verFlip) {
	
	uint16_t aFrame = (uint16_t)anim.getCurFrame();
	uint8_t aTextureID = anim.getTexture();

	const Texture* tex = scr.getTexture(aTextureID);

	bool flip = horFlip;
	if (anim.getReversive())
		flip = anim.getFlip();


	if (tex && aFrame < tex->framesLen) {	
		scr.drawTextureRect(aTextureID, tex->frames[aFrame], Vector2f(_pos.x - pos.x, _pos.y - pos.y), 
				 			angle, flip, verFlip);
	}
}