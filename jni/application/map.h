#pragma once

#include <zenilib.h>
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

using namespace Zeni;

static const float tile_size = 64.0f;

static const vector<string> textures{
	"blank", //0
	"ground", //1
};

class Tile {
	int id;
	Point2f position;
	string texture;
	bool can_collide;
	
public:
	Tile::Tile(int id_, Point2f pos_, bool collide_) {
		id = id_;
		Point2f position = pos_;
		texture = textures[id];
		can_collide = collide_;
	}

	virtual bool collide(Point2f pos) { return can_collide; }

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

	void Tile::render(Point2f p) const { render(p.x, p.y); }
};

class Ground_Tile : public Tile {
public:
	Ground_Tile(int id_, Point2f pos_) : Tile(id_, pos_, true) {};

	virtual bool collide(Point2f pos) {
		return true;
		Point2f tpos = get_position();
		if ((pos.x >= tpos.x) && (pos.x <= (tpos.x + tile_size)) &&
			(pos.y >= tpos.y) && (pos.y <= (tpos.y + tile_size)))
			return true;
		return false;
	}
};

class Map {
	vector<vector<Tile>> map;
	float m_scroll_speed;
	Point2f m_top_left;

public:
	Map::Map(float scroll_speed_ = 0.0f) 
		: m_scroll_speed(scroll_speed_) 
	{
	}

	Map::Map(string filename) {
		load(filename);
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
			vector<Tile> temp;

			if (!len)
				len = words.size();
			else if (words.size() != len)
				throw new exception("Inconsistent map file line lengths");

			for (unsigned int i = 0; i < len; i++) {
				temp.push_back(Tile(stoi(words[i]), Point2f(float(row), float(i)), stoi(words[i]) ? true : false));
			}

			map.push_back(temp);

			row++;
		}

		file.close();
	}

	void Map::step(const float &time_step_) {
		m_top_left.x += m_scroll_speed * time_step_;
	}

	void Map::render_all(Vector2f game_resolution) {
		render_all(game_resolution, m_top_left);
	}

	bool Map::collide(Point2f pos) {
		unsigned int ty = int(floor(pos.y / tile_size));
		unsigned int tx = int(floor(pos.x / tile_size));

		if ((map.size() > ty) && (map[0].size() > tx))
			return get(tx, ty).collide(pos);

		return false;
	}

	void Map::render_all(Vector2f game_resolution, Point2f top_left) {
		for (float x = top_left.x; x < game_resolution.x + top_left.x; x += tile_size) {
			for (float y = top_left.y; y < game_resolution.y + top_left.y; y += tile_size) {
				unsigned int ty = int(floor(y / tile_size));
				unsigned int tx = int(floor(x / tile_size));

				if ((map.size() > ty) && (map[0].size() > tx))
					get(tx, ty).render(x, y);
			}
		}
	}

	Tile &get(int x, int y) {
		return map[y][x];
	}

	int get_id(int x, int y) {
		return get(x, y).get_id();
	}

	string get_texture(int x, int y) {
		return get(x, y).get_texture();
	}
};