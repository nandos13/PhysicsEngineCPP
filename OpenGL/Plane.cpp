#include "stdafx.h"
#include "Plane.h"

#include <glm\glm\glm.hpp>
#include <aie\Gizmos.h>


Plane::Plane(const glm::vec2 pos, const glm::vec2 normal)
	: PhysicsObject(pos), m_normal(glm::normalize(normal)) {
	m_type = PLANE;
}

Plane::~Plane()
{
}

void Plane::Update(const float deltaTime)
{
	/* This function needs to be overrided to allow instantiation of a Plane, 
	however nothing needs to be done in Update. */
}

void Plane::Draw() const
{
	glm::vec2 parallel = glm::vec2(m_normal.y, -m_normal.x);
	glm::vec2 offset = parallel * glm::vec2(100);

	// Get two points on the plane to draw between
	glm::vec2 p1 = m_position - offset;
	glm::vec2 p2 = m_position + offset;

	Gizmos::add2DLine(p1, p2, glm::vec4(1, 0.1f, 0.1f, 1));
}

const glm::vec2 Plane::GetNormal() const
{
	return m_normal;
}
