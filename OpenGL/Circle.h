#pragma once
#include "Rigidbody.h"
class Circle : public Rigidbody
{
	friend class Physics;

protected:

	float m_radius;

public:
	Circle(const glm::vec2 pos, const glm::vec2 vel, const float mass, const float radius);
	~Circle();

	virtual void Draw() const;

	virtual bool ContainsPoint(glm::vec2 point) const;

	const float GetRadius() const;

	virtual void GetBoundingPoints(glm::vec2& p1, glm::vec2& p2, glm::vec2& p3, glm::vec2& p4, float* localRotation = nullptr) const;
	virtual const std::list<glm::vec2> GetAxes() const;
	virtual const glm::vec2 GetExtents(const glm::vec2 axis, std::list<glm::vec2>* extentPoints) const;
	virtual const glm::vec2 GetClosestPointOnAxis(const glm::vec2 axis, const glm::vec2 distPoint) const;

};

