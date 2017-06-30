#pragma once
#include <glfw-3.1.2\include\GLFW\glfw3.h>
#include <glm/glm/glm.hpp>
using namespace glm;

class Camera
{
public:
	mat4 getView();
	mat4 getProjection(unsigned int windowWidth, unsigned int windowHeight);

	float getDistance();

	void update(GLFWwindow* window, float dt);

	float radius = 1;
};