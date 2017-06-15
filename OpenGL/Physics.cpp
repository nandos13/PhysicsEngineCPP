#include "stdafx.h"
#include "Physics.h"

#include <glm\glm\glm.hpp>
#include <list>
#include <aie\Gizmos.h>		// TODO: Remove this when debugging is done

#include "UtilityFunctions.h"

using namespace JakePerry;

// Static variable declaration
glm::vec2 Physics::m_gravity;

/* Uses the Separating Axis Theorem to determine if two Convex-Hull Rigidbody objects are colliding */
const bool Physics::IsCollidingSAT(Rigidbody * objA, Rigidbody * objB, glm::vec2& minTranslationVec, glm::vec2& contactPoint)
{
	if (objA == nullptr || objB == nullptr)	return false;

	// Get axes to check for each rigidbody
	std::list<glm::vec2> axes;
	axes = AppendList<glm::vec2>(objA->GetAxes(), objB->GetAxes());

	// We also need to check the axis between both center-masses
	glm::vec2 AtoB = glm::normalize(objB->m_position - objA->m_position);
	if (std::find(axes.cbegin(), axes.cend(), AtoB) == axes.cend())
		axes.push_back(AtoB);

	// Should we draw debug info to the screen?
	/* NOTE: If true, all axes will be checked even if one is found to have no overlap.
	 * This is more expensive than immediately returning false, but is needed to draw gizmos for each axis.
	 */
	bool drawDebugGizmos = (objA->m_debugMode && objB->m_debugMode);
	bool drawDebugContactPoint = (objA->m_debugMode || objB->m_debugMode);

	// Set up a few variables needed to find the minimum translation vector & contact point.
	float minimumTranslationLength = std::numeric_limits<float>::max();	// Used to find minimum, starts at max so everything found is smaller
	glm::vec2 minimumTranslationVector = glm::vec2(0);
	glm::vec2 ExtentAClosestToB = glm::vec2(0);	// Stores the extent point of objA closest to objB when the min translation vector is found
	glm::vec2 ExtentBClosestToA = glm::vec2(0);

	// Check each axis
	bool colliding = true;
	for (auto& iter = axes.cbegin(); iter != axes.cend(); iter++)
	{
		glm::vec2 axis = *iter;

		// Get extents of each Rigidbody on this axis
		std::list<glm::vec2> pointsA;
		std::list<glm::vec2> pointsB;
		glm::vec2 extentsA = objA->GetExtents(axis, &pointsA);
		glm::vec2 extentsB = objB->GetExtents(axis, &pointsB);

		float aMin = extentsA.x;
		float aMax = extentsA.y;
		float bMin = extentsB.x;
		float bMax = extentsB.y;

		if (drawDebugGizmos)
		{
			// Draw axis & axis-overlap for both objects
			Gizmos::add2DLine(axis * -100.0f, axis * 100.0f, glm::vec4(1));
			glm::vec2 axisNormal = glm::vec2(-axis.y, axis.x);
			glm::vec2 offset = (iter == axes.cbegin()) ? axisNormal * 0.12f : axisNormal * -0.12f;
			glm::vec2 amin = axis * extentsA.x + offset;
			glm::vec2 amax = axis * extentsA.y + offset;
			glm::vec2 bmin = axis * extentsB.x - offset;
			glm::vec2 bmax = axis * extentsB.y - offset;
			Gizmos::add2DLine(amin, amax, glm::vec4(0, 1, 0, 1));
			Gizmos::add2DLine(bmin, bmax, glm::vec4(1, 0, 1, 1));

			// 
			if (pointsA.size() == 2)
			{
				Gizmos::add2DLine(*(pointsA.cbegin()), amin, glm::vec4(0.5f, 0.9f, 1.0f, 0.7f));
				Gizmos::add2DLine(*(std::next(pointsA.cbegin(), 1)), amax, glm::vec4(0.5f, 0.9f, 1.0f, 0.7f));
			}
			if (pointsB.size() == 2)
			{
				Gizmos::add2DLine(*(pointsB.cbegin()), bmin, glm::vec4(0.5f, 0.9f, 1.0f, 0.7f));
				Gizmos::add2DLine(*(std::next(pointsB.cbegin(), 1)), bmax, glm::vec4(0.5f, 0.9f, 1.0f, 0.7f));
			}
		}

		// Individually check each overlap condition & find minimum overlap axis
		bool overlapping = false;

		if (	intAxisCheckSAT(aMin, bMin, bMax, overlapping, minimumTranslationLength)
			||	intAxisCheckSAT(aMax, bMin, bMax, overlapping, minimumTranslationLength)
			||	intAxisCheckSAT(bMin, aMin, aMax, overlapping, minimumTranslationLength)
			||	intAxisCheckSAT(bMax, aMin, aMax, overlapping, minimumTranslationLength))
		{
			minimumTranslationVector = axis;
		}

		// Check if the extents overlap
		if (!overlapping)
		{
			// The Rigidbody objects are not overlapping on this axis. Return false
			if (!drawDebugGizmos)
				return false;	// No collision & no gizmos need to be drawn. Exit here

			colliding = false;	// Set bool to false, but keep checking & drawing axes (only for visual feedback)
		}
	}

	if (colliding)
	{
		// Find which extent point of objA on the axis is closest to objB
		ExtentAClosestToB = objA->GetClosestPointOnAxis(minimumTranslationVector, objB->m_position);

		// Find which extent point of objB on the axis is closest to objA
		ExtentBClosestToA = objB->GetClosestPointOnAxis(minimumTranslationVector, objA->m_position);

		Gizmos::add2DCircle(ExtentAClosestToB, 0.05f, 4, glm::vec4(0,1,0,1));
		Gizmos::add2DCircle(ExtentBClosestToA, 0.05f, 4, glm::vec4(0,1,0,1));

		// Set reference parameter for minimum translation vector
		minTranslationVec = minimumTranslationVector * minimumTranslationLength;

		// Find contact point
		contactPoint = (ExtentAClosestToB + ExtentBClosestToA) * 0.5f;

		if (drawDebugContactPoint)
		{
			// TODO: DELETE THIS WHEN DONE
			Gizmos::add2DCircle(contactPoint, 0.1f, 4, glm::vec4(1));
			Gizmos::add2DLine(glm::vec2(0.1f), minimumTranslationVector + glm::vec2(0.1f), glm::vec4(1,0,0,1));
		}

		return true;
	}

	return false;
}

