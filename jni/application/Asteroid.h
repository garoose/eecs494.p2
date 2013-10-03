#pragma once

#include <zenilib.h>

#include "Game_Object.h"

using namespace Zeni;

class Asteroid : public Game_Object, public Collidable {
public:
	Asteroid::Asteroid(const Point2f &position_,
		const Vector2f &size_,
		const float &theta_,
		const float &speed_,
		const float &min_speed_,
		const float &max_speed_,
		const float &acceleration_)
		: Game_Object(position_, size_, theta_, speed_, min_speed_, max_speed_, acceleration_),
		Collidable(vector<Point2f> { Point2f(30.0f, 25.0f), Point2f(size_.x - 100.0f, 25.0f), Point2f(size_.x - 10.0f, size_.y - 10.0f), Point2f(30.0f, size_.y - 10.0f) })
		{	
		}

	void Asteroid::render() const {
		Game_Object::render("asteroid");
	}
};