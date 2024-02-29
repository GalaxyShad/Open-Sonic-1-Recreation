#pragma once

#include "core/_index.hpp"

#include "AnimMgr.h"

class IDrawer {
    virtual void draw(uint8_t tex, irect texRect, v2f _pos, v2i offset=v2i(0, 0),
                      float angle=0.0, bool horFlip=false, bool verFlip=false) = 0;

    virtual void draw(uint8_t tex, Frame frame, v2f _pos, float angle=0.0,
                      bool horFlip=false, bool verFlip=false) = 0;

    virtual void draw(const AnimMgr& anim, v2f _pos, float angle=0.0, 
                      bool horFlip=false, bool verFlip=false) = 0;
};

class Camera : public IDrawer {
    public:
        Camera(Screen& scr) : scr(scr) { }

        void create(v2f _pos, Size _levelSize, bool free);
        void update();

        bool isFree() { return m_free; }

		void draw(uint8_t tex, irect texRect, v2f _pos, v2i offset=v2i(0, 0),
			      float angle=0.0, bool horFlip=false, bool verFlip=false) override;
        void draw(uint8_t tex, Frame frame, v2f _pos, float angle=0.0, bool horFlip=false,
                  bool verFlip=false) override;
        void draw(const AnimMgr& anim, v2f _pos, float angle=0.0, bool horFlip=false, 
			      bool verFlip=false) override;
		
        v2f getPos()           { return pos; }
        v2f getCenterPosition()     { return v2f(pos.x + (float)size.width / 2, pos.y + (float)size.height / 2);}
		void setPos(v2f _pos)  { pos = _pos; }

        Size getSize() { return size; }

        void setBottomBorder(int _value) { bottomBorder = _value; } 
        int getBottomBorder() { return bottomBorder; }
        void setRightBorder(int _value)  { rightBorder = _value;  } 

        Screen& getScr() { return scr; }

    private:
        Screen& scr;
        v2f pos;
        Size size;
        Size levelSize;   
        int bottomBorder = 0;
        int rightBorder = 0;
        bool m_free = false;
};
