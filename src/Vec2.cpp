#include "../include/Vec2.hpp"
#include <cmath>

using namespace std;

Vec2::Vec2(float x, float y){
    X = x;
    Y = y;
}

float Vec2::GetX() const {
    return X;
}

float Vec2::GetY() const {
    return Y;
}

Vec2 Vec2::operator+(const Vec2& vector2) {
    return Vec2(X + vector2.GetX(), Y + vector2.GetY());
}

Vec2 Vec2::operator-(const Vec2& vector2) {
    return Vec2(X - vector2.GetX(), Y - vector2.GetY());
}

Vec2 Vec2::operator*(float scalar) const {
    return Vec2(X * scalar, Y * scalar);
}

float Vec2::magnitude(){
    return sqrt((X*X) + (Y*Y));
}

Vec2 Vec2::normalize(){
    float mag = magnitude();
    if (mag == 0)
        return Vec2(0,0);

    return Vec2(X/mag, Y/mag);
}

float Vec2::distance(Vec2& vector1, Vec2& vector2) {
    return (vector1 - vector2).magnitude();
}

float Vec2::angle() {
    return atan2(Y, X);
}

float Vec2::slope(Vec2& vector1, Vec2& vector2) {
    Vec2 diff = vector2 - vector1;
    return tan(diff.angle());
}

Vec2 Vec2::rotate(float angle) {
    float x = X * cos(angle) - Y * sin(angle);
    float y = X * sin(angle) + Y * cos(angle);
    return Vec2(x, y);
}

Vec2& Vec2::operator=(const Vec2& vector2){
    if (this != &vector2) {
        X = vector2.GetX();
        Y = vector2.GetY();
    }
    return *this;
}

bool Vec2::operator!=(const Vec2& vector2){
    if (this->GetX() == vector2.GetX() && this->GetY() == vector2.GetY()) {
        return false;
    }
    return true;
}

bool Vec2::operator==(const Vec2& vector2) {
    if (this->GetX() == vector2.GetX() && this->GetY() == vector2.GetY()) {
        return true;
    }
    return false;
}

