#include <zenilib.h>
#include <vector>
#include <string>

#include "Buggy.h"
#include "Tile.h"
#include "Score.h"

extern std::string test;
extern Score m_score;

using namespace Zeni;
using std::vector;

void Collidable::render(const Point2f &pos, const float &theta, Collidable *c) {
	unsigned int i, j = cbox.size() - 1;

	for (i = 0; i < cbox.size(); i++) {
		Point2f p0 = adjust_point(cbox[i], pos, theta);
		Point2f p1 = adjust_point(cbox[j], pos, theta);

		Color col = check_collision(pos, theta, c) ? get_Colors()["red"] : get_Colors()["yellow"];

		Line_Segment<Vertex2f_Color> l(Vertex2f_Color(p0, col), Vertex2f_Color(p1, col));
		get_Video().render(l);
		j = i;
	}
}

void Collidable::render(Collidable *c) {
	render(get_position(), get_theta(), c);
}

//TODO: rotate hit boxes with object rotation
//Return true if this object set at Point pos facing theta collides with c
bool Collidable::check_collision(const Point2f &pos, const float &theta, Collidable *c) {
	if (this == c)
		return false;

	if (cbox.size() == 1 && c->check_collision(adjust_point(cbox[0], pos, theta))) {
		collide(c);
		return true;
	}

	//Go through all the points in the collision shape
	unsigned int i, j = cbox.size() - 1;

	for (i = 0; i < cbox.size(); i++) {
		Point2f p0 = adjust_point(cbox[i], pos, theta);
		Point2f p1 = adjust_point(cbox[j], pos, theta);

		//check for collisions along each line segment
		if (p0.x == p1.x) {
			Point2f start = p0.y > p1.y ? p1 : p0;
			Point2f end = p0.y > p1.y ? p0 : p1;
			for (Point2f pp = start; pp.y <= end.y; pp.y++) {
				if (c->check_collision(pp)) {
					collide(c);
					return true;
				}
			}
		}
		else {
			Point2f start = p0.x > p1.x ? p1 : p0;
			Point2f end = p0.x > p1.x ? p0 : p1;
			for (Point2f pp = start; pp.x <= end.x && pp.y <= end.y; pp.x += 1, pp.y = liney(p0, p1, pp.x)) {
				if (c->check_collision(pp)) {
					collide(c);
					return true;
				}
			}
		}
		j = i;
	} // end for loop

	return false;
}

bool Collidable::check_collision(Collidable *c) {
	return check_collision(get_position(), get_theta(), c);
}

//Return true if this object is colliding with point collide_p
bool Collidable::check_collision(const Point2f &collide_p) {
	return pointInPolygon(collide_p);
}

//is point inside the polygon
bool Collidable::pointInPolygon(const Point2f &point) {
	int i, j, nvert = cbox.size();
	bool c = false;

	for (i = 0, j = nvert - 1; i < nvert; j = i++) {
		Point2f pi(adjust_point(cbox[i]));
		Point2f pj(adjust_point(cbox[j]));

		if (((pi.y) >= point.y != (pj.y >= point.y)) &&
			(point.x <= (pj.x - pi.x) * (point.y - pi.y) / (pj.y - pi.y) + pi.x)
			)
			return true;
			c = !c;
	}

	return c;
}

/*bool Collidable::pointInPolygon(const Point2f &pos) {
if (cbox.size() == 1) {
Point2f pc = adjust_point(cbox[0]);
return (pos.x == pc.x && pos.y == pc.y);
}

unsigned int  i, j = cbox.size() - 1;
bool oddNodes = false;

for (i = 0; i < cbox.size(); i++) {
Point2f pi(adjust_point(cbox[i]));
Point2f pj(adjust_point(cbox[j]));

if ((pi.y < pos.y && pj.y >= pos.y
|| pj.y < pos.y && pi.y >= pos.y)
&&  (pi.x <= pos.x || pj.x <= pos.x)) {
oddNodes ^= (pi.x + (pos.y - pi.y) / (pj.y - pi.y)*(pj.x - pi.x) < pos.x);
}
j = i;
}

return oddNodes;
}*/