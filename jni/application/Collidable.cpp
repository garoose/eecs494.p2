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

	for (i = 0; i < cbox.size(); j = i++) {
		Point2f p0 = adjust_point(cbox[j], pos, theta);
		Point2f p1 = adjust_point(cbox[i], pos, theta);

		Color col = get_Colors()["yellow"];
		if (c && c->check_collision(this))
			col = get_Colors()["red"];

		Line_Segment<Vertex2f_Color> l(Vertex2f_Color(p0, col), Vertex2f_Color(p1, col));
		get_Video().render(l);
	}
}

void Collidable::render(Collidable *c) {
	render(get_position(), get_theta(), c);
}

float determinant(Vector2f vec1, Vector2f vec2){
	return vec1.x * vec2.y - vec1.y * vec2.x;
}

//one edge is a-b, the other is c-d
bool edgeIntersection(const Vector2f &a, const Vector2f &b, const Vector2f &c, const Vector2f &d){
	float det = determinant(b - a, c - d);
	float t = determinant(c - a, c - d) / det;
	float u = determinant(b - a, c - a) / det;
	if ((t < 0) || (u < 0) || (t > 1) || (u > 1)) {
		return false;
		//return NO_INTERSECTION;
	}
	else {
		return true;
		//return a * (1 - t) + t * b;
	}
}

//TODO: rotate hit boxes with object rotation
//Return true if this object set at Point pos facing theta collides with c
bool Collidable::check_collision(const Point2f &pos, const float &theta, Collidable *c) {
	bool ret = false;

	if (this == c)
		return false;

	//Go through all the points in the collision shape
	unsigned int i, j = cbox.size() - 1;

	for (i = 0; i < cbox.size(); j = i++) {
		Point2f p0 = adjust_point(cbox[j], pos, theta);
		Point2f p1 = adjust_point(cbox[i], pos, theta);

		//c->check_collision(p0, p1);

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
		} else {
			Point2f start = p0.x > p1.x ? p1 : p0;
			Point2f end = p0.x > p1.x ? p0 : p1;
			for (Point2f pp = start; pp.x <= end.x && pp.y <= end.y; pp.x += 1, pp.y = liney(p0, p1, pp.x)) {
				if (c->check_collision(pp)) {
					collide(c);
					return true;
				}
			}
		}
	} // end for loop

	return ret;
}

bool Collidable::check_collision(Collidable *c) {
	return check_collision(get_position(), get_theta(), c);
}

bool Collidable::check_collision(const Point2f &p1, const Point2f &p2) {
	int i, j, nvert = cbox.size();

	for (i = 0, j = nvert - 1; i < nvert; j = i++) {
		if (edgeIntersection(p1, p2, adjust_point(cbox[j]), adjust_point(cbox[i])))
			return true;
	}

	return false;
}

//Return true if this object is colliding with point collide_p
bool Collidable::check_collision(const Point2f &collide_p) {
	return pointInPolygonWinding(collide_p);
}

// isLeft(): tests if a point is Left|On|Right of an infinite line.
//    Input:  three points P0, P1, and P2
//    Return: >0 for P2 left of the line through P0 and P1
//            =0 for P2  on the line
//            <0 for P2  right of the line
//    See: Algorithm 1 "Area of Triangles and Polygons"
inline int Collidable::isLeft(const Point2f &P0, const Point2f &P1, const Point2f &P2)
{
	return ((P1.x - P0.x) * (P2.y - P0.y) - (P2.x - P0.x) * (P1.y - P0.y));
}

// wn_PnPoly(): winding number test for a point in a polygon
//      Input:   P = a point,
//               V[] = vertex points of a polygon V[n+1] with V[n]=V[0]
//      Return:  wn = the winding number (=0 only when P is outside)
int Collidable::pointInPolygonWinding(const Point2f &P)
{
	int    wn = 0;    // the  winding number counter

	// loop through all edges of the polygon
	for (unsigned int i = 0; i < cbox.size(); i++) {   // edge from V[i] to  V[i+1]
		Point2f pi(adjust_point(cbox[i]));
		Point2f pj(adjust_point(cbox[i + 1]));

		if (pi.y <= P.y) {          // start y <= P.y
			if (pj.y  > P.y)      // an upward crossing
			if (isLeft(pi, pj, P) > 0)  // P left of  edge
				++wn;            // have  a valid up intersect
		}
		else {                        // start y > P.y (no test needed)
			if (pj.y <= P.y)     // a downward crossing
			if (isLeft(pi, pj, P) < 0)  // P right of  edge
				--wn;            // have  a valid down intersect
		}
	}
	return wn;
}

//is point inside the polygon
bool Collidable::pointInPolygon(const Point2f &point) {
	int i, j, nvert = cbox.size();
	bool c = false;

	for (i = 0, j = nvert - 1; i < nvert; j = i++) {
		Point2f pi(adjust_point(cbox[i]));
		Point2f pj(adjust_point(cbox[j]));

		if (((pi.y) > point.y != (pj.y > point.y)) &&
			(point.x <= (pj.x - pi.x) * (point.y - pi.y) / (pj.y - pi.y) + pi.x)
			)
			c = !c;
	}

	return c;
}