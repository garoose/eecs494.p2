#pragma once

#include <zenilib.h>

#include "Map.h"

using namespace Zeni;

class Game_Object {
private:
	Point2f m_position; // Upper left corner
	Vector2f m_size; // (width, height)
	Vector2f forward;
	float m_theta;
	float m_speed;
	float m_min_speed;
	float m_max_speed;
	float m_acceleration;
	Point2f reset_pos;

public:
	Game_Object(const Point2f &position_,
		const Vector2f &size_,
		const float &theta_ = 0.0f,
		const float &speed_ = 0.0f,
		const float &min_speed_ = 0.0f,
		const float &max_speed_ = 0.0f,
		const float &acceleration_ = 0.0f)
		: m_position(position_),
		m_size(size_),
		m_theta(theta_),
		m_speed(speed_),
		m_min_speed(min_speed_),
		m_max_speed(max_speed_),
		m_acceleration(acceleration_),
		forward(cos(theta_), -sin(theta_)),
		reset_pos(position_)
	{
	}

	// If you might delete base class pointers, you need a virtual destructor.
	virtual ~Game_Object() {}

	virtual void render() const = 0; // pure virtual function call

	virtual bool can_move(const Vector2f &delta_, Map &m) {
		if ((m_position.x + delta_.x) < 0)
			return false;

		return true;
	}

	virtual void step(const float &time_step, Map &m)
	{
	}

	const Point2f & get_position() const { return m_position; }
	const Vector2f & get_size() const { return m_size; }
	const float & get_theta() const { return m_theta; }

	const float get_radius() const {
		return 0.5f * m_size.magnitude();
	}

	float get_speed() { return m_speed; }
	float get_acceleration() { return m_acceleration; }

	void reset_position() {
		m_position = reset_pos;
	}

	void turn_left(const float &theta_, Map m) {
		float start = m_theta;

		m_theta += theta_;

		while (!can_move(Vector2f(0.0f, 0.0f), m)) {
			m_theta += theta_ < 0 ? 0.0001f : -0.0001f;

			if (m_theta <= start)
				break;
		}

		forward = Vector2f(cos(m_theta), -sin(m_theta));
	}

	void move_forward(const float &move_, Map m) {
		Vector2f delta(move_ * forward.x, 0.0f);
		
		//FIXME: causes freezing
		//while (delta.x && !can_move(delta)) {
		//	delta.x += delta.x < 0 ? 0.0001f : -0.0001f;
		//}
		if (can_move(delta, m))
			m_position.x += delta.i;
	}

	bool move_down(const float &move_, Map m) {
		Vector2f delta(0.0f, move_);

		//while (delta.y && !can_move(delta, m)) {
		//	delta.y += delta.y < 0 ? 0.0001f : -0.0001f;
		//}
		if (can_move(delta, m)) {
			m_position.y += delta.y;
			return true;
		}
		return false;
	}

	void set_speed(const float &speed_) {
		if (speed_ <= m_max_speed && speed_ >= m_min_speed)
			m_speed = speed_;
	}

	void accelerate(const float &delta_) {
		set_speed(m_speed + delta_);
	}

protected:
	void render(const String &texture, const Color &filter = Color()) const {
		// Use a helper defined in Zeni/EZ2D.h
		render_image(
			texture, // which texture to use
			m_position, // upper-left corner
			m_position + m_size, // lower-right corner
			m_theta, // rotation in radians
			1.0f, // scaling factor
			m_position + 0.5f * m_size, // point to rotate & scale about
			false, // whether or not to horizontally flip the texture
			filter); // what Color to "paint" the texture
	}

};