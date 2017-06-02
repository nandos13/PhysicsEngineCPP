#pragma once
#include "PhysicsObject.h"

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

	void ApplyForce(const glm::vec2 force, const glm::vec2 position = glm::vec2(0));
};

