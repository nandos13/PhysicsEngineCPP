#pragma once
#include "PhysicsObject.h"

#include "Rigidbody.h"

class Spring : public PhysicsObject
{
protected:

	Rigidbody* m_body1;
	Rigidbody* m_body2;

	glm::vec2 m_contact1;
	glm::vec2 m_contact2;
	float m_restLength;
	float m_restoringForce;
	float m_dampening;

	void GetWorldPoints(glm::vec2& p1Out, glm::vec2& p2Out) const;

public:
	Spring(Rigidbody& body1, Rigidbody& body2, const glm::vec2 contactPoint1 = glm::vec2(0), const glm::vec2 contactPoint2 = glm::vec2(0), 
		float restLength = 1, float restoringForce = 1, float dampening = 1);
	~Spring();

	virtual void Update(const float deltaTime);
	virtual void Draw() const;
};

