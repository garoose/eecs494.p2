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

	void render(const Point2f &pos, Map &m) const {
		for (unsigned int c = 0; c < cbox.size(); c++) {
			Point2f p0 = pos + cbox[c];
			Point2f p1 = c == cbox.size() - 1 ? pos + cbox[0] : pos + cbox[c + 1];

			Color col = collide(pos, m) ? get_Colors()["red"] : get_Colors()["yellow"];

			Line_Segment<Vertex2f_Color> l(Vertex2f_Color(p0, col), Vertex2f_Color(p1, col));
			get_Video().render(l);
		}
	}

	//TODO: rotate hit boxes with object rotation
	bool Collidable::collide(const Point2f &pos, Map &m) const {
		if (cbox.size() == 1)
			return m.collide(pos + cbox[0]);

		for (unsigned int c = 0; c < cbox.size(); c++) {
			Point2f p0 = pos + cbox[c];
			Point2f p1 = c == cbox.size() - 1 ? pos + cbox[0] : pos + cbox[c + 1];
			float delta = 1;

			//TODO: figure out how to clean this up
			if (p0.x == p1.x) {
				if (p0.y > p1.y) {
					for (Point2f p = p0; p.y > p1.y; p.y--) {
						if (m.collide(p))
							return true;
					}
				} else {
					for (Point2f p = p0; p.y < p1.y; p.y++) {
						if (m.collide(p))
							return true;
					}
				}
			} else if (p0.x > p1.x) {
				for (Point2f p = p0; p.x > p1.x; p.x--) {
					if (m.collide(p))
						return true;
				}
			} else {
				for (Point2f p = p0; p.x < p1.x; p.x += delta, p.y = liney(p0, p1, p.x)) {
					if (m.collide(p))
						return true;
				}
			}
		}

		return false;
	}

private:
	float liney(const Point2f &p0, const Point2f &p1, const float &x) const {
		return p0.y + ((p1.y - p0.y) / (p1.x - p0.x) * (x - p0.x));
	}
};