#include "stdafx.h"
#include "Physics.h"

#include <glm\glm\glm.hpp>

// Static variable declaration
glm::vec2 Physics::m_gravity;

/**
 * Internal use only:
 * Auto-detects the type of the second PhysicsObject parameter & handles
 * collisions between this type and a Plane object.
 */
void Physics::intHandleCollision(Plane * objA, PhysicsObject * objB)
{
	PHYS_TYPE objBType = objB->GetType();

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
	PHYS_TYPE objBType = objB->GetType();

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
	PHYS_TYPE objBType = objB->GetType();

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

	PHYS_TYPE objAType = objA->GetType();

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
		glm::vec2 deltaVelocity = -(vPerp * planeNormal) * (1 + circleRestitution);

		// Add the force to the circle
		objA->ApplyForce(deltaVelocity);

		// Offset position to prevent ball from sinking into the plane
		objA->SetPosition(objA->m_position - penetration * planeNormal);
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
	float width = objA->GetWidth();
	float height = objA->GetHeight();
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
}

/* Handles collision between two circles */
void Physics::HandleCollision(Circle * objA, Circle * objB)
{
	// TODO
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
