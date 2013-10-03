#pragma once

#include <vector>

#include <zenilib.h>

#include "Map.h"

using std::vector;
using namespace Zeni;

class Collidable
{
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

	void Collidable::render(const Point2f &pos, Collidable &c) const {
		for (unsigned int p = 0; p < cbox.size(); p++) {
			Point2f p0 = pos + cbox[p];
			Point2f p1 = p == cbox.size() - 1 ? pos + cbox[0] : pos + cbox[p + 1];

			Color col = collide(pos, c) ? get_Colors()["red"] : get_Colors()["yellow"];

			Line_Segment<Vertex2f_Color> l(Vertex2f_Color(p0, col), Vertex2f_Color(p1, col));
			get_Video().render(l);
		}
	}

	//TODO: rotate hit boxes with object rotation
	//Return true if this object at pos collides with c
	virtual bool Collidable::collide(const Point2f &pos, Collidable &c) const {
		if (cbox.size() == 1)
			return c.collide(pos + cbox[0]);
			
		//Go through all the points in the collision shape
		for (unsigned int p = 0; p < cbox.size(); p++) {
			Point2f p0 = pos + cbox[p];
			Point2f p1 = p == cbox.size() - 1 ? pos + cbox[0] : pos + cbox[p + 1];
			float delta = 1;

			//check for collisions along each line segment
			Point2f start = p0.x > p1.x ? p0 : p1;
			Point2f end = p0.x > p1.x ? p1 : p0;
			for (Point2f pp = start; pp.x <= end.x && pp.y <= end.y; pp.x += delta, pp.y = liney(p0, p1, pp.x)) {
				if (c.collide(pp))
					return true;
			}
		}

		return false;
	}


	//Return true if this object is colliding with pos
	virtual bool Collidable::collide(const Point2f &pos) {
		return pointInPolygon(pos);
	}

private:
	float liney(const Point2f &p0, const Point2f &p1, const float &x) const {
		return p0.y + ((p1.y - p0.y) / (p1.x - p0.x) * (x - p0.x));
	}

	bool pointInPolygon(const Point2f &p) {
		if (cbox.size() == 1)
			return (p.x == cbox[0].x && p.y == cbox[0].y);

		unsigned int  i, j = cbox.size() - 1;
		bool oddNodes = false;

		for (i = 0; i < cbox.size(); i++) {
			if ((cbox[i].y < p.y && cbox[j].y >= p.y
				|| cbox[j].y < p.y && cbox[i].y >= p.y)
				&&  (cbox[i].x <= p.x || cbox[j].x <= p.x)) {
					oddNodes ^= (cbox[i].x + (p.y - cbox[i].x) / (cbox[j].y - cbox[i].y)*(cbox[j].x - cbox[i].x) < p.x);
			}
			j = i;
		}

		return oddNodes;
	}
};