#pragma once

#include <zenilib.h>
#include <vector>
#include <map>
#include <string>

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

public:
	Tile(int id_, string texture_, vector<Point2f> collide_ = {}) 
		: Collidable(collide_)
	{
		id = id_;
		texture = texture_;
	}

	static Tile *Tile::make_tile(int id_);

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

protected:
	void change_texture(string tex) {
		texture = tex;
	}
};

class Ground_Tile : public Tile {
public:
	Ground_Tile(int id_, string texture_) 
		: Tile(id_, texture_, vector<Point2f> { Point2f(0.0f, 0.0f), Point2f(tile_size, 0.0f), Point2f(tile_size, tile_size), Point2f(0.0f, tile_size) })
	{
	}

	bool Ground_Tile::collide(const Point2f &pos) override {
		return true;
	}

};

class Ground_Half_Tile : public Tile {
public:
	Ground_Half_Tile(int id_, string texture_)
		: Tile(id_, texture_, vector<Point2f> { Point2f(0.0f, tile_size / 2), Point2f(tile_size, tile_size / 2), Point2f(tile_size, tile_size), Point2f(0.0f, tile_size) })
	{
	}

	bool Ground_Half_Tile::collide(const Point2f &pos) override {
		return true;
	}

};


class Slope_Bottom_Tile : public Tile {
public:
	Slope_Bottom_Tile(int id_, string texture_)
		: Tile(id_, texture_, vector<Point2f> { Point2f(0.0f, tile_size), Point2f(tile_size, tile_size / 2), Point2f(tile_size, tile_size) })
	{
	}

};

class Slope_Top_Tile : public Tile {
public:
	Slope_Top_Tile(int id_, string texture_)
		: Tile(id_, texture_, vector<Point2f> { Point2f(0.0f, tile_size / 2), Point2f(tile_size, 0.0f), Point2f(tile_size, tile_size), Point2f(0.0f, tile_size) })
	{
	}

	bool Slope_Top_Tile::collide(const Point2f &pos) override {

		return true;
	}

};

class Mars_Rock_Tile : public Tile {
public:
	Mars_Rock_Tile(int id_, string texture_)
		: Tile(id_, texture_)
	{
	}

	bool Mars_Rock_Tile::collide(const Point2f &pos) override {
		change_texture("sky");
		return false;
	}
};

Tile *Tile::make_tile(int id_) {
	switch (id_) {
	case 6:
		return new Tile(id_, "sky3");
	case 5:
		return new Tile(id_, "sky2");
	case 0:
		return new Tile(id_, "sky");
	case 1:
		return new Ground_Tile(id_, "ground");
	case 2:
		return new Ground_Half_Tile(id_, "ground_half");
	case 3:
		return new Slope_Bottom_Tile(id_, "slope_bottom");
	case 4:
		return new Slope_Top_Tile(id_, "slope_top");
	case 7:
		return new Mars_Rock_Tile(id_, "mars_rock");
	default:
		return nullptr;
	}
}