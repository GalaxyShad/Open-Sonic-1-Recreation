#pragma once

#include "core/_index.hpp"

#include "AnimMgr.h"

class IDrawer {
    virtual void draw(uint8_t tex, IntRect texRect, Vector2f _pos, Vector2i offset=Vector2i(0, 0),
                      float angle=0.0, bool horFlip=false, bool verFlip=false) = 0;

    virtual void draw(uint8_t tex, Frame frame, Vector2f _pos, float angle=0.0,
                      bool horFlip=false, bool verFlip=false) = 0;

    virtual void draw(const AnimMgr& anim, Vector2f _pos, float angle=0.0, 
                      bool horFlip=false, bool verFlip=false) = 0;
};

class Camera : public IDrawer {
    public:
        Camera(Screen& scr) : scr(scr) { }

        void create(Vector2f _pos, Size _levelSize, bool free);
        void update();

        bool isFree() { return free; }

		void draw(uint8_t tex, IntRect texRect, Vector2f _pos, Vector2i offset=Vector2i(0, 0),
			      float angle=0.0, bool horFlip=false, bool verFlip=false) override;
        void draw(uint8_t tex, Frame frame, Vector2f _pos, float angle=0.0, bool horFlip=false,
                  bool verFlip=false) override;
        void draw(const AnimMgr& anim, Vector2f _pos, float angle=0.0, bool horFlip=false, 
			      bool verFlip=false) override;
		
        Vector2f getPos()           { return pos; }
		void setPos(Vector2f _pos)  { pos = _pos; }

        Size getSize() { return size; }

        void setBottomBorder(int _value) { bottomBorder = _value; } 
        int getBottomBorder() { return bottomBorder; }
        void setRightBorder(int _value)  { rightBorder = _value;  } 

        Screen& getScr() { return scr; }

    private:
        Screen& scr;
        Vector2f pos;
        Size size;
        Size levelSize;   
        int bottomBorder = 0;
        int rightBorder = 0;
        bool m_free = false;
};
