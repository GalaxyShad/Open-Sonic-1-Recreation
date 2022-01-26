#pragma once

struct Vector2f {
    float x;
    float y;
    Vector2f(float _x, float _y): 
        x(_x), y(_y) {}
    Vector2f() {}
};

struct Vector2i {
    int x;
    int y;
    Vector2i(int _x, int _y):
        x(_x), y(_y) {}
    Vector2i() {}
};

struct Vector3i {
    int x;
    int y;
    int z;
    Vector3i(int _x, int _y, int _z): 
        x(_x), y(_y), z(_z) {}
    Vector3i() {}
};

struct IntRect {
    int left;
    int top;
    int width;
    int height;
    IntRect(int _left, int _top, int _width, int _height): 
        left(_left), top(_top), width(_width), height(_height) {}
    IntRect() {}
};

struct FloatRect {
    float left;
    float top;
    float width;
    float height;
    FloatRect(float _left, float _top, float _width, float _height): 
        left(_left), top(_top), width(_width), height(_height) {}
    FloatRect() {}
};

struct Size {
    int width;
    int height;
    Size(int _width, int _height):
        width(_width), height(_height) {}
    Size() {}
};
