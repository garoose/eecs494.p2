#pragma once

#include <zenilib.h>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <iterator>
#include <exception>

#include "Collidable.h"
#include "Tile.h"

using std::vector;
using std::string;
using std::exception;

using namespace Zeni;

class Map : public Collidable {
	vector<vector<Tile *>> map;

public:
	Map::Map()
	{
	}

	Map::Map(string filename) {
		load(filename);
	}

	Map::~Map() {
		for (unsigned int x = 0; x < map[0].size(); x++) {
			for (unsigned int y = 0; y < map.size(); y++) {
				//delete get(x, y);
			}
		}
	}

	void Map::load(string filename) {
		std::ifstream file(filename);
		string line;
		unsigned int len = 0;
		unsigned int row = 0;

		while (getline(file, line)) {
			std::istringstream iss(line);
			std::istream_iterator<string> begin(iss), end;
			vector<string> words(begin, end);
			vector<Tile *> temp;

			if (!len)
				len = words.size();
			else if (words.size() != len)
				throw new exception("Inconsistent map file line lengths");

			for (unsigned int i = 0; i < len; i++) {
				temp.push_back(Tile::make_tile(stoi(words[i])));
			}

			map.push_back(temp);

			row++;
		}

		file.close();
	}

	bool Map::collide(const Point2f &pos) override {
		//Find the relevant tile to collide with
		unsigned int tx = int(floor(pos.x / tile_size));
		unsigned int ty = int(floor(pos.y / tile_size));

		if ((map.size() > ty) && (map[0].size() > tx))
			return get(tx, ty)->collide(pos);

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
};