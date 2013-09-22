#pragma once

#include <zenilib.h>
#include "Game_Object.h"

using namespace Zeni;


void render_tire(Point2f position, float tire_size) {
	Video &vr = get_Video();

	Vertex2f_Texture p0(position, Point2f(0.0f, 0.0f));
	Vertex2f_Texture p1(Point2f(position.x, position.y + tire_size), Point2f(0.0f, 1.0f));
	Vertex2f_Texture p2(Point2f(position.x + tire_size, position.y + tire_size), Point2f(1.0f, 1.0f));
	Vertex2f_Texture p3(Point2f(position.x + tire_size, position.y), Point2f(1.0f, 0.0f));
	Material material("tire");

	Quadrilateral<Vertex2f_Texture> quad(p0, p1, p2, p3);
	quad.fax_Material(&material);

	vr.render(quad);
}

class Buggy : public Game_Object {
public:
	Buggy(const Point2f &position_,
		const Vector2f &size_,
		const float &theta_,
		const float &speed_)
		: Game_Object(position_, size_, theta_, speed_)
	{
	}

	void render() const {
		Game_Object::render("buggy");

		Video &vr = get_Video();

		float tire_size = 64.0f;

		float x = get_position().x;
		float y = get_position().y;
		Vector2f size = get_size();
		float radius = get_radius();
		float theta = get_theta();
		float ltheta = get_theta() - 3 * Global::pi / 4;
		float rtheta = get_theta() - Global::pi / 4;
		Point2f center (x + (size.i * 0.5f) - (tire_size * 0.5f), y + (size.j * 0.5f) - (tire_size * 0.5f));
		Point2f left_tire_pos (center.x + 85.0f * cos(ltheta), center.y + 85.0f * -sin(ltheta));
		Point2f right_tire_pos(center.x + 85.0f * cos(rtheta), center.y + 85.0f * -sin(rtheta));

		render_tire(left_tire_pos, tire_size);
		render_tire(right_tire_pos, tire_size);
	}
private:

};

