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
	float rows = 0, num_rows = stoi(line);

	while (getline(file, line) && rows < num_rows) {
		if (line[0] == '#')
			continue;

		auto words = split_string(line);
		vector<Tile *> temp;

		if (!len)
			len = words.size();
		else if (words.size() != len)
			throw new exception("Inconsistent map file line lengths");

		for (unsigned int i = 0; i < len; i++) {
			temp.push_back(Tile::make_tile(std::stoi(words[i]), Point2f(i * tile_size.x, rows * tile_size.y)));
		}

		map.push_back(temp);

		rows++;
	}

	//Read in object list
	while (getline(file, line)) {
		if (line[0] == '#')
			continue;

		auto words = split_string(line);

		if (words[0] == "asteroid") {
			list.push_back(new Asteroid(Point2f(stof(words[1]), stof(words[2])), 
				Vector2f(stof(words[3]), stof(words[4])), Global::pi * stof(words[5]), stof(words[6])));
		}

	}

	file.close();
}

bool Map::check_collision(Collidable *c, const Vector2f &delta_) {
	float cx = c->get_position().x + delta_.x;
	float cy = c->get_position().y + delta_.y;

	for (unsigned int x = cx; x < cx + c->get_size().x; x += tile_size.x) {
		for (unsigned int y = cy; y < cy + c->get_size().y; y += tile_size.y) {
			//Find the relevant tile to collide with
			unsigned int tx = int(floor(x / tile_size.x));
			unsigned int ty = int(floor(y / tile_size.y));

			if ((map.size() > ty) && (map[0].size() > tx))
				return c->check_collision(c->get_position() + delta_, c->get_theta(), get(tx, ty));
		}
	}

	return false;
}

void Map::render_all(Vector2f game_resolution, Point2f top_left, Collidable *b) {
	for (float x = 0; x < map[0].size() * tile_size.x; x += tile_size.x) {
		for (float y = 0; y < map.size() * tile_size.y; y += tile_size.y) {
			unsigned int tx = int(floor((x) / tile_size.x));
			unsigned int ty = int(floor((y) / tile_size.y));

			if ((map.size() > ty) && (map[0].size() > tx)) {
				get(tx, ty)->render(x, y);
				get(tx, ty)->Collidable::render(Point2f(x, y), 0.0f, nullptr);
			}
		}
	}

	for each (Game_Object *o in list) {
		float x = o->get_position().x;
		if (!o->is_gone() && x >= top_left.x - buffer && x <= top_left.x + game_resolution.x + buffer) {
			o->render();
			o->render_collisions(this);
		}
	}
}

void Map::step_all(const float &time_step, Vector2f game_resolution, Point2f top_left) {
	for each (Game_Object *o in list) {
		float x = o->get_position().x;
		if (!o->is_gone() && x >= top_left.x - buffer && x <= top_left.x + game_resolution.x + buffer)
			o->step(time_step, this);
	}
}

vector<string> Map::split_string(string line) {
	std::istringstream iss(line);
	std::istream_iterator<string> begin(iss), end;
	return std::vector<string>(begin, end);
}