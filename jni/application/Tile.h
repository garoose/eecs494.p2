#pragma once

#include <zenilib.h>
#include <vector>
#include <map>
#include <string>
#include <cassert>

#include "Collidable.h"

using std::map;
using std::vector;
using std::string;

using namespace Zeni;

class Tile;

static const float tile_size = 64.0f;

class Tile : public Collidable {
	int id;
	Point2f position;
	string texture;
	bool can_collide;

public:
	Tile(int id_, string texture_, bool collide_) {
		id = id_;
		texture = texture_;
		can_collide = collide_;
	}

	static Tile *Tile::make_tile(int id_);

	virtual bool Tile::collide(const Point2f &pos) { assert(false); return can_collide; }

	void Tile::render(float x, float y) const
	{
		Video &vr = get_Video();

		Vertex2f_Texture p0(Point2f(x, y), Point2f(0.0f, 0.0f));
		Vertex2f_Texture p1(Point2f(x, y + tile_size), Point2f(0.0f, 1.0f));
		Vertex2f_Texture p2(Point2f(x + tile_size, y + tile_size), Point2f(1.0f, 1.0f));
		Vertex2f_Texture p3(Point2f(x + tile_size, y), Point2f(1.0f, 0.0f));
		Material material(texture.c_str());

		Quadrilateral<Vertex2f_Texture> quad(p0, p1, p2, p3);
		quad.fax_Material(&material);

		vr.render(quad);
	}

	Point2f Tile::get_position() const { return position; }
	int Tile::get_id() const { return id; }
	string Tile::get_texture() const { return texture; }

	void Tile::render(Point2f p) const {
		render(p.x, p.y); 
	}
};

class Ground_Tile : public Tile {
public:
	Ground_Tile(int id_, string texture_) : Tile(id_, texture_, true) {};

	virtual bool Ground_Tile::collide(const Point2f &pos) {
		return true;
		Point2f tpos = get_position();
		if ((pos.x >= tpos.x) && (pos.x <= (tpos.x + tile_size)) &&
			(pos.y >= tpos.y) && (pos.y <= (tpos.y + tile_size)))
			return true;
		return false;
	}
};

Tile *Tile::make_tile(int id_) {
	switch (id_) {
	case 0:
		return new Tile(0, "blank", false);
	case 1:
		return new Tile(1, "ground", true);
	default:
		return nullptr;
	}
}