/**
 * Internal use only:
 * For use within Separating-Axis-Theorem function. Checks for overlap, and if overlapping, 
 * records the minimum overlap amount.
 * Returns true if the axis appears to be the minimum translation vector.
 */
const bool Physics::intAxisCheckSAT(const float firstMin, const float secondMin, const float secondMax, bool & overlap, float& overlapAmount)
{
	if (IsBetween(firstMin, secondMin, secondMax))
	{
		overlap = true;

		float amount = std::fminf(std::abs(firstMin - secondMin), std::abs(firstMin - secondMax));

		if (amount < overlapAmount)
		{
			overlapAmount = amount;
			return true;
		}
	}
	return false;
}

/**
 * Internal use only:
 * Auto-detects the type of the second PhysicsObject parameter & handles
 * collisions between this type and a Plane object.
 */
void Physics::intHandleCollision(Plane * objA, PhysicsObject * objB)
{
	PHYS_TYPE objBType = objB->m_type;

	switch (objBType)
	{
	case NULLTYPE: return;

	case CIRCLE:	HandleCollision(objA, (Circle*)objB);
		break;
	case BOX:		HandleCollision(objA, (Box*)objB);
		break;
	}
}

/**
* Internal use only:
* Auto-detects the type of the second PhysicsObject parameter & handles
* collisions between this type and a Circle object.
*/
void Physics::intHandleCollision(Circle * objA, PhysicsObject * objB)
{
	PHYS_TYPE objBType = objB->m_type;

	switch (objBType)
	{
	case NULLTYPE: return;

	case PLANE:		HandleCollision(objA, (Plane*)objB);
		break;
	case CIRCLE:	HandleCollision(objA, (Circle*)objB);
		break;
	case BOX:		HandleCollision(objA, (Box*)objB);
		break;
	}
}

