#pragma once
#include "PhysicsObject.h"

class Plane : public PhysicsObject
{
protected:

	glm::vec2 m_normal;

public:
	Plane(const glm::vec2 pos, const glm::vec2 normal);
	~Plane();

	virtual void Update(const float deltaTime);
	virtual void Draw() const;

	const glm::vec2 GetNormal() const;
};

