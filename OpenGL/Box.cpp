#include "stdafx.h"
#include "Box.h"

#include <vector>
#include <algorithm>

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

	Gizmos::add2DTri(p1, p2, p3, glm::vec4(1, 1, 0, 0.8f));
	Gizmos::add2DTri(p3, p4, p1, glm::vec4(1, 1, 0, 0.8f));
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

/* Returns a list with two axes: The Box's localX & localY axis. */
const std::list<glm::vec2> Box::GetAxes() const
{
	std::list<glm::vec2> axisList;
	axisList.push_back(m_localX);
	axisList.push_back(m_localY);

	return axisList;
}

const glm::vec2 Box::GetExtents(const glm::vec2 axis, std::list<glm::vec2>* extentPoints) const
{
	float min = std::numeric_limits<float>::max();
	float max = -(std::numeric_limits<float>::max());

	glm::vec2 p1, p2, p3, p4;
	GetBoundingPoints(p1, p2, p3, p4);

	std::list<glm::vec2> cornerList;
	cornerList.push_back(p1);
	cornerList.push_back(p2);
	cornerList.push_back(p3);
	cornerList.push_back(p4);

	glm::vec2 minPoint = glm::vec2(0);
	glm::vec2 maxPoint = glm::vec2(0);

	for (auto& iter = cornerList.cbegin(); iter != cornerList.cend(); iter++)
	{
		float project = glm::dot(*iter, axis);
		if (project < min)
		{
			min = project;
			minPoint = *iter;
		}
		if (project > max)
		{
			max = project;
			maxPoint = *iter;
		}
	}

	if (extentPoints != nullptr)
	{
		extentPoints->clear();
		extentPoints->push_back(minPoint);
		extentPoints->push_back(maxPoint);
	}

	return glm::vec2(min, max);
}

/* Finds a point on a given axis that is closest to the specified point. */
const glm::vec2 Box::GetClosestPointOnAxis(const glm::vec2 axis, const glm::vec2 distPoint) const
{
	// Get all corners
	glm::vec2 p1, p2, p3, p4;
	GetBoundingPoints(p1, p2, p3, p4);

	std::list<glm::vec2> cornerList;
	cornerList.push_back(p1);
	cornerList.push_back(p2);
	cornerList.push_back(p3);
	cornerList.push_back(p4);

	// Loop through each corner & find the two corners closest to distPoint when projected onto the axis
	float distPointProjection = glm::dot(distPoint, axis);
	float minDistance = std::numeric_limits<float>::max();

	std::vector<std::pair<float, glm::vec2>> differenceValues;

	for (auto& iter = cornerList.cbegin(); iter != cornerList.cend(); iter++)
	{
		// Find the difference in projection between this corner and the distPoint
		glm::vec2 corner = *iter;
		float projection = glm::dot(corner, axis);
		float difference = std::fabsf(projection - distPointProjection);

		// Push back into the list of values
		differenceValues.push_back(std::make_pair(difference, corner));
	}

	// Sort the value list by distance value
	std::sort(differenceValues.begin(), differenceValues.end(), 
		[](const std::pair<float, glm::vec2> lhs, const std::pair<float, glm::vec2> rhs) { return lhs.first < rhs.first; }
	);

	// Get the first two corners in the sorted list
	glm::vec2 cornerOne(0), cornerTwo(0);
	float cornerOneProj, cornerTwoProj;

	auto iter = differenceValues.cbegin();
	if (iter != differenceValues.cend())
	{
		cornerOne = iter->second;
		cornerOneProj = iter->first;
	}
	iter = std::next(iter, 1);
	if (iter != differenceValues.cend())
	{
		cornerTwo = iter->second;
		cornerTwoProj = iter->first;
	}

	// TODO: DELETE WHEN DONE
	Gizmos::add2DCircle(cornerOne, 0.15f, 4, glm::vec4(1, 0.2f, 0, 1));
	Gizmos::add2DCircle(cornerTwo, 0.15f, 4, glm::vec4(1, 0.7f, 0, 1));

	// Get line from minPoint to maxPoint & find closest point on line to distPoint
	glm::vec2 minToMax = glm::normalize(cornerTwo - cornerOne);

	Gizmos::add2DLine(glm::vec2(-0.08f), minToMax + glm::vec2(-0.08f), glm::vec4(0,0,1,1));

	float pointProjected = glm::dot(distPoint, minToMax);
	glm::vec2 closestPoint = minToMax * pointProjected;

	if (cornerOneProj > cornerTwoProj)
		closestPoint += cornerTwo;
	else
		closestPoint += cornerOne;

	return closestPoint;
}
