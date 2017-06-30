#pragma once

#include <glm\glm\common.hpp>

enum PHYS_TYPE
{
	NULLTYPE = 0,
	PLANE = 1,
	CIRCLE = 2,
	BOX = 3,
	JOINT = 4
};

class PhysicsObject
{
	friend class Physics;

protected:

	PHYS_TYPE m_type;
	glm::vec2 m_position;

	bool m_debugMode;

	static const glm::vec2 GetGravity();

public:

	PhysicsObject(const glm::vec2 pos);
	~PhysicsObject();

	virtual void Update(const float deltaTime) = 0;
	virtual void Draw() const = 0;
	virtual void CheckCollisions(PhysicsObject* other);

	virtual bool ContainsPoint(glm::vec2 point) const;

	const PHYS_TYPE GetType() const;
	const glm::vec2 GetPosition() const;
	const bool GetDebugState() const;

	void SetPosition(const glm::vec2 pos);
	void SetDebugState(bool state);
};

