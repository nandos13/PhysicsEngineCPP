#include "stdafx.h"
#include "Rigidbody.h"


Rigidbody::Rigidbody(const glm::vec2 pos, const glm::vec2 vel, const float mass)
	: PhysicsObject(pos), m_velocity(vel), m_mass(mass), m_restitution(1.0f) {
}

Rigidbody::~Rigidbody()
{
}

void Rigidbody::Update(const float deltaTime)
{
	// Apply gravity
	ApplyForce(m_gravity);

	m_position += m_velocity * deltaTime;
}

const glm::vec2 Rigidbody::GetVelocity() const
{
	return m_velocity;
}

const float Rigidbody::GetMass() const
{
	return m_mass;
}

const float Rigidbody::GetRestitution() const
{
	return m_restitution;
}

void Rigidbody::SetMass(const float m)
{
	m_mass = m;
}

void Rigidbody::SetRestitution(const float r)
{
	m_restitution = r;
}

void Rigidbody::ApplyForce(const glm::vec2 force)
{
	m_velocity += force / m_mass;
}
