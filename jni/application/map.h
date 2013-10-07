#pragma once

#include <zenilib.h>
#include <vector>
#include <string>

#include "Collidable.h"

class Asteroid;
class Game_Object;
class Tile;

using namespace Zeni;

class Map : public Collidable {
private:
	std::vector<std::vector<Tile *>> map;
	std::vector<Game_Object *> list;

	Point2f position;
	float theta;

public:
	Map::Map()
		: position({ 0.0f, 0.0f }),
		theta(0.0f)
	{
	}

	Map::Map(std::string filename)
		: Map()
	{
		load(filename);
	}

	virtual Map::~Map();

	//Not needed by Map collidable
	const Point2f &get_position() const override { return position;  }
	const float &get_theta() const override { return theta; }

	void Map::load(std::string filename);

	bool Map::check_collision(const Point2f &pos) override;

	void Map::render_all(Vector2f game_resolution, Point2f top_left, Collidable &b) const;
	void Map::step_all(const float &time_step, Vector2f game_resolution, Point2f top_left);

	Tile *Map::get(int x, int y) const;
	int Map::get_id(int x, int y) const;
	std::string Map::get_texture(int x, int y) const;
	
private:
	std::vector<std::string> Map::split_string(std::string line);
};