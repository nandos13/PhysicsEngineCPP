#include "stdafx.h"
#include "Rigidbody.h"

#include "UtilityFunctions.h"


/* Calculate & store local axes */
void Rigidbody::GetLocalAxes()
{
	float cs = cosf(m_angle);
	float sn = sinf(m_angle);
	m_localX = glm::vec2(cs, sn);
	m_localY = glm::vec2(-sn, cs);
}

Rigidbody::Rigidbody(const glm::vec2 pos, const glm::vec2 vel, const float mass)
	: PhysicsObject(pos), m_velocity(vel), m_mass(mass), 
		m_angle(0.0f), m_angularVelocity(0.0f), m_momentInertia(0.0f), m_restitution(1.0f),
		m_localX(glm::vec2(0)), m_localY(glm::vec2(0)) {
}

Rigidbody::~Rigidbody()
{
}

void Rigidbody::Update(const float deltaTime)
{
	// Apply gravity
	m_velocity += GetGravity() * deltaTime;

	// Update position & angle via velocity values
	m_angle += m_angularVelocity * deltaTime;
	m_position += m_velocity * deltaTime;

	GetLocalAxes();
}

const glm::vec2 Rigidbody::GetVelocity() const
{
	return m_velocity;
}

const float Rigidbody::GetAngularVelocity() const
{
	return m_angularVelocity;
}

const float Rigidbody::GetMomentOfInertia() const
{
	return m_momentInertia;
}

const float Rigidbody::GetMass() const
{
	return m_mass;
}

const float Rigidbody::GetRestitution() const
{
	return m_restitution;
}

void Rigidbody::SetAngle(const float a)
{
	m_angle = a;
}

void Rigidbody::SetMass(const float m)
{
	m_mass = m;
}

void Rigidbody::SetRestitution(const float r)
{
	m_restitution = JakePerry::Clampf(r, 0, 1);
}

const glm::vec2 Rigidbody::GetLocalXVector() const
{
	return m_localX;
}

const glm::vec2 Rigidbody::GetLocalYVector() const
{
	return m_localY;
}

void Rigidbody::ApplyForce(const glm::vec2 force, const glm::vec2 position)
{
	m_velocity += force / m_mass;
	m_angularVelocity += (force.y * position.x - force.x * position.y) / (m_momentInertia);
}
