#pragma once

#include <zenilib.h>
#include <vector>

#include "Collidable.h"
#include "Game_Object.h"

using namespace Zeni;

const Vector2f ckptsize(1.0f, 1024.0);

class Checkpoint : public Game_Object {
private:
	Map *map;
	Color col;

public:

	Checkpoint::Checkpoint(const Point2f &pos, Map *m)
		: Game_Object(pos, ckptsize),
		map(m),
		col(get_Colors()["white"])
	{
		collide_init_box(ckptsize);
	}

	void Checkpoint::render() const override {
		Line_Segment<Vertex2f_Color> l(Vertex2f_Color(get_position(), col), Vertex2f_Color(get_position() + get_size(), col));
		get_Video().render(l);
	}
	void Checkpoint::step(const float &, Map *) override {}

	void Checkpoint::collide(Collidable *c) override {
		c->collide_with_checkpoint(this);
		col = get_Colors()["green"];
	}

	void Checkpoint::collide_with_buggy(Buggy *b);
	void Checkpoint::collide_with_tire(Tire *t);
	
};