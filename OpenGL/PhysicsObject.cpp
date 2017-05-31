#include "stdafx.h"
#include "PhysicsObject.h"

#include "Physics.h"

// Static variable declaration
glm::vec2 PhysicsObject::m_gravity;


PhysicsObject::PhysicsObject(const glm::vec2 pos)
	: m_position(pos), m_type(NULLTYPE) {
}

PhysicsObject::~PhysicsObject()
{
}

void PhysicsObject::CheckCollisions(PhysicsObject * other)
{
	if (this == nullptr || other == nullptr)	return;
	if (m_type == NULLTYPE || other->m_type == NULLTYPE)	return;

	Physics::HandleCollisionBase(this, other);
}

const PHYS_TYPE PhysicsObject::GetType() const
{
	return m_type;
}

const glm::vec2 PhysicsObject::GetPosition() const
{
	return m_position;
}

/* Set the value of gravity in the negative-y direction. */
void PhysicsObject::SetGravity(float value)
{
	m_gravity.y = 0 - value;
}
