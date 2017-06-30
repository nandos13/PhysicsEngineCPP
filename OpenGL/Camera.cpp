#include <glm/glm/ext.hpp>
#include "Camera.h"

void Camera::update(GLFWwindow* window, float dt)
{
	if (glfwGetKey(window, GLFW_KEY_W)) radius = radius - 8.0f * dt;
	if (glfwGetKey(window, GLFW_KEY_S)) radius = radius + 8.0f * dt;
}

mat4 Camera::getView()
{
	return glm::lookAt(vec3(0, 0, 10*radius), vec3(0, 0, 0), vec3(0, 1, 0));
}

mat4 Camera::getProjection(unsigned int windowWidth, unsigned int windowHeight)
{
	glm::mat4 orthomatrix;
	float left = -(float)(windowWidth) / 1000.0f;
	float right = (float)(windowWidth) / 1000.0f;
	float top = (float)(windowHeight) / 1000.0f;
	float bottom = -(float)(windowHeight) / 1000.0f;
	static float Znear = 0.01f;
	static float Zfar = 1000; 


	orthomatrix[0].x = 2.0f / (right - left);
	orthomatrix[0].y = 0;
	orthomatrix[0].z = 0;
	orthomatrix[0].w = -(right + left) / (right - left);

	orthomatrix[1].x = 0;
	orthomatrix[1].y = 2.0f / (top - bottom);
	orthomatrix[1].z = 0;
	orthomatrix[1].w = -(top + bottom) / (top - bottom);

	orthomatrix[2].x = 0;
	orthomatrix[2].y = 0;
	orthomatrix[2].z = -2.0f / (Zfar - Znear);
	orthomatrix[2].w = (Znear + Zfar) / (Znear - Zfar);

	orthomatrix[3].x = 0;
	orthomatrix[3].y = 0;
	orthomatrix[3].z = -(Zfar + Znear) / (Zfar - Znear);
	orthomatrix[3].w = 1;

	return orthomatrix;
}

float Camera::getDistance()
{
	return 10.0f * radius;
}
