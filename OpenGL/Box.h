#pragma once
#include "Rigidbody.h"
class Box : public Rigidbody
{
	friend class Physics;

protected:

	glm::vec2 m_size;

public:
	Box(const glm::vec2 pos, const glm::vec2 vel, const float mass, const glm::vec2 size);
	~Box();

	virtual void Draw() const;

	const float GetWidth() const;
	const float GetHeight() const;
	const glm::vec2 GetSize() const;

	virtual void GetBoundingPoints(glm::vec2& p1, glm::vec2& p2, glm::vec2& p3, glm::vec2& p4, float* localRotation = nullptr) const;
	virtual const std::list<glm::vec2> GetAxes() const;
	virtual const glm::vec2 GetExtents(const glm::vec2 axis, std::list<glm::vec2>* extentPoints) const;
	virtual const glm::vec2 GetClosestPointOnAxis(const glm::vec2 axis, const glm::vec2 distPoint) const;

};

