#include "../include/Collider.hpp"

#include <math.h>

#ifdef DEBUG
#include "../include/Camera.hpp"
#include "../include/Game.hpp"
#include <SDL.h>
#endif // DEBUG

bool Collider::showCollision = false;

Collider::Collider(GameObject& associated, Vec2 scale, Vec2 offset): Component(associated) {
    this->scale = scale;
    this->offset = offset;

	this->box.W = associated.box.GetW() * scale.GetX();
	this->box.H = associated.box.GetH() * scale.GetY();

	double angleRad = (associated.angleDeg * M_PI)/180;
	Vec2 newCenter = offset.rotate(angleRad) + associated.box.center();
	box.X = newCenter.GetX() - box.GetW() / 2;
	box.Y = newCenter.GetY() - box.GetH() / 2;
}

void Collider::Update(float dt) {
	if (associated.isStatic) return;

    this->box.W = associated.box.GetW() * scale.GetX();
    this->box.H = associated.box.GetH() * scale.GetY();

    double angleRad = (associated.angleDeg * M_PI)/180;
    Vec2 newCenter = offset.rotate(angleRad) + associated.box.center();
    box.X = newCenter.GetX() - box.GetW() / 2;
    box.Y = newCenter.GetY() - box.GetH() / 2;
}

void Collider::Render() {
#ifdef DEBUG
	if (!showCollision) return;

	int screenWidth = 1200;
	int screenHeight = 900;

	if (box.X + box.W < Camera::pos.GetX() || box.X > Camera::pos.GetX() + screenWidth ||
		box.Y + box.H < Camera::pos.GetY() || box.Y > Camera::pos.GetY() + screenHeight) {
		return;
		}

    Vec2 center( box.center() );
	SDL_Point points[5];

	Vec2 point = (Vec2(box.X, box.Y) - center).rotate( associated.angleDeg/(180/M_PI) )
					+ center - Camera::pos;
	points[0] = {(int)point.X, (int)point.Y};
	points[4] = {(int)point.X, (int)point.Y};
	
	point = (Vec2(box.X + box.W, box.Y) - center).rotate( associated.angleDeg/(180/M_PI) )
					+ center - Camera::pos;
	points[1] = {(int)point.X, (int)point.Y};
	
	point = (Vec2(box.X + box.W, box.Y + box.H) - center).rotate( associated.angleDeg/(180/M_PI) )
					+ center - Camera::pos;
	points[2] = {(int)point.X, (int)point.Y};
	
	point = (Vec2(box.X, box.Y + box.H) - center).rotate( associated.angleDeg/(180/M_PI) )
					+ center - Camera::pos;
	points[3] = {(int)point.X, (int)point.Y};

	SDL_SetRenderDrawColor(Game::GetInstance().GetRenderer(), 255, 0, 0, SDL_ALPHA_OPAQUE);
	SDL_RenderDrawLines(Game::GetInstance().GetRenderer(), points, 5);
#endif // DEBUG
};


bool Collider::Is(string type) {
    return type == "Collider";
}

void Collider::SetScale(Vec2 scale) {
    this->scale = scale;
}

void Collider::SetOffset(Vec2 offset) {
    this->offset = offset;
}

