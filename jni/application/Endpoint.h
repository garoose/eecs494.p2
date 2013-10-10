#pragma once

#include <zenilib.h>
#include <vector>

#include "Collidable.h"
#include "Game_Object.h"

using namespace Zeni;

const Vector2f endsize(1.0f, 1024.0);

class Endpoint : public Game_Object {
private:
	Map *map;

public:

	Endpoint::Endpoint(const Point2f &pos, Map *m)
		: Game_Object(pos, endsize),
		map(m)
	{
		collide_init_box(endsize);
	}

	void Endpoint::render() const override {
	}
	void Endpoint::step(const float &, Map *) override {}

	void Endpoint::collide(Collidable *c) override {
		c->collide_with_endpoint(this);
	}

	void Endpoint::collide_with_buggy(Buggy *b);
	void Endpoint::collide_with_tire(Tire *t);

};