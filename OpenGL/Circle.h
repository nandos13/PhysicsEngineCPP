#pragma once
#include "Rigidbody.h"
class Circle : public Rigidbody
{
protected:

	float m_radius;

public:
	Circle(const glm::vec2 pos, const glm::vec2 vel, const float mass, const float radius);
	~Circle();

	virtual void Draw() const;

	const float GetRadius() const;
};

