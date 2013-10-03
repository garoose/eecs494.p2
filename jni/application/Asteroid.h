#pragma once

#include <zenilib.h>

#include "Game_Object.h"
#include "Map.h"

using namespace Zeni;

class Asteroid : public Game_Object, public Collidable {
public:
	Asteroid::Asteroid(const Point2f &position_,
		const Vector2f &size_,
		const float &theta_,
		const float &speed_)
		: Game_Object(position_, size_, theta_, speed_),
		Collidable(vector<Point2f> { Point2f(0.0f, 0.0f), Point2f(size_.x, 0.0f), Point2f(size_.x, size_.y), Point2f(0.0f, size_.y) })
		{	
		}

	void Asteroid::render() const override {
		Game_Object::render("asteroid");
	}

	void Asteroid::step(const float &time_step, Map &m) {
		move_forward(time_step * get_speed(), m);
	}
};