#pragma once

#include <glm\glm\common.hpp>

enum PHYS_TYPE
{
	NULLTYPE = 0,
	PLANE = 1,
	CIRCLE = 2,
	BOX = 3
};

class PhysicsObject
{
protected:

	static glm::vec2 m_gravity;

	PHYS_TYPE m_type;
	glm::vec2 m_position;

public:

	PhysicsObject(const glm::vec2 pos);
	~PhysicsObject();

	virtual void Update(const float deltaTime) = 0;
	virtual void Draw() const = 0;
	virtual void CheckCollisions(PhysicsObject* other);

	const PHYS_TYPE GetType() const;
	const glm::vec2 GetPosition() const;

	static void SetGravity(float value);
};

