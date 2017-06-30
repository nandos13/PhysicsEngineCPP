#pragma once
#include "PhysicsObject.h"

#include <list>

class Rigidbody : public PhysicsObject
{
	friend class Physics;
	friend class Spring;

protected:

	glm::vec2 m_velocity;
	float m_angle;
	float m_angularVelocity;
	float m_momentInertia;
	float m_mass;
	float m_restitution;
	float m_drag;
	float m_angularDrag;

	bool m_isKinematic;
	bool m_isAwake;
	unsigned short m_sleepFrameCount;
	void CheckSleepState();

	void ResolveCollision(Rigidbody* other, glm::vec2 contactPoint, glm::vec2* direction = NULL);

	// Local unit-vectors to store rotation
	glm::vec2 m_localX;
	glm::vec2 m_localY;
	void UpdateLocalAxes();

public:
	Rigidbody(const glm::vec2 pos, const glm::vec2 vel, const float mass);
	~Rigidbody();

	virtual void Update(const float deltaTime);

	const glm::vec2 GetVelocity() const;
	const float GetAngularVelocity() const;
	const float GetMomentOfInertia() const;
	const float GetMass() const;
	const float GetRestitution() const;
	bool GetKinematicState() const;

	void SetAngle(const float a);
	void SetMass(const float m);
	void SetRestitution(const float r);
	void SetDrag(const float d);
	void SetAngularDrag(const float d);
	void SetKinematicState(const bool state);
	void WakeUp();

	const glm::vec2 ToWorld(const glm::vec2 localPoint) const;
	const glm::vec2 GetLocalXVector() const;
	const glm::vec2 GetLocalYVector() const;
	virtual void GetBoundingPoints(glm::vec2& p1, glm::vec2& p2, glm::vec2& p3, glm::vec2& p4, float* localRotation = nullptr) const = 0;
	virtual const std::list<glm::vec2> GetAxes() const = 0;
	virtual const glm::vec2 GetExtents(const glm::vec2 axis, std::list<glm::vec2>* extentPoints = nullptr) const = 0;
	virtual const glm::vec2 GetClosestPointOnAxis(const glm::vec2 axis, const glm::vec2 distPoint) const = 0;

	void ApplyForce(const glm::vec2 force, const glm::vec2 position = glm::vec2(0));
};

