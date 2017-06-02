#include "stdafx.h"
#include "Circle.h"

#include <glm\glm\glm.hpp>
#include <aie\Gizmos.h>


Circle::Circle(const glm::vec2 pos, const glm::vec2 vel, const float mass, const float radius)
	: Rigidbody(pos, vel, mass), m_radius(radius) {
	m_type = CIRCLE;
	m_momentInertia = 0.5f * m_mass * m_radius * m_radius;
}

Circle::~Circle()
{
}

void Circle::Draw() const
{
	Gizmos::add2DCircle(m_position, m_radius, 16, glm::vec4(1, 0, 1, 0.8f));
	Gizmos::add2DCircle(m_position + m_localX * m_radius * 0.8f, m_radius * 0.1f, 6, glm::vec4(1, 1, 1, 0.5f));
}

const float Circle::GetRadius() const
{
	return m_radius;
}
