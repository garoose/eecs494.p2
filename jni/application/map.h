#pragma once

#include <zenilib.h>
#include <vector>
#include <string>

#include "Collidable.h"

class Asteroid;
class Game_Object;
class Tile;

using namespace Zeni;

class Map {
private:
	std::vector<std::vector<Tile *>> map;
	std::vector<Game_Object *> list;

	const float buffer = 100.0f; //how far off the screen to stop rendering objects

public:
	Map::Map()
	{
	}

	Map::Map(std::string filename)
		: Map()
	{
		load(filename);
	}

	virtual Map::~Map();

	void Map::load(std::string filename);

	void Map::checkpoint();
	void Map::reset();

	bool Map::check_collision(Collidable *c, const Vector2f &delta_ = Vector2f(0.0f,0.0f));

	void Map::render_all(Vector2f game_resolution, Point2f top_left, Collidable *b);
	void Map::step_all(const float &time_step, Vector2f game_resolution, Point2f top_left);

	Tile *Map::get(int x, int y) const;
	int Map::get_id(int x, int y) const;
	std::string Map::get_texture(int x, int y) const;
	
private:
	std::vector<std::string> Map::split_string(std::string line);
};