#pragma once

#include <zenilib.h>
#include <vector>
#include <string>

#include "Collidable.h"

using namespace Zeni;

static const float tile_size = 64.0f;

class Tile : public Collidable {
	int id;
	Point2f position;
	std::string texture;
	float theta;
	bool flip;

public:
	Tile(int id_, std::string texture_, bool flip_ = false, std::vector<Point2f> collide_ = {}) 
		: Collidable(collide_),
		id(id_),
		texture(texture_),
		theta(0.0f),
		flip(flip_)
	{
	}

	static Tile *Tile::make_tile(int id_);

	void Tile::render(float x, float y) const;

	const Point2f &Tile::get_position() const { return position; }
	const float &get_theta() const { return theta; }
	int Tile::get_id() const { return id; }
	std::string Tile::get_texture() const { return texture; }

	void Tile::render(Point2f p) const {
		render(p.x, p.y); 
	}

	virtual void checkpoint() {}
	virtual void reset() {}

protected:
	void change_texture(std::string tex) {
		texture = tex;
	}
};

class Ground_Tile : public Tile {
	std::string texture;
	bool ckpt;
	bool gone;

public:
	Ground_Tile(int id_, std::string texture_, bool flip_ = false,
		std::vector<Point2f> collide_ = { Point2f(0.0f, 0.0f), Point2f(tile_size, 0.0f),
														Point2f(tile_size, tile_size), Point2f(0.0f, tile_size) })
		: Tile(id_, texture_, flip_, collide_ ),
		texture(texture_),
		ckpt(false),
		gone(false)
	{
	}

	void Ground_Tile::collide(Collidable *c) override;

	void Ground_Tile::collide_with_asteroid(Asteroid *a) override;
	void Ground_Tile::collide_with_buggy(Buggy *b) override;

	//bool Ground_Tile::check_collision(const Point2f &p) override {
		//return true;
	//}

	void checkpoint() override {
		ckpt = true;
	}

	void reset() override {
		if (!ckpt)
			change_texture(texture);
	}

};

class Ground_Half_Tile : public Ground_Tile {
public:
	Ground_Half_Tile(int id_, std::string texture_)
		: Ground_Tile(id_, texture_, false,
		std::vector<Point2f> { Point2f(0.0f, tile_size / 2), Point2f(tile_size, tile_size / 2),
						  Point2f(tile_size, tile_size), Point2f(0.0f, tile_size) })
	{
	}

};

class Slope_Bottom_Tile : public Ground_Tile {
public:
	Slope_Bottom_Tile(int id_, std::string texture_, bool flip_ = false)
		: Ground_Tile(id_, texture_, flip_,
		std::vector<Point2f> { Point2f(0.0f, tile_size), Point2f(tile_size, tile_size / 2), Point2f(tile_size, tile_size) })
	{
	}

};

class Slope_Top_Tile : public Ground_Tile {
public:
	Slope_Top_Tile(int id_, std::string texture_, bool flip_ = false)
		: Ground_Tile(id_, texture_, flip_,
		std::vector<Point2f> { Point2f(0.0f, tile_size / 2), Point2f(tile_size, 0.0f),
						  Point2f(tile_size, tile_size), Point2f(0.0f, tile_size) })
	{
	}

};

class Mars_Rock_Tile : public Tile {
	std::string texture;
	bool ckpt;

public:
	Mars_Rock_Tile(int id_, std::string texture_)
		: Tile(id_, texture_, false, std::vector<Point2f> { Point2f(0.0f, 0.0f), Point2f(tile_size, 0.0f),
		Point2f(tile_size, tile_size), Point2f(0.0f, tile_size) }),
		texture(texture_),
		ckpt(false)
	{
	}

	void Mars_Rock_Tile::collide(Collidable *c) override {
		test = "Rock Collide";
		c->collide_with_rock(this);
	}

	void Mars_Rock_Tile::collect() {
		change_texture("sky");
	}

	void Mars_Rock_Tile::collide_with_buggy(Buggy *b);

	void checkpoint() override {
		ckpt = true;
	}

	void reset() override {
		if (!ckpt)
			change_texture(texture);
	}

	//bool Mars_Rock_Tile::check_collision(const Point2f &pos) override {
	//	return true;
	//}
};