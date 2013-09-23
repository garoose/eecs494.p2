#pragma once

#include <zenilib.h>
#include "Game_Object.h"

using namespace Zeni;

class Buggy : public Game_Object {
public:
	Buggy(const Point2f &position_,
		const Vector2f &size_,
		const float &theta_,
		const float &speed_)
		: Game_Object(position_, size_, theta_, speed_),
		tire_size(64.0f)
	{
	}

	void render() const {
		Game_Object::render("buggy");

		render_tire(left_tire);
		render_tire(right_tire);
	}

	bool can_move(const Vector2f &delta_) {
		if (left_tire.y + tire_size + delta_.y >= 500.0f || right_tire.y + tire_size + delta_.y >= 500.0f)
			return false;

		return Game_Object::can_move(delta_);
	}

	void step(const float &time_step) {
		// without a multiplier, this will rotate a full turn after ~6.28s
		//turn_left((m_controls.up - m_controls.down) * time_step);
		// without the '100.0f', it would move at ~1px/s
		move_forward((m_controls.right - m_controls.left) * time_step * get_speed());

		//fall
		if (left_tire.y + tire_size < 500.0f)
			turn_left(time_step);

		if (right_tire.y + tire_size < 500.0f)
			turn_left(-time_step);

		move_down(time_step * 98.0f);

		//jump
		if (m_jump.can_jump) {
			m_jump.height = 0;
			m_jump.in_jump = true;
			m_jump.can_jump = false;
		}

		if (m_jump.in_jump) {
			float deltaj = time_step * m_jump.speed;
			m_jump.height += deltaj;
			move_down(-deltaj);

			if (m_jump.height >= m_jump.max_height)
				m_jump.in_jump = false;
		}

		Point2f center(get_position().x + (get_size().i * 0.5f) - (tire_size * 0.5f), 
					   get_position().y + (get_size().j * 0.5f) - (tire_size * 0.5f));
		float distance = get_radius() - 50.0f;
		float ltheta = get_theta() - 3 * Global::pi / 4;
		float rtheta = get_theta() - Global::pi / 4;

		left_tire = Point2f(center.x + distance * cos(ltheta), center.y + distance * -sin(ltheta));
		right_tire = Point2f(center.x + distance * cos(rtheta), center.y + distance * -sin(rtheta));
	}

	void Buggy::on_key(const SDL_KeyboardEvent &event) {
		switch (event.keysym.sym) {
		case SDLK_LEFT:
			m_controls.left = event.type == SDL_KEYDOWN;
			break;

		case SDLK_RIGHT:
			m_controls.right = event.type == SDL_KEYDOWN;
			break;

		case SDLK_UP:
			m_controls.up = event.type == SDL_KEYDOWN;
			break;

		case SDLK_DOWN:
			m_controls.down = event.type == SDL_KEYDOWN;
			break;
		}
	}

private:
	struct Controls {
		Controls() : left(false), right(false), up(false), down(false) {}

		bool left;
		bool right;
		bool up;
		bool down;
	} m_controls;

	struct Jump {
		Jump() : can_jump(true), in_jump(false), speed(700.0f), height(0.0f), max_height(100.0f) {}

		bool can_jump;
		bool in_jump;
		float speed;
		float height;
		float max_height;
	} m_jump;

	float tire_size;
	Point2f right_tire;
	Point2f left_tire;

	void render_tire(Point2f position) const {
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

};

