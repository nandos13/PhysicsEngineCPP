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

/**
 * Returns the 4 corners of a bounding box in world-space. If a localRotation is specified,
 * the bounding box will be rotated around the Circle's center position. Else, no 
 * rotation is applied to the box.
 */
void Circle::GetBoundingPoints(glm::vec2 & p1, glm::vec2 & p2, glm::vec2 & p3, glm::vec2 & p4, float* localRotation) const
{
	glm::vec2 axisX(1, 0);
	glm::vec2 axisY(0, 1);

	// Use local rotation if one has been specified
	if (localRotation)
	{
		float cs = cosf(glm::radians(*localRotation));
		float sn = sinf(glm::radians(*localRotation));

		axisX = glm::vec2(cs, sn);
		axisY = glm::vec2(-sn, cs);
	}

	p1 = m_position - axisX * m_radius + axisY * m_radius;
	p2 = m_position + axisX * m_radius + axisY * m_radius;
	p3 = m_position + axisX * m_radius - axisY * m_radius;
	p4 = m_position - axisX * m_radius - axisY * m_radius;
}

/* Circles have no axes to check. Returns an empty list. */
const std::list<glm::vec2> Circle::GetAxes() const
{
	return std::list<glm::vec2>();
}

const glm::vec2 Circle::GetExtents(const glm::vec2 axis, std::list<glm::vec2>* extentPoints) const
{
	glm::vec2 radiusRotated = m_radius * axis;
	float centre = glm::dot(m_position, axis);

	glm::vec2 minPoint = m_position - radiusRotated;
	glm::vec2 maxPoint = m_position + radiusRotated;

	if (extentPoints != nullptr)
	{
		extentPoints->clear();
		extentPoints->push_back(minPoint);
		extentPoints->push_back(maxPoint);
	}

	return glm::vec2(centre - m_radius, centre + m_radius);
}

const glm::vec2 Circle::GetClosestPointOnAxis(const glm::vec2 axis, const glm::vec2 distPoint) const
{
	std::list<glm::vec2> extentPoints;
	GetExtents(axis, &extentPoints);

	glm::vec2 extentPoint1 = *(extentPoints.cbegin());
	glm::vec2 extentPoint2 = *(std::next(extentPoints.cbegin(), 1));

	return (glm::distance(distPoint, extentPoint1) < glm::distance(distPoint, extentPoint2)) ? extentPoint1 : extentPoint2;
}
