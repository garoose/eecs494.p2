#pragma once

#include <zenilib.h>
#include <vector>
#include <string>

extern std::string test;

using namespace Zeni;

class Buggy;
class Ground_Tile;
class Mars_Rock_Tile;
class Asteroid;

class Collidable {
private:
	std::vector<Point2f> cbox;

public:
	Collidable() {}

	Collidable(std::vector <Point2f> cbox_)
		: cbox(cbox_)
	{
	}

	Collidable(const Vector2f &size)
		: cbox(std::vector <Point2f> { Point2f(0.0f, 0.0f), Point2f(size.x, 0.0f), 
			Point2f(size.x, size.y), Point2f(0.0f, size.y) })
	{
	}

	//Need to define these in derived class
	virtual const Point2f &get_position() const = 0;
	virtual const float &get_theta() const = 0;
	virtual const Vector2f &get_size() const = 0;

	//Render lines to represent collision box
	void Collidable::render(const Point2f &pos, const float &theta, Collidable *c);
	void Collidable::render(Collidable *c);

	//TODO: rotate hit boxes with object rotation
	//Return true if this object set at Point pos facing theta collides with c
	virtual bool Collidable::check_collision(const Point2f &pos, const float &theta, Collidable *c);
	virtual bool Collidable::check_collision(Collidable *c); //use get_position() and get_theta()

	//Return true if this object is colliding with point collide_p
	virtual bool Collidable::check_collision(const Point2f &collide_p);

	virtual void Collidable::collide(Collidable *c) {
		test = "default Collide";
	}

	virtual void Collidable::collide_with_ground(Ground_Tile *g) {}
	virtual void Collidable::collide_with_rock(Mars_Rock_Tile *r) {}
	virtual void Collidable::collide_with_buggy(Buggy *b) {}
	virtual void Collidable::collide_with_asteroid(Asteroid *a) {}

private:
	float Collidable::liney(const Point2f &p0, const Point2f &p1, const float &x) const {
		return p0.y + ((p1.y - p0.y) / (p1.x - p0.x) * (x - p0.x));
	}

	//Adjusts a collision box corner for position and theta
	Point2f Collidable::adjust_point(const Point2f &p, const Point2f &pos, const float &theta) {
		Point2f ret = p;

		//ret.x = p.x * cos(theta);
		//ret.y = p.y * -sin(theta);

		return ret + pos;
	}

	Point2f Collidable::adjust_point(const Point2f &p) {
		return adjust_point(p, get_position(), get_theta());
	}

	inline int Collidable::isLeft(const Point2f &P0, const Point2f &P1, const Point2f &P2);
	int Collidable::pointInPolygonWinding(const Point2f &P);

	bool Collidable::pointInPolygon(const Point2f &point);
};