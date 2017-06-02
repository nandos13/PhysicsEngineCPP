#pragma once
#include "PhysicsObject.h"

#include <list>

class Rigidbody : public PhysicsObject
{
	friend class Physics;

protected:

	glm::vec2 m_velocity;
	float m_angle;
	float m_angularVelocity;
	float m_momentInertia;
	float m_mass;
	float m_restitution;

	void ResolveCollision(Rigidbody* other, glm::vec2 contactPoint, glm::vec2* direction = NULL);

	// Local unit-vectors to store rotation
	glm::vec2 m_localX;
	glm::vec2 m_localY;
	void GetLocalAxes();

public:
	Rigidbody(const glm::vec2 pos, const glm::vec2 vel, const float mass);
	~Rigidbody();

	virtual void Update(const float deltaTime);

	const glm::vec2 GetVelocity() const;
	const float GetAngularVelocity() const;
	const float GetMomentOfInertia() const;
	const float GetMass() const;
	const float GetRestitution() const;

	void SetAngle(const float a);
	void SetMass(const float m);
	void SetRestitution(const float r);

	const glm::vec2 GetLocalXVector() const;
	const glm::vec2 GetLocalYVector() const;
	virtual void GetBoundingPoints(glm::vec2& p1, glm::vec2& p2, glm::vec2& p3, glm::vec2& p4, float* localRotation = nullptr) const = 0;
	virtual const std::list<glm::vec2> GetAxes() const = 0;
	virtual const glm::vec2 GetExtents(const glm::vec2 axis) const = 0;

	void ApplyForce(const glm::vec2 force, const glm::vec2 position = glm::vec2(0));
};