/**
* Internal use only:
* Auto-detects the type of the second PhysicsObject parameter & handles
* collisions between this type and a Box object.
*/
void Physics::intHandleCollision(Box * objA, PhysicsObject * objB)
{
	PHYS_TYPE objBType = objB->m_type;

	switch (objBType)
	{
	case NULLTYPE: return;

	case PLANE:		HandleCollision(objA, (Plane*)objB);
		break;
	case CIRCLE:	HandleCollision(objA, (Circle*)objB);
		break;
	case BOX:		HandleCollision(objA, (Box*)objB);
		break;
	}
}

/* Automatically casts specified physics-objects to their intended types & handles collisions between the two */
void Physics::HandleCollisionBase(PhysicsObject * objA, PhysicsObject * objB)
{
	if (objA == nullptr || objB == nullptr)	return;

	PHYS_TYPE objAType = objA->m_type;

	switch (objAType)
	{
	case NULLTYPE: return;

	case PLANE:		intHandleCollision((Plane*)objA, objB);
		break;
	case CIRCLE:	intHandleCollision((Circle*)objA, objB);
		break;
	case BOX:		intHandleCollision((Box*)objA, objB);
		break;
	}
}

/* Handles collision between a circle and a plane */
void Physics::HandleCollision(Plane * objA, Circle * objB)
{
	HandleCollision(objB, objA);	// Call circle/plane method
}

/* Handles collision between a circle and a plane */
void Physics::HandleCollision(Circle * objA, Plane * objB)
{
	if (objA == nullptr || objB == nullptr)	return;

	const glm::vec2 planeNormal = objB->m_normal;
	const float circleRadius = objA->m_radius;

	float penetration = 0;

	// Get distance from circle to closest point on the plane
	float distance = glm::dot( (objA->m_position - objB->m_position), planeNormal );

	// Get the velocity in the perpendicular direction
	float vPerp = glm::dot( objA->m_velocity, planeNormal);

	// Check if the objects collide this frame
	if (	(distance > 0 && distance < circleRadius && vPerp < 0)
		||	(distance < 0 && distance > -circleRadius && vPerp > 0)	)
	{
		if (distance > 0 && distance < circleRadius)
			penetration = distance - circleRadius;
		else
			penetration = distance + circleRadius;

		// Get the deflection vector
		const float circleRestitution = objA->m_restitution;
		glm::vec2 deltaVelocity = -objA->m_mass * (vPerp * planeNormal) * (1 + circleRestitution);

		// Add the force to the circle
		objA->ApplyForce(deltaVelocity);

		// Offset position to prevent ball from sinking into the plane
		objA->m_position -= penetration * planeNormal;
	}
}

/* Handles collision between a box and a plane */
void Physics::HandleCollision(Plane * objA, Box * objB)
{
	HandleCollision(objB, objA);	// Call box/plane method
}

