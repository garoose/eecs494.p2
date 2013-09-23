#pragma once

#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <iterator>

using std::vector;
using std::string;

static const vector<string> textures{ "blank", "ground" };

struct Tile {
	int id;
	string texture;
	
public:
	Tile::Tile(int id_) {
		id = id_;
		texture = textures[id];
	}
};

class Map {
	vector<vector<Tile>> map;

public:
	Map::Map() {}

	Map::Map(string filename) {
		load(filename);
	}

	void Map::load(string filename) {
		std::ifstream file(filename);

		string line;

		while (getline(file, line)) {
			std::istringstream iss(line);
			std::istream_iterator<string> begin(iss), end;
			vector<string> words(begin, end);
			vector<Tile> temp;

			for (unsigned int i = 0; i < words.size(); i++) {
				Tile t(stoi(words[i]));
				temp.push_back(t);
			}

			map.push_back(temp);
		}

		file.close();
	}

	Tile get(int x, int y) {
		return map[x][y];
	}

	int get_id(int x, int y) {
		return get(x, y).id;
	}

	string get_texture(int x, int y) {
		return get(x, y).texture;
	}
};