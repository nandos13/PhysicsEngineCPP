#include "stdafx.h"
#include "Box.h"

#include <glm\glm\glm.hpp>
#include <aie\Gizmos.h>


Box::Box(const glm::vec2 pos, const glm::vec2 vel, const float mass, const glm::vec2 size)
	: Rigidbody(pos, vel, mass), m_size(size) {
	m_type = BOX;
}

Box::~Box()
{
}

void Box::Draw() const
{
	// Get coordinates for each corner & draw two triangles
	glm::vec2 p1 = m_position - m_localX * m_size.x / 2.0f - m_localY * m_size.y / 2.0f;
	glm::vec2 p2 = m_position + m_localX * m_size.x / 2.0f - m_localY * m_size.y / 2.0f;
	glm::vec2 p3 = m_position - m_localX * m_size.x / 2.0f + m_localY * m_size.y / 2.0f;
	glm::vec2 p4 = m_position + m_localX * m_size.x / 2.0f + m_localY * m_size.y / 2.0f;

	Gizmos::add2DTri(p1, p2, p4, glm::vec4(1, 1, 0, 1));
	Gizmos::add2DTri(p1, p4, p3, glm::vec4(1, 1, 0, 1));
}
