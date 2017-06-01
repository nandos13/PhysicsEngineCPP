#pragma once

#include "PhysicsObject.h"
#include "Plane.h"
#include "Circle.h"
#include "Box.h"

class Physics
{
private:

	static void intHandleCollision(Plane* objA, PhysicsObject* objB);
	static void intHandleCollision(Circle* objA, PhysicsObject* objB);
	static void intHandleCollision(Box* objA, PhysicsObject* objB);

public:

	static void HandleCollisionBase(PhysicsObject* objA, PhysicsObject* objB);
	
	static void HandleCollision(Plane* objA, Circle* objB);
	static void HandleCollision(Circle* objA, Plane* objB);

	static void HandleCollision(Plane* objA, Box* objB);
	static void HandleCollision(Box* objA, Plane* objB);

	static void HandleCollision(Box* objA, Circle* objB);
	static void HandleCollision(Circle* objA, Box* objB);

	static void HandleCollision(Circle* objA, Circle* objB);

	static void HandleCollision(Box* objA, Box* objB);

};

