#pragma once

#include <zenilib.h>
#include <vector>
#include <string>

#include "Collidable.h"

using namespace Zeni;

static const Vector2f tile_size(64.0f, 64.0f);

class Tile : public Collidable {
	int id;
	Point2f position;
	std::string texture;
	float theta;
	bool flip;

protected:
	bool gone;

public:
	Tile(int id_, std::string texture_, const Point2f &pos_, bool flip_ = false, std::vector<Point2f> collide_ = {}) 
		: Collidable(collide_),
		id(id_),
		texture(texture_),
		position(pos_),
		theta(0.0f),
		flip(flip_),
		gone(false)
	{
	}

	static Tile *Tile::make_tile(int id_, const Point2f &pos_);

	void Tile::render(float x, float y) const;

	const Point2f &Tile::get_position() const override { return position; }
	const float &get_theta() const override { return theta; }
	const Vector2f &get_size() const override { return tile_size; }

	int Tile::get_id() const { return id; }
	std::string Tile::get_texture() const { return texture; }

	bool Tile::check_collision(const Point2f &pos_) override {
		if (!gone)
			return Collidable::check_collision(pos_);
		return false;
	}

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

public:
	Ground_Tile(int id_, std::string texture_, const Point2f &pos_, bool flip_ = false,
		std::vector<Point2f> collide_ = { Point2f(0.0f, 0.0f), Point2f(tile_size.x, 0.0f),
										Point2f(tile_size.x, tile_size.y), Point2f(0.0f, tile_size.y) })
		: Tile(id_, texture_, pos_, flip_, collide_ ),
		texture(texture_),
		ckpt(false)
	{
	}

	void Ground_Tile::collide(Collidable *c) override;

	void Ground_Tile::collide_with_asteroid(Asteroid *a) override;
	void Ground_Tile::collide_with_buggy(Buggy *b) override;

	void checkpoint() override {
		ckpt = true;
	}

	void reset() override {
		if (!ckpt) {
			change_texture(texture);
			gone = false;
		}
	}

};

class Ground_Half_Tile : public Ground_Tile {
public:
	Ground_Half_Tile(int id_, std::string texture_, const Point2f &pos_)
		: Ground_Tile(id_, texture_, pos_, false,
		std::vector<Point2f> { Point2f(0.0f, tile_size.y / 2), Point2f(tile_size.x, tile_size.y / 2),
						  Point2f(tile_size.x, tile_size.y), Point2f(0.0f, tile_size.y) })
	{
	}

};

class Slope_Bottom_Tile : public Ground_Tile {
public:
	Slope_Bottom_Tile(int id_, std::string texture_, const Point2f &pos_, bool flip_ = false)
		: Ground_Tile(id_, texture_, pos_, flip_,
		std::vector<Point2f> { Point2f(0.0f, tile_size.y), Point2f(tile_size.x, tile_size.y / 2), Point2f(tile_size.x, tile_size.y) })
	{
	}

};

class Slope_Top_Tile : public Ground_Tile {
public:
	Slope_Top_Tile(int id_, std::string texture_, const Point2f &pos_, bool flip_ = false)
		: Ground_Tile(id_, texture_, pos_, flip_,
		std::vector<Point2f> { Point2f(0.0f, tile_size.y / 2), Point2f(tile_size.x, 0.0f),
						  Point2f(tile_size.x, tile_size.y), Point2f(0.0f, tile_size.y) })
	{
	}

};

class Mars_Rock_Tile : public Tile {
	std::string texture;
	bool ckpt;

public:
	Mars_Rock_Tile(int id_, std::string texture_, const Point2f &pos_)
		: Tile(id_, texture_, pos_, false, std::vector<Point2f> { Point2f(0.0f, 0.0f), Point2f(tile_size.x, 0.0f),
		Point2f(tile_size.x, tile_size.y), Point2f(0.0f, tile_size.y) }),
		texture(texture_),
		ckpt(false)
	{
	}

	void Mars_Rock_Tile::collide(Collidable *c) override {
		test = "Rock Collide";
		collect();
		c->collide_with_rock(this);
	}

	void Mars_Rock_Tile::collect() {
		change_texture("sky");
		gone = true;
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