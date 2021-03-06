#include "stdafx.h"
#include "Rigidbody.h"

#include <glm\glm\glm.hpp>
#include "UtilityFunctions.h"
#include "Time.h"


const float Rigidbody::m_sleepVelocityQualifier = 1.0f;
const float Rigidbody::m_sleepAngularVelocityQualifier = 5.0f;

/**
 * Checks if the Rigidbody has been barely moving for several consecutive frames,
 * and puts the object to sleep if so. 
 */
void Rigidbody::CheckSleepState()
{
	if (m_inactiveTime >= 1.5f)
		m_isAwake = false;
}

bool Rigidbody::QualifiedForSleep() const
{
	float velVectorLength = glm::length(m_velocity);
	float angularVelAbsolute = fabsf(m_angularVelocity);

	return (	velVectorLength < m_sleepVelocityQualifier
			&&	angularVelAbsolute < m_sleepAngularVelocityQualifier
			&&	angularVelAbsolute <= fabsf(m_angularVelocityPrevious)
			);
}

/**
 * Resolves a collision between two Rigidbody objects. If no direction is specified,
 * an average point is found between each object's centers when deciding which direction
 * to move the objects.
 */
void Rigidbody::ResolveCollision(Rigidbody * other, glm::vec2 contactPoint, glm::vec2 * direction)
{
	// Awake check. Check if one object is awake and the other is not.
	if (m_isAwake != other->m_isAwake)
	{
		// Check if one of the objects is moving too fast to be sleep qualified
		if (!QualifiedForSleep() || !other->QualifiedForSleep())
		{
			m_isAwake = true;
			m_inactiveTime = 0;

			other->m_isAwake = true;
			other->m_inactiveTime = 0;
		}
	}

	// Find the vector between center points, or use the provided direction of force
	glm::vec2 unitDisp = direction ? *direction : glm::normalize(other->m_position - m_position);

	// Get the normal to unitDisp
	glm::vec2 unitNormal(unitDisp.y, -unitDisp.x);

	// Calculate the total velocity of the contact points (linear & rotational)
	float r1 = glm::dot(contactPoint - m_position, -unitNormal);
	float r2 = glm::dot(contactPoint - other->m_position, unitNormal);
	float v1 = glm::dot(m_velocity, unitDisp) - r1 * m_angularVelocity;
	float v2 = glm::dot(other->m_velocity, unitDisp) + r2 * other->m_angularVelocity;

	// Check if the objects are moving closer
	if (v1 > v2)
	{
		// Calculate the effective mass at the contact point for each object
		float mass1 = 1.0f / (1.0f / m_mass + (r1 * r1) / m_momentInertia);
		float mass2 = 1.0f / (1.0f / other->m_mass + (r2 * r2) / other->m_momentInertia);

		glm::vec2 force = (1.0f + m_restitution) * mass1 * mass2 / (mass1 + mass2) * (v1 - v2) * unitDisp;

		// Apply equal and opposite forces
		ApplyForce(-force, contactPoint - m_position);
		other->ApplyForce(force, contactPoint - other->m_position);
	}
}

/* Calculate & store local axes */
void Rigidbody::UpdateLocalAxes()
{
	float cs = cosf(glm::radians(m_angle));
	float sn = -sinf(glm::radians(m_angle));
	m_localX = glm::vec2(cs, sn);
	m_localY = glm::vec2(-sn, cs);
}

Rigidbody::Rigidbody(const glm::vec2 pos, const glm::vec2 vel, const float mass)
	: PhysicsObject(pos), m_velocity(vel), m_mass(mass), 
		m_angle(0.0f), m_angularVelocity(0.0f), m_momentInertia(0.0f), m_restitution(1.0f), 
		m_drag(0.0f), m_angularDrag(0),
		m_angularVelocityPrevious(0.0f),
		m_isKinematic(false), m_isAwake(true), m_inactiveTime(0.0f),
		m_localX(glm::vec2(1, 0)), m_localY(glm::vec2(0, 1)) {
}

Rigidbody::~Rigidbody()
{
}

void Rigidbody::Update(const float deltaTime)
{
	if (!m_isKinematic && !Time::IsPaused())
	{
		if (m_isAwake)
		{
			m_angularVelocityPrevious = m_angularVelocity;

			// Update position & angle via velocity values
			m_angle += glm::degrees(m_angularVelocity) * deltaTime;
			m_position += m_velocity * deltaTime;

			// Apply gravity
			m_velocity += GetGravity() * deltaTime;

			// Drag
			float velMagnitude = glm::length(m_velocity);
			if (velMagnitude > 0)
			{
				// Apply linear drag
				if (m_drag > 0)
				{
					float difference = (1 - deltaTime * m_drag);
					if (difference < 0)
						m_velocity = glm::vec2(0);
					else
						m_velocity = m_velocity * difference;
				}

				// Apply angular drag
				if (m_angularDrag > 0)
				{
					float difference = (1 - deltaTime * m_angularDrag);
					if (difference < 0)
						m_angularVelocity = 0;
					else
						m_angularVelocity = m_angularVelocity * difference;
				}
			}

			// Check for small velocity
			if (QualifiedForSleep())
			{
				m_inactiveTime += deltaTime;
				CheckSleepState();
			}
			else
				m_inactiveTime = 0.0f;
		}
		else
		{
			// The object is asleep. Nullify velocities
			m_angularVelocity = 0;
			m_velocity = glm::vec2(0);
		}
	}

	UpdateLocalAxes();
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

bool Rigidbody::GetKinematicState() const
{
	return m_isKinematic;
}

void Rigidbody::SetAngle(const float a)
{
	m_angle = a;
	UpdateLocalAxes();
}

void Rigidbody::SetMass(const float m)
{
	m_mass = m;
}

void Rigidbody::SetRestitution(const float r)
{
	m_restitution = JakePerry::Clampf(r, 0, 1);
}

void Rigidbody::SetDrag(const float d)
{
	m_drag = fabsf(d);
}

void Rigidbody::SetAngularDrag(const float d)
{
	m_angularDrag = fabsf(d);
}

void Rigidbody::SetKinematicState(const bool state)
{
	m_isKinematic = state;
}

void Rigidbody::WakeUp()
{
	m_isAwake = true;
}

/* Returns world coordinates of the rigidbody's localPoint */
const glm::vec2 Rigidbody::ToWorld(const glm::vec2 localPoint) const
{
	glm::vec2 x = localPoint.x * m_localX;
	glm::vec2 y = localPoint.y * m_localY;

	return x + y + m_position;
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
	if (!m_isKinematic)
	{
		m_velocity += force / m_mass;
		m_angularVelocity -= (force.y * position.x - force.x * position.y) / (m_momentInertia);
	}
}
