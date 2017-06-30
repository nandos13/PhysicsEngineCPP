#include "stdafx.h"
#include "PhysicsObject.h"

#include "Physics.h"


const glm::vec2 PhysicsObject::GetGravity()
{
	return Physics::GetGravity();
}

PhysicsObject::PhysicsObject(const glm::vec2 pos)
	: m_position(pos), m_type(NULLTYPE), m_debugMode(false) {
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

bool PhysicsObject::ContainsPoint(glm::vec2 point) const
{
	return false;
}

const PHYS_TYPE PhysicsObject::GetType() const
{
	return m_type;
}

const glm::vec2 PhysicsObject::GetPosition() const
{
	return m_position;
}

const bool PhysicsObject::GetDebugState() const
{
	return m_debugMode;
}

void PhysicsObject::SetPosition(const glm::vec2 pos)
{
	m_position = pos;
}

void PhysicsObject::SetDebugState(bool state)
{
	m_debugMode = state;
}
