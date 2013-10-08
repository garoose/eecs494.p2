#include <zenilib.h>
#include <vector>
#include <string>

#include "Buggy.h"
#include "Game_Object.h"
#include "Score.h"
#include "Map.h"
#include "Tile.h"

extern std::string test;

using std::vector;
using namespace Zeni;

bool Tire::check_collision(const Vector2f &delta, Collidable *c) {
	return Collidable::check_collision(get_position() + delta, get_theta(), c);
}

void Tire::collide(Collidable *C) {
	test = "Tire Collide";
}

void Tire::collide_with_rock(Mars_Rock_Tile *r) {
	m_buggy->collide_with_rock(r);
}

void Tire::attach(const Point2f &center, const float &forward) {
	position = Point2f(center.x + attach_distance * cos(forward - attach_theta),
		center.y + attach_distance * -sin(forward - attach_theta));
	position -= Point2f(size.x * 0.5f, size.y * 0.5f);
}

void Tire::render() const {
	Video &vr = get_Video();

	Vertex2f_Texture p0(position, Point2f(0.0f, 0.0f));
	Vertex2f_Texture p1(Point2f(position.x, position.y + size.y), Point2f(0.0f, 1.0f));
	Vertex2f_Texture p2(Point2f(position.x + size.x, position.y + size.y), Point2f(1.0f, 1.0f));
	Vertex2f_Texture p3(Point2f(position.x + size.x, position.y), Point2f(1.0f, 0.0f));
	Material material("tire");

	Quadrilateral<Vertex2f_Texture> quad(p0, p1, p2, p3);
	quad.fax_Material(&material);

	vr.render(quad);
}

bool Buggy::check_collision(const Vector2f &delta, Collidable *c) {
	return Collidable::check_collision(get_position() + delta, get_theta(), c);
}

void Buggy::collide(Collidable *c) {
	test = "Buggy Collide";
	c->collide_with_buggy(this);
}

void Buggy::explode() {
	m_score->inc(-10);
	play_sound("explode");
	reset_position();
}

void Buggy::collide_with_rock(Mars_Rock_Tile *r) {
	m_score->inc(100);
	r->collect();
}

void Buggy::collect() {
	m_score->inc(100);
}

bool Buggy::can_move(const Vector2f &delta_, Map *m) {
	attach_wheels();

	if (m->check_collision(this, delta_) || m->check_collision(&left_tire, delta_) || m->check_collision(&right_tire, delta_))
		return false;

	return Game_Object::can_move(delta_, m);
}

void Buggy::step(const float &time_step, Map *m) {
	bool leftcdown = m->check_collision(&left_tire, Vector2f(0.0f, time_step * gravity));
	bool rightcdown = m->check_collision(&right_tire, Vector2f(0.0f, time_step * gravity));
	bool leftcright = m->check_collision(&left_tire, Vector2f(time_step * get_speed(), 0.0f));
	bool rightcright = m->check_collision(&right_tire, Vector2f(time_step * get_speed(), 0.0f));

	//only allow speed ajustments when on the ground
	if (m_jump.can_jump) {
		accelerate((m_controls.right - m_controls.left) * time_step * get_acceleration());
	}

	//move buggy ahead at current speed
	move_right(time_step * get_speed(), m);

	//fall
	if (!leftcdown && !rightcdown) {
		if (!move_down(time_step * gravity, m)) {
			m_jump.can_jump = true;
			m_jump.in_jump = false;
		}
		else
			m_jump.can_jump = false;
	}
	else { //one or both tires touching the ground
		m_jump.can_jump = true;
		m_jump.in_jump = false;

		if (!leftcdown)
			turn_left(time_step * 2, m);
		else if (!rightcdown)
			turn_left(-time_step * 2, m);

		//prevent tires from getting stuck
		if (leftcright || rightcright)
			move_down((-gravity + 10.0f) * time_step, m);

	}

	//jump
	if (m_jump.can_jump && m_controls.up) {
		m_jump.jump();
		m_controls.up = false;
	}

	if (m_jump.in_jump) {
		if (m_jump.state == up) {
			float deltaj = time_step * m_jump.speed;
			m_jump.height += deltaj;
			move_down(-deltaj, m);
		}

		if (m_jump.height >= m_jump.max_height)
			m_jump.state = down;
	}

	attach_wheels();
}

void Buggy::render() const {
	Game_Object::render("buggy");

	left_tire.render();
	right_tire.render();
}

void Buggy::render_collisions(Map *m) {
	Collidable::render(nullptr);
	left_tire.render_collisions(nullptr);
	right_tire.render_collisions(nullptr);
}

void Buggy::on_key(const SDL_KeyboardEvent &event) {
	switch (event.keysym.sym) {
	case SDLK_LEFT:
	case SDLK_a:
		m_controls.left = event.type == SDL_KEYDOWN;
		break;

	case SDLK_RIGHT:
	case SDLK_d:
		m_controls.right = event.type == SDL_KEYDOWN;
		break;

	case SDLK_UP:
	case SDLK_w:
		m_controls.up = event.type == SDL_KEYDOWN;
		break;

	case SDLK_DOWN:
	case SDLK_s:
		m_controls.down = event.type == SDL_KEYDOWN;
		break;
	}
}