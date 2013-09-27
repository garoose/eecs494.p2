#pragma once

#include <zenilib.h>
#include "Game_Object.h"
#include "Map.h"

using namespace Zeni;

enum jump_state { up, down };

class Tire {
	float size;
	float theta;
	float distance;
	Point2f position;

public:
	Tire::Tire(const float &size_, const float &theta_, const float &distance_)
		: size(size_), theta(theta_), distance(distance_), position(0.0f, 0.0f)
	{
	}

	void Tire::attach(const Point2f &center, const float &forward) {
		position = Point2f(center.x + distance * cos(forward - theta), center.y + distance * -sin(forward - theta));
		position -= Point2f(size * 0.5f, size * 0.5f);
	}

	void Tire::render() const {
		Video &vr = get_Video();

		static float spin = 0.0f;
		spin = spin ? 0.0f : 1.0f;

		Vertex2f_Texture p0(position, Point2f(spin, spin));
		Vertex2f_Texture p1(Point2f(position.x, position.y + size), Point2f(spin, 1.0f - spin));
		Vertex2f_Texture p2(Point2f(position.x + size, position.y + size), Point2f(1.0f - spin, 1.0f - spin));
		Vertex2f_Texture p3(Point2f(position.x + size, position.y), Point2f(1.0f - spin, spin));
		Material material("tire");

		Quadrilateral<Vertex2f_Texture> quad(p0, p1, p2, p3);
		quad.fax_Material(&material);

		vr.render(quad);
	}

	const Point2f &get_position() { return position; }
	const float &get_size() { return size; }
	float bottom() { return size + position.y; }
};

class Buggy : public Game_Object {
private:
	float tire_size;
	Tire left_tire;
	Tire right_tire;

public:
	Buggy(const Point2f &position_,
		const Vector2f &size_,
		const float &theta_,
		const float &speed_,
		const float &min_speed_,
		const float &max_speed_,
		const float &acceleration_)
		: Game_Object(position_, size_, theta_, speed_, min_speed_, max_speed_, acceleration_),
		left_tire(64.0f, (3 * Global::pi/ 4), (get_radius() - 60.0f)),
		right_tire(64.0f, (Global::pi / 4), (get_radius() - 60.0f))
	{
	}

	bool collide(Tile t) {
		Point2f p = get_position();
		if (t.collide(p))
			return true;
	}

	void attach_wheels() {
		Point2f center(get_position().x + (get_size().x * 0.5f),
			get_position().y + (get_size().y * 0.5f));

		left_tire.attach(center, get_theta());
		right_tire.attach(center, get_theta());
	}

	bool can_move(const Vector2f &delta_) {
		attach_wheels();

		if (left_tire.bottom() + delta_.y > 500.0f || right_tire.bottom() + delta_.y > 500.0f)
			return false;

		return Game_Object::can_move(delta_);
	}

	void step(const float &time_step) {
		if (m_jump.can_jump) {
			accelerate((m_controls.right - m_controls.left) * time_step * get_acceleration());
			move_forward(time_step * get_speed());
		}

		//fall
		if (left_tire.bottom() < 500.0f && right_tire.bottom() < 500.0f)
			move_down(time_step * 120.0f);
		else {
			m_jump.can_jump = true;
			m_jump.in_jump = false;

			if (left_tire.bottom() < 500.0f)
				turn_left(time_step * 2);
			else if (right_tire.bottom() < 500.0f)
				turn_left(-time_step * 2);
		}

		//jump
		if (m_jump.can_jump && m_controls.up) {
			m_jump.jump(time_step * get_speed());
			m_controls.up = false;
		}

		if (m_jump.in_jump) {
			if (m_jump.state == up) {
				float deltaj = time_step * m_jump.speed;
				m_jump.height += deltaj;
				move_down(-deltaj);
			}

			move_forward(m_jump.move);

			if (m_jump.height >= m_jump.max_height)
				m_jump.state = down;
		}

		attach_wheels();
	}
	void render() const {
		Game_Object::render("buggy");

		left_tire.render();
		right_tire.render();
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
		Jump() : can_jump(false), in_jump(false), speed(700.0f), height(0.0f), max_height(100.0f) {}

		bool can_jump;
		bool in_jump;
		float speed;
		float height;
		float max_height;
		float move;
		jump_state state;

	public:
		void jump(const float &move_) {
			height = 0;
			move = move_;
			in_jump = true;
			state = up;
			can_jump = false;
		}
	} m_jump;

};

