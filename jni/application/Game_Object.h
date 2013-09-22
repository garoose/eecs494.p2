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
		m_speed(speed_)
	{
	}

	// If you might delete base class pointers, you need a virtual destructor.
	virtual ~Game_Object() {}

	virtual void render() const = 0; // pure virtual function call

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
	}

	void move_forward(const float &move_) {
		// Performance consideration: calculate and store a forward Vector2f when turning and avoid cosine & sine here?
		m_position.x += move_ * cos(m_theta);
		m_position.y += move_ * -sin(m_theta);
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
	float m_theta;

	float m_speed;
};