/* Handles collision between a box and a plane */
void Physics::HandleCollision(Box * objA, Plane * objB)
{
	float width = objA->m_size.x;
	float height = objA->m_size.y;
	glm::vec2 position = objA->m_position;
	glm::vec2 localX = objA->m_localX;
	glm::vec2 localY = objA->m_localY;

	int numberOfContacts = 0;
	glm::vec2 contact(0, 0);
	float contactVelocity = 0;
	float radius = 0.5f * std::fminf(width, height);

	// Find which side the center-of-mass is on
	float comFromPlane = glm::dot(position - objB->m_position, objB->m_normal);
	float penetration = 0;

	// Check each corner of the box to see if it has hit the plane
	for (float x = -width / 2; x < width; x += width)
	{
		for (float y = -height / 2; y < height; y += height)
		{
			// Get corner's position in world-space
			glm::vec2 p = position + x * localX + y * localY;
			float distFromPlane = glm::dot(p - objB->m_position, objB->m_normal);

			// Get total velocity at the point
			float velocityIntoPlane = glm::dot(objA->m_velocity + objA->m_angularVelocity * (-y*localX+x*localY), objB->m_normal);

			/* If this corner is on the opposite side from the center-of-mass & also moving further into the plane,
			 * we need to resolve the collision
			 */
			if (	(distFromPlane > 0 && comFromPlane < 0 && velocityIntoPlane > 0)
				||	(distFromPlane < 0 && comFromPlane > 0 && velocityIntoPlane < 0)	)
			{
				numberOfContacts++;
				contact += p;
				contactVelocity += velocityIntoPlane;

				if (	(comFromPlane >= 0 && penetration > distFromPlane)
					||	(comFromPlane < 0 && penetration < distFromPlane))
				{
					penetration = distFromPlane;
				}
			}
		}
	}

	// Did any points hit?
	if (numberOfContacts > 0)
	{
		// Get average collision velocity into the plane. This covers linear and rotational velocity of all corners involved
		float collisionVelocity = contactVelocity / (float)numberOfContacts;

		// Get the acceleration required to stop or reverse the average velocity, depending on restitution
		glm::vec2 acceleration = -objB->m_normal * ((1.0f + objA->m_restitution) * collisionVelocity);

		// Get average position at which to apply the force
		glm::vec2 localContact = (contact / (float)numberOfContacts) - position;

		// This is the perpendicular distance we apply the force at, relavite to the center-of-mass. So torque = F * r
		float r = glm::dot(localContact, glm::vec2(objB->m_normal.y, -objB->m_normal.x));

		/* Calculate the "effective mass". This is a combination of moment of inertia & mass, 
		 * and tells us how much the contact point velocity will change with the force we're applying.
		 */
		float mass0 = 1.0f / (1.0f / objA->m_mass + (r * r) / objA->m_momentInertia);

		// Apply the force
		objA->ApplyForce(acceleration * mass0, localContact);

		objA->m_position -= objB->m_normal * penetration;
	}
}

/* Handles collision between a circle and a box */
void Physics::HandleCollision(Box * objA, Circle * objB)
{
	HandleCollision(objB, objA);	// Call circle/box method
}

/* Handles collision between a circle and a box */
void Physics::HandleCollision(Circle * objA, Box * objB)
{
	// TODO
	glm::vec2 minTranslationVec = glm::vec2(0);
	glm::vec2 contactPoint = glm::vec2(0);
	if (IsCollidingSAT(objA, objB, minTranslationVec, contactPoint))
		objB->ResolveCollision(objA, contactPoint, &(minTranslationVec));
}

/* Handles collision between two circles */
void Physics::HandleCollision(Circle * objA, Circle * objB)
{
	float sumOfRadii = objA->m_radius + objB->m_radius;
	float distBetweenCircles = glm::distance(objA->m_position, objB->m_position);

	// If the distance between the two circles is less than the sum of their radii, they are touching
	if (distBetweenCircles < sumOfRadii)
	{
		float penetration = sumOfRadii - distBetweenCircles;
		glm::vec2 circleAtoB = glm::normalize(objB->m_position - objA->m_position);

		objA->m_position -= circleAtoB * penetration;
		objB->m_position += circleAtoB * penetration;

		glm::vec2 averagePosition = 0.5f * (objA->m_position + objB->m_position);
		objA->ResolveCollision(objB, averagePosition);
	}
}

/* Handles collision between two boxes */
void Physics::HandleCollision(Box * objA, Box * objB)
{
	// TODO
}

const glm::vec2 Physics::GetGravity()
{
	return m_gravity;
}

/* Set the value of gravity in the negative-y direction. */
void Physics::SetGravity(const float value)
{
	m_gravity.y = 0 - value;
}
