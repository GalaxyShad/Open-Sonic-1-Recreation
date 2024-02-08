#pragma once

struct v2f {
    float x;
    float y;
    v2f(float _x, float _y): 
        x(_x), y(_y) {}
    v2f() {}
};

struct v2i {
    int x;
    int y;
    v2i(int _x, int _y):
        x(_x), y(_y) {}
    v2i() {}
};

struct v3i {
    int x;
    int y;
    int z;
    v3i(int _x, int _y, int _z): 
        x(_x), y(_y), z(_z) {}
    v3i() {}
};

struct irect {
    int left;
    int top;
    int width;
    int height;
    irect(int _left, int _top, int _width, int _height): 
        left(_left), top(_top), width(_width), height(_height) {}
    irect() {}
};

struct frect {
    float left;
    float top;
    float width;
    float height;
    frect(float _left, float _top, float _width, float _height): 
        left(_left), top(_top), width(_width), height(_height) {}
    frect() {}
};

struct Size {
    int width;
    int height;
    Size(int _width, int _height):
        width(_width), height(_height) {}
    Size() {}
};
