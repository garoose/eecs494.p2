#pragma once

#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <iterator>
#include <exception>

using std::vector;
using std::string;
using std::exception;

static const vector<string> textures{ "blank", "ground" };

static const float tile_size = 64.0f;

struct Tile {
	int id;
	string texture;
	float size;
	
public:
	Tile::Tile(int id_) {
		id = id_;
		texture = textures[id];
	}

	void Tile::render(int x, int y)
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
};

class Map {
	vector<vector<Tile>> map;

public:
	Map::Map() {}

	Map::Map(string filename) {
		std::cout << "in map init" << std::endl;
		load(filename);
	}

	void Map::load(string filename) {
		std::ifstream file(filename);
		string line;
		unsigned int len = 0;

		while (getline(file, line)) {
			std::istringstream iss(line);
			std::istream_iterator<string> begin(iss), end;
			vector<string> words(begin, end);
			vector<Tile> temp;

			if (!len)
				len = words.size();
			else if (words.size() != len)
				throw new exception("Inconsistent map file line lengths");

			for (unsigned int i = 0; i < len; i++) {
				std::cout << stoi(words[i]);
				Tile t(stoi(words[i]));
				temp.push_back(t);
			}
			std::cout << std::endl;
			map.push_back(temp);
		}

		file.close();
	}

	void render_all(Point2f top_left, Vector2f game_resolution) {
		for (int x = 0; x < game_resolution.x; x += tile_size) {
			for (int y = 0; y < game_resolution.y; y += tile_size) {
				float ty = (top_left.y + y) / tile_size;
				float tx = (top_left.x + x) / tile_size;

				if ((map.size() > ty) && (map[0].size() > tx))
					get(tx, ty).render(x, y);
			}
		}
	}

	Tile get(int x, int y) {
		return map[y][x];
	}

	int get_id(int x, int y) {
		return get(x, y).id;
	}

	string get_texture(int x, int y) {
		return get(x, y).texture;
	}
};