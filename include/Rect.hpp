#ifndef RECT_HPP
#define RECT_HPP

#include <string>
#include "Vec2.hpp"

using namespace std;

class Rect {
public:
    float X;
    float Y;
    float W;
    float H;
    int Z=0;

    Rect(float x = 0, float y = 0, float w = 0, float h = 0);
    float GetX() const;
    float GetY() const;
    float GetW() const;
    float GetH() const;
    Rect operator+(const Vec2& vector);
    Vec2 center();
    static float distance(Rect& rect1, Rect& rect2);
    bool contains(const Vec2& vector);
    Rect& operator=(const Rect& rect2);
};



#endif //RECT_HPP
