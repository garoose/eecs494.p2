#include "Map.h"

#include <zenilib.h>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <iterator>
#include <exception>

#include "Collidable.h"
#include "Game_Object.h"
#include "Tile.h"

#include "Asteroid.h"

using std::vector;
using std::string;
using std::exception;

using namespace Zeni;

Map::~Map() {
	for (unsigned int x = 0; x < map[0].size(); x++) {
		for (unsigned int y = 0; y < map.size(); y++) {
			//delete get(x, y);
		}
	}

	for each (Game_Object *o in list) {
		delete o;
	}
}

Tile *Map::get(int x, int y) const {
	return map[y][x];
}

int Map::get_id(int x, int y) const {
	return get(x, y)->get_id();
}

string Map::get_texture(int x, int y) const {
	return get(x, y)->get_texture();
}

void Map::load(string filename) {
	std::ifstream file(filename);
	string line;
	unsigned int len = 0;

	//get the number of rows
	getline(file, line);
	int rows = stoi(line);

	while (getline(file, line) && rows > 0) {
		auto words = split_string(line);
		vector<Tile *> temp;

		if (!len)
			len = words.size();
		else if (words.size() != len)
			throw new exception("Inconsistent map file line lengths");

		for (unsigned int i = 0; i < len; i++) {
			temp.push_back(Tile::make_tile(std::stoi(words[i])));
		}

		map.push_back(temp);

		rows--;
	}

	//Read in object list
	while (getline(file, line)) {
		auto words = split_string(line);

		if (words[0] == "asteroid") {
			list.push_back(new Asteroid(Point2f(stof(words[1]), stof(words[2])), 
				Vector2f(stof(words[3]), stof(words[4])), Global::pi * stof(words[5]), stof(words[6])));
		}

	}

	file.close();
}

bool Map::check_collision(const Point2f &pos) {
	//Find the relevant tile to collide with
	unsigned int tx = int(floor(pos.x / tile_size));
	unsigned int ty = int(floor(pos.y / tile_size));

	if ((map.size() > ty) && (map[0].size() > tx))
		return false;
	return get(tx, ty)->check_collision(pos);

	return false;
}

void Map::render_all(Vector2f game_resolution, Point2f top_left, Collidable &b) const {
	for (float x = 0; x < map[0].size() * tile_size; x += tile_size) {
		for (float y = 0; y < map.size() * tile_size; y += tile_size) {
			unsigned int tx = int(floor((x) / tile_size));
			unsigned int ty = int(floor((y) / tile_size));

			if ((map.size() > ty) && (map[0].size() > tx)) {
				get(tx, ty)->render(x, y);
				//get(tx, ty)->Collidable::render(Point2f(x, y), b);
			}
		}
	}

	for each (Game_Object *o in list) {
		float x = o->get_position().x;
		if (x >= top_left.x && x <= top_left.x + game_resolution.x)
			o->render();
	}
}

void Map::step_all(const float &time_step, Vector2f game_resolution, Point2f top_left) {
	for each (Game_Object *o in list) {
		float x = o->get_position().x;
		if (x >= top_left.x && x <= top_left.x + game_resolution.x)
			o->step(time_step, this);
	}
}

vector<string> Map::split_string(string line) {
	std::istringstream iss(line);
	std::istream_iterator<string> begin(iss), end;
	return std::vector<string>(begin, end);
}