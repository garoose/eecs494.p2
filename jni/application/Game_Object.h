#pragma once

#include <zenilib.h>

using namespace Zeni;

class Game_Object {
public:
	Game_Object(const Point2f &position_,
		const Vector2f &size_,
		const float &theta_ = 0.0f,
		const float &speed_ = 0.0f)
		: m_position(position_),
		m_size(size_),
		m_theta(theta_),
		m_speed(speed_),
		forward(cos(theta_), -sin(theta_))
	{
	}

	// If you might delete base class pointers, you need a virtual destructor.
	virtual ~Game_Object() {}

	virtual void render() const = 0; // pure virtual function call
	virtual bool can_move(const Vector2f &delta_) {
		//fix dese magic numbers yo
		if ((m_position.x + delta_.i) < 0 || (m_position.x + m_size.i + delta_.i) > 1024.0f)
			return false;
		if ( (m_position.y + m_size.j + delta_.j) > 768.0f)
			return false;

		return true;
	}

public:
	const Point2f & get_position() const { return m_position; }
	const Vector2f & get_size() const { return m_size; }
	const float & get_theta() const { return m_theta; }

	const float get_radius() const {
		return 0.5f * m_size.magnitude();
	}

	float get_speed() { return m_speed; }

	void turn_left(const float &theta_) {
		m_theta += theta_;

		forward = Vector2f(cos(m_theta), -sin(m_theta));
	}

	void move_forward(const float &move_) {
		Vector2f delta(move_ * forward.x, 0.0f);
		
		while (delta.x > 0 && !can_move(delta)) {
			delta.x -= 0.0001f;
			if (delta.x < 0)
				delta.x = 0;
		}

		delta.y = move_ * forward.y;

		while (delta.y > 0 && !can_move(delta)) {
			delta.y -= 0.0001f;
			if (delta.y < 0)
				delta.y = 0;
		}

		if (can_move(delta)) {
			m_position.x += delta.i;
			m_position.y += delta.j;
		}
	}

	void move_down(const float &move_) {
		Vector2f delta(0.0f, move_);

		while (delta.y > 0 && !can_move(delta)) {
			delta.y -= 0.0001f;
			if (delta.y < 0)
				delta.y = 0;
		}

		m_position.y += delta.y;
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

private:
	Point2f m_position; // Upper left corner
	Vector2f m_size; // (width, height)
	Vector2f forward;
	float m_theta;

	float m_speed;
};