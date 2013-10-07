#pragma once

#include <zenilib.h>
#include <vector>

using namespace Zeni;
using std::vector;

class Buggy;
class Tile;

class Collidable {
private:
	vector<Point2f> cbox;

public:
	Collidable() {}

	Collidable(vector <Point2f> cbox_)
		: cbox(cbox_)
	{
	}

	Collidable(const Vector2f &size)
		: cbox(vector <Point2f> { Point2f(0.0f, 0.0f), Point2f(size.x, 0.0f), Point2f(size.x, size.y), Point2f(0.0f, size.y) })
	{
	}

	//Need to define these in derived class
	virtual const Point2f &get_position() const = 0;
	virtual const float &get_theta() const = 0;

	void Collidable::render(Collidable *c) {
		for (unsigned int p = 0; p < cbox.size(); p++) {
			Point2f p0 = get_position() + cbox[p];
			Point2f p1 = p == cbox.size() - 1 ? get_position() + cbox[0] : get_position() + cbox[p + 1];

			Color col = check_collision(get_position(), get_theta(), c) ? get_Colors()["red"] : get_Colors()["yellow"];

			Line_Segment<Vertex2f_Color> l(Vertex2f_Color(p0, col), Vertex2f_Color(p1, col));
			get_Video().render(l);
			/*
			if (p0.x == p1.x) {
				Point2f start = p0.y > p1.y ? p1 : p0;
				Point2f end = p0.y > p1.y ? p0 : p1;
				for (Point2f pp = start; pp.y <= end.y; pp.y++) {
					col = get_Colors()["purple"];
					Line_Segment<Vertex2f_Color> ll(Vertex2f_Color(pp, col), Vertex2f_Color(Point2f(pp.x, pp.y - 10.0f), col));
					get_Video().render(ll);
				}
			} else {
				Point2f start = p0.x > p1.x ? p1 : p0;
				Point2f end = p0.x > p1.x ? p0 : p1;
				for (Point2f pp = start; pp.x <= end.x && pp.y <= end.y; pp.x += 1, pp.y = liney(p0, p1, pp.x)) {
					col = get_Colors()["purple"];
					Line_Segment<Vertex2f_Color> ll(Vertex2f_Color(pp, col), Vertex2f_Color(Point2f(pp.x, pp.y - 10.0f), col));
					get_Video().render(ll);
				}
			}
			*/
		}
	}

	//TODO: rotate hit boxes with object rotation
	//Return true if this object set at Point pos facing theta collides with c
	virtual bool Collidable::check_collision(const Point2f &pos, const float &theta, Collidable *c) {
		if (cbox.size() == 1 && c->check_collision(pos + cbox[0])) {
			collide(c);
			c->collide(this);
			return true;
		}

		//Go through all the points in the collision shape
		for (unsigned int p = 0; p < cbox.size(); p++) {
			Point2f p0 = pos + cbox[p];
			Point2f p1 = p == cbox.size() - 1 ? pos + cbox[0] : pos + cbox[p + 1];

			//check for collisions along each line segment
			if (p0.x == p1.x) {
				Point2f start = p0.y > p1.y ? p1 : p0;
				Point2f end = p0.y > p1.y ? p0 : p1;
				for (Point2f pp = start; pp.y <= end.y; pp.y++) {
					if (c->check_collision(pp)) {
						collide(c);
						c->collide(this);
						return true;
					}
				}
			} else {
				Point2f start = p0.x > p1.x ? p1 : p0;
				Point2f end = p0.x > p1.x ? p0 : p1;
				for (Point2f pp = start; pp.x <= end.x && pp.y <= end.y; pp.x += 1, pp.y = liney(p0, p1, pp.x)) {
					if (c->check_collision(pp)) {
						collide(c);
						c->collide(this);
						return true;
					}
				}
			}
		}

		return false;
	}

	virtual bool Collidable::check_collision(Collidable *c) {
		return check_collision(get_position(), get_theta(), c);
	}

	//Return true if this object is colliding with point collide_p
	virtual bool Collidable::check_collision(const Point2f &collide_p) {
		return pointInPolygon(collide_p);
	}

	virtual void Collidable::collide(const Collidable *c)
	{
	}

	virtual void Collidable::collide(const Tile *c)
	{
	}

	virtual void Collidable::collide(const Buggy *c)
	{
	}

private:
	float liney(const Point2f &p0, const Point2f &p1, const float &x) const {
		return p0.y + ((p1.y - p0.y) / (p1.x - p0.x) * (x - p0.x));
	}

	//Adjusts a collision box corner for position and theta
	Point2f adjust_point(const Point2f &pos) {
		return pos + get_position();
	}

	//FIXME: comparing absolute to relative
	bool pointInPolygon(const Point2f &pos) {
		Point2f p = get_position();
		const float &theta = get_theta();

		if (cbox.size() == 1) {
			Point2f pc = adjust_point(cbox[0]);
			return (p.x == pc.x && p.y == pc.y);
		}

		unsigned int  i, j = cbox.size() - 1;
		bool oddNodes = false;

		for (i = 0; i < cbox.size(); i++) {
			Point2f pi(adjust_point(cbox[i]));
			Point2f pj(adjust_point(cbox[j]));

			if ((pi.y < p.y && pj.y >= p.y
				|| pj.y < p.y && pi.y >= p.y)
				&&  (pi.x <= p.x || pj.x <= p.x)) {
					oddNodes ^= (pi.x + (p.y - pi.y) / (pj.y - pi.y)*(pj.x - pi.x) < p.x);
			}
			j = i;
		}

		return oddNodes;
	}
};