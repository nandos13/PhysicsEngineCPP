#include "stdafx.h"
#include "Spring.h"

#include <glm\glm\glm.hpp>
#include <aie\Gizmos.h>


void Spring::GetWorldPoints(glm::vec2 & p1Out, glm::vec2 & p2Out) const
{
	p1Out = m_body1->ToWorld(m_contact1);
	p2Out = m_body2->ToWorld(m_contact2);
}

Spring::Spring(Rigidbody& body1, Rigidbody& body2, const glm::vec2 contactPoint1, const glm::vec2 contactPoint2,
	float restLength, float restoringForce, float dampening)
	: PhysicsObject(glm::vec2(0)), m_body1(&body1), m_body2(&body2),
		m_contact1(contactPoint1), m_contact2(contactPoint2), m_restLength(restLength), 
		m_restoringForce(restoringForce), m_dampening(dampening) {
	m_type = JOINT;
}

Spring::~Spring()
{
}

void Spring::Update(const float deltaTime)
{
	glm::vec2 p1, p2;
	GetWorldPoints(p1, p2);
	glm::vec2 dist = p2 - p1;

	float len = sqrtf(dist.x * dist.x + dist.y * dist.y);

	// Apply dampening
	glm::vec2 dv = m_body2->m_velocity - m_body1->m_velocity;
	glm::vec2 force = dist * m_restoringForce * (m_restLength - len) - m_dampening * dv;

	m_body1->ApplyForce(-force * deltaTime, p1 - m_body1->m_position);
	m_body2->ApplyForce(force * deltaTime, p2 - m_body2->m_position);
}

void Spring::Draw() const
{
	glm::vec2 p1, p2;
	GetWorldPoints(p1, p2);
	Gizmos::add2DLine(p1, p2, glm::vec4(1));
}
