#pragma once
#include "Rigidbody.h"
class Box : public Rigidbody
{
protected:

	glm::vec2 m_size;

public:
	Box(const glm::vec2 pos, const glm::vec2 vel, const float mass, const glm::vec2 size);
	~Box();

	virtual void Draw() const;

};

