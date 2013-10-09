#pragma once

#include <zenilib.h>
#include <vector>
#include <string>

#include "Game_Object.h"
#include "Map.h"

extern std::string test;

using namespace Zeni;
using std::vector;

class Asteroid : public Game_Object {
	std::string texture;
	Chronometer<Time> remove;

public:
	Asteroid::Asteroid(const Point2f &position_,
		const Vector2f &size_,
		const float &theta_,
		const float &speed_)
		: Game_Object(position_, size_,
		vector<Point2f> { Point2f(0.0f, 0.0f), Point2f(size_.x, 0.0f), Point2f(size_.x, size_.y), Point2f(0.0f, size_.y) },
		theta_, speed_),
		texture("asteroid")
		{	
		}

	void Asteroid::render() const override {
		if (!gone)
			Game_Object::render(texture.c_str());
	}
	
	void Asteroid::render_collisions(Map *m) {
		Collidable::render(nullptr);
	}

	void Asteroid::collide(Collidable *c) override {
		test = "Asteroid Collide";
		if (!gone) {
			c->collide_with_asteroid(this);
		}

		explode();
	}

	void Asteroid::explode() {
		remove.start();
		texture = "asteroid_broken";
	}

	void Asteroid::collide_with_buggy(Buggy *b);

	void Asteroid::step(const float &time_step, Map *m) {
		if (gone)
			return;

		if (remove.seconds() == 0)
			move_forward(time_step * get_speed(), m);

		if (m->check_collision(this)) {
			
		}

		if (remove.seconds() >= 1.0f)
			gone = true;
	}

	void Asteroid::reset() {
		gone = false;
		remove.reset();
		Game_Object::reset();
	}
};

