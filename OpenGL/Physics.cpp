#include "stdafx.h"
#include "Physics.h"

#include <glm\glm\glm.hpp>

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

	const glm::vec2 planeNormal = objB->GetNormal();
	const float circleRadius = objA->GetRadius();

	// Get distance from circle to closest point on the plane
	float distance = glm::dot( (objA->GetPosition() - objB->GetPosition()), objB->GetNormal() );

	// Get the velocity in the perpendicular direction
	float vPerp = glm::dot( objA->GetVelocity(), objB->GetNormal());

	// Check if the objects collide this frame
	if (	(distance > 0 && distance < circleRadius && vPerp < 0)
		||	(distance < 0 && distance > -circleRadius && vPerp > 0)	)
	{
		// Get the deflection vector
		const float circleRestitution = objA->GetRestitution();
		glm::vec2 deltaVelocity = -(vPerp * planeNormal) * (1 + circleRestitution);

		// Add the force to the circle
		objA->ApplyForce(deltaVelocity);
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
	// TODO
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