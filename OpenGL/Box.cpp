#include "stdafx.h"
#include "Box.h"

#include <glm\glm\glm.hpp>
#include <aie\Gizmos.h>


Box::Box(const glm::vec2 pos, const glm::vec2 vel, const float mass, const glm::vec2 size)
	: Rigidbody(pos, vel, mass), m_size(size) {
	m_type = BOX;
	m_momentInertia = 1.0f / 12.0f * m_mass * m_size.x * m_size.y;
}

Box::~Box()
{
}

void Box::Draw() const
{
	// Get coordinates for each corner & draw two triangles
	glm::vec2 p1, p2, p3, p4;
	GetBoundingPoints(p1, p2, p3, p4);

	Gizmos::add2DTri(p1, p2, p3, glm::vec4(1, 1, 0, 1));
	Gizmos::add2DTri(p3, p4, p1, glm::vec4(1, 1, 0, 1));
}

const float Box::GetWidth() const
{
	return m_size.x;
}

const float Box::GetHeight() const
{
	return m_size.y;
}

const glm::vec2 Box::GetSize() const
{
	return m_size;
}

/* Finds the 4 corners of the box in world-space. LocalRotation is always ignored for Box. */
void Box::GetBoundingPoints(glm::vec2 & p1, glm::vec2 & p2, glm::vec2 & p3, glm::vec2 & p4, float* localRotation) const
{
	p1 = m_position - m_localX * m_size.x / 2.0f + m_localY * m_size.y / 2.0f;
	p2 = m_position + m_localX * m_size.x / 2.0f + m_localY * m_size.y / 2.0f;
	p3 = m_position + m_localX * m_size.x / 2.0f - m_localY * m_size.y / 2.0f;
	p4 = m_position - m_localX * m_size.x / 2.0f - m_localY * m_size.y / 2.0f;
}
