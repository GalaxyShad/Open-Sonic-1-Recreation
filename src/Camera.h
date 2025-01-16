#pragma once

#include "core/_index.hpp"

class Camera {
public:
    Camera(Screen &scr) : scr(scr) {}

    void create(v2f _pos, Size _levelSize, bool free);
    void update();

    bool isFree() { return m_free; }

    v2f getPos() { return pos; }
    v2f getCenterPosition() {
        return v2f(pos.x + (float)size.width / 2,
                   pos.y + (float)size.height / 2);
    }
    void setPos(v2f _pos) { pos = _pos; }

    Size getSize() { return size; }

    void setBottomBorder(int _value) { bottomBorder = _value; }
    int getBottomBorder() { return bottomBorder; }
    void setRightBorder(int _value) { rightBorder = _value; }

    Screen &getScr() { return scr; }

private:
    Screen &scr;
    v2f pos;
    Size size;
    Size levelSize;
    int bottomBorder = 0;
    int rightBorder = 0;
    bool m_free = false;
};
