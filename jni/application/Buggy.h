#pragma once

#include <zenilib.h>
#include <vector>

#include "Game_Object.h"
#include "Score.h"

using namespace Zeni;

class Buggy;
class Map;

static float gravity = 140.0f;

enum jump_state { up, down };

class Tire : public Collidable {
	float size;
	float theta;
	float attach_theta;
	float attach_distance;
	Point2f position;
	Buggy *m_buggy;
	Score *m_score;

public:
	Tire::Tire(const float &size_, const float &theta_, const float &distance_, Buggy *buggy_, Score *score_)
		: size(size_), attach_theta(theta_), attach_distance(distance_), position(0.0f, 0.0f),
		Collidable(std::vector<Point2f> { Point2f(0.0f, 0.0f), Point2f(size_, 0.0f), Point2f(size_, size_), Point2f(0.0f, size_) }),
		theta(0.0f),
		m_buggy(buggy_),
		m_score(score_)
		//Collidable(Vector2f(size, size))
	{
	}

	bool Tire::check_collision(const Vector2f &delta, Collidable *c);

	void Tire::collide(Collidable *c) override;

	void Tire::collide_with_rock(Mars_Rock_Tile *r) override;

	void Tire::attach(const Point2f &center, const float &forward);

	void Tire::render() const;

	void Tire::render_collisions(Collidable *c) {
		Collidable::render(c);
	}

	const Point2f &get_position() const override { return position; }
	const float &get_theta() const override { return theta; }
	const float &get_size() const { return size; }
	Point2f bottom() const { return Point2f(position.x, size + position.y); }
};

class Buggy : public Game_Object {
private:
	float tire_size;
	Tire left_tire;
	Tire right_tire;
	Score *m_score;

public:
	Buggy::Buggy(const Point2f &position_,
		const Vector2f &size_,
		const float &theta_,
		const float &speed_,
		const float &min_speed_,
		const float &max_speed_,
		const float &acceleration_,
		Score *score_)
		: Game_Object(position_, size_,
		std::vector<Point2f> { Point2f(30.0f, 25.0f), Point2f(size_.x - 100.0f, 25.0f),
			Point2f(size_.x - 10.0f, size_.y - 10.0f), Point2f(30.0f, size_.y - 10.0f) },
		theta_, speed_, min_speed_, max_speed_, acceleration_),
		left_tire(64.0f, (3 * Global::pi / 4), (get_radius() - 60.0f), this, m_score),
		right_tire(64.0f, (Global::pi / 4), (get_radius() - 60.0f), this, m_score),
		m_score(score_)
	{
	}

	bool Buggy::check_collision(const Vector2f &delta, Collidable *c);

	void Buggy::collide(Collidable *c) override;
	void Buggy::explode();
	void Buggy::collide_with_rock(Mars_Rock_Tile *r) override;

	void Buggy::collect();

	bool Buggy::can_move(const Vector2f &delta_, Map *m);

	void Buggy::step(const float &time_step, Map *m);

	void Buggy::render() const;

	void Buggy::render_collisions(Map *m_);

	void Buggy::on_key(const SDL_KeyboardEvent &event);

private:
	struct Controls {
		Controls() : left(false), right(false), up(false), down(false) {}

		bool left;
		bool right;
		bool up;
		bool down;
	} m_controls; //end struct Controls

	struct Jump {
		Jump() : can_jump(false), in_jump(false), speed(700.0f), height(0.0f), max_height(120.0f) {}

		bool can_jump;
		bool in_jump;
		float speed;
		float height;
		float max_height;
		jump_state state;

	public:
		void jump() {
			height = 0;
			in_jump = true;
			state = up;
			can_jump = false;
		}
	} m_jump; //end struct Jump

	void Buggy::attach_wheels() {
		Point2f center(Game_Object::get_position().x + (get_size().x * 0.5f),
			Game_Object::get_position().y + (get_size().y * 0.5f));

		left_tire.attach(center, Game_Object::get_theta());
		right_tire.attach(center, Game_Object::get_theta());
	}

};

