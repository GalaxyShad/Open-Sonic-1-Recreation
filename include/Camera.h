#pragma once

#include "Geometry.h"
#include "AnimMgr.h"
#include "Screen.h"

class Camera {
    public:
        Camera(Screen& scr) : scr(scr) { }

        void create(Vector2f _pos, Size _levelSize);
        void update();

		void draw(uint8_t tex, IntRect texRect, Vector2f _pos, Vector2i offset=Vector2i(0, 0),
			      float angle=0.0, bool horFlip=false, bool verFlip=false);
        void draw(uint8_t tex, Frame frame, Vector2f _pos, float angle=0.0, bool horFlip=false,
                  bool verFlip=false);
        void draw(const AnimMgr& anim, Vector2f _pos, float angle=0.0, bool horFlip=false, 
			      bool verFlip=false);
		
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
};
