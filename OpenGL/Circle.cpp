#include "stdafx.h"
#include "Circle.h"


Circle::Circle(const glm::vec2 pos, const glm::vec2 vel, const float mass, const float radius)
	: Rigidbody(pos, vel, mass), m_radius(radius) {
	m_type = CIRCLE;
}

Circle::~Circle()
{
}

const float Circle::GetRadius() const
{
	return m_radius;
}
