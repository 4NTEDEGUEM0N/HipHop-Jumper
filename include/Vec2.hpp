#ifndef VEC2_HPP
#define VEC2_HPP

#include <string>

using namespace std;

class Vec2 {
public:
    float X;
    float Y;

    Vec2(float x = 0, float y = 0);
    float GetX() const;
    float GetY() const;
    Vec2 operator+(const Vec2& vector2);
    Vec2 operator-(const Vec2& vector2);
    Vec2 operator*(float scalar) const;
    float magnitude();
    Vec2 normalize();
    static float distance(Vec2& vector1, Vec2& vector2);
    float angle();
    static float slope(Vec2& vector1, Vec2& vector2);
    Vec2 rotate(float angle);
    Vec2& operator=(const Vec2& vector2);
    bool operator!=(const Vec2& vector2);
    bool operator==(const Vec2& vector2);
};



#endif //VEC2_HPP