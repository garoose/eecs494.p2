#include <zenilib.h>
#include <vector>
#include <string>

#include "Tile.h"
#include "Collidable.h"
#include "Asteroid.h"
#include "Buggy.h"

extern std::string test;

using namespace Zeni;

using std::map;
using std::vector;
using std::string;

void Tile::render(float x, float y) const
{
	Video &vr = get_Video();

	Vertex2f_Texture p0, p1, p2, p3;

	if (flip) {
		p0 = Vertex2f_Texture(Point2f(x, y), Point2f(1.0f, 0.0f));
		p1 = Vertex2f_Texture(Point2f(x, y + tile_size), Point2f(0.0f, 0.0f));
		p2 = Vertex2f_Texture(Point2f(x + tile_size, y + tile_size), Point2f(0.0f, 1.0f));
		p3 = Vertex2f_Texture(Point2f(x + tile_size, y), Point2f(1.0f, 1.0f));
	}
	else {
		p0 = Vertex2f_Texture(Point2f(x, y), Point2f(0.0f, 0.0f));
		p1 = Vertex2f_Texture(Point2f(x, y + tile_size), Point2f(0.0f, 1.0f));
		p2 = Vertex2f_Texture(Point2f(x + tile_size, y + tile_size), Point2f(1.0f, 1.0f));
		p3 = Vertex2f_Texture(Point2f(x + tile_size, y), Point2f(1.0f, 0.0f));
	}

	Material material(texture.c_str());

	Quadrilateral<Vertex2f_Texture> quad(p0, p1, p2, p3);
	quad.fax_Material(&material);

	vr.render(quad);
}

void Ground_Tile::collide(Collidable *c) {
	test = "Ground Collide";
	c->collide_with_ground(this);
}

void Ground_Tile::collide_with_asteroid(Asteroid *a) {
	change_texture("sky");
	gone = true;
}

void Ground_Tile::collide_with_buggy(Buggy *b) {
	b->explode();
}

void Mars_Rock_Tile::collide_with_buggy(Buggy *b) {
	collect();
	b->collect();
}

Tile *Tile::make_tile(int id_) {
	switch (id_) {
	case 6:
		return new Tile(id_, "sky3");
	case 5:
		return new Tile(id_, "sky2");
	case 0:
		return new Tile(id_, "sky");
	case 1:
		return new Ground_Tile(id_, "ground");
	case 2:
		return new Ground_Half_Tile(id_, "ground_half");
	case 3:
		return new Slope_Bottom_Tile(id_, "slope_bottom");
	case 4:
		return new Slope_Top_Tile(id_, "slope_top");
	case 7:
		return new Mars_Rock_Tile(id_, "mars_rock");
	case 8:
		return new Slope_Bottom_Tile(id_, "slope_bottom", true);
	case 9:
		return new Slope_Top_Tile(id_, "slope_top", true);
	default:
		return nullptr;
	}
}