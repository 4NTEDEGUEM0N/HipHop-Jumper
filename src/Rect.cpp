#include "../include/Rect.hpp"

using namespace std;

Rect::Rect(float x, float y, float w, float h){
    X = x;
    Y = y;
    W = w;
    H = h;
}

float Rect::GetX() const{
    return X;
}

float Rect::GetY() const{
    return Y;
}

float Rect::GetW() const{
    return W;
}

float Rect::GetH() const{
    return H;
}

Rect Rect::operator+(const Vec2& vector) {
    return Rect(X + vector.GetX(), Y + vector.GetY(), W, H);
}

Vec2 Rect::center() {
    return Vec2(X + W/2, Y + H/2);
}

float Rect::distance(Rect& rect1, Rect& rect2) {
    Vec2 center1 = rect1.center();
    Vec2 center2 = rect2.center();
    return Vec2::distance(center1, center2);
}

bool Rect::contains(const Vec2& vector) {
    return (vector.GetX() >= X && vector.GetX() <= X + W && vector.GetY() >= Y && vector.GetY() <= Y + H);
}

Rect& Rect::operator=(const Rect& rect2) {
    if (this != &rect2) {
        X = rect2.GetX();
        Y = rect2.GetY();
        W = rect2.GetW();
        H = rect2.GetH();
    }

    return *this;
}
