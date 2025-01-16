#include "Camera.h"
#include "core/Screen.h"
#include "core/game_enviroment/artist/ArtistStructs.h"

#include <string>

void Camera::create(v2f _pos, Size _levelSize, bool free) {
    levelSize = _levelSize;
    bottomBorder = levelSize.height * 256;
    rightBorder = levelSize.width * 256;
    pos = _pos;
    size = Size(427, 240);
    m_free = free;
}

void Camera::update() {
    if (m_free)
        return;

    if (pos.x < 0.0)
        pos.x = 0.0;
    if (pos.y < 0.0)
        pos.y = 0.0;
    if (pos.x + size.width > levelSize.width * 256)
        pos.x = levelSize.width * 256 - size.width;
    if (pos.y + size.height > levelSize.height * 256)
        pos.y = levelSize.height * 256 - size.height;

    if (pos.x + size.width > rightBorder)
        pos.x = rightBorder - size.width;
    if (pos.y + size.height > bottomBorder)
        pos.y = bottomBorder - size.height;
}

