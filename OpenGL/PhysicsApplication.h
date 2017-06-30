#pragma once

#include <conio.h>
#include <gl_core_4_4.h>
#include <glfw-3.1.2\include\GLFW\glfw3.h>
#include <aie/Gizmos.h>
#include <glm/glm/glm.hpp>
#include <glm/glm/ext.hpp>
#include <list>

#include "Application.h"
#include "Camera.h"
#include "PhysicsObject.h"

class PhysicsApplication : public Application
{
private:

	unsigned int GetWindowWidth() const;
	unsigned int GetWindowHeight() const;

	std::list<PhysicsObject*> m_physObjects;
	bool m_pause;

	void StartScene();
	void StartScenePoolTable();

	// Pool cue variables
	glm::vec2 m_contactPoint;
	glm::vec2 m_mousePoint;
	bool m_isMouseDown;
	void updatePoolCue();

public:

	virtual bool startup();
	virtual void shutdown();
	virtual bool update();
	virtual void draw();

	int day = 0;

	Camera camera;

	GLuint shaderID;
};
