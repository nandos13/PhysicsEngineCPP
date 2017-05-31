#include "PhysicsApplication.h"

#include "Plane.h"
#include "Rigidbody.h"
#include "Circle.h"

using namespace glm;

void PhysicsApplication::StartScene()
{
	PhysicsObject::SetGravity(9.81f);


	m_physObjects.clear();

	// Add objects
	m_physObjects.push_back( new Circle(glm::vec2(0), glm::vec2(1, 0), 1.0f, 0.5f) );
	m_physObjects.push_back( new Plane(glm::vec2(0, -3.2f), glm::vec2(0, 1)) );
}

bool PhysicsApplication::startup()
{
	glfwInit();

	window = glfwCreateWindow(1280, 720,
		"Computer Graphics",
		nullptr, nullptr);
	if (window == nullptr) {
		glfwTerminate();
		return false;
	}
	glfwMakeContextCurrent(window);

	if (ogl_LoadFunctions() == ogl_LOAD_FAILED) {
		glfwDestroyWindow(window);
		glfwTerminate();
		return false;
	}

	auto major = ogl_GetMajorVersion();
	auto minor = ogl_GetMinorVersion();
	printf("GL: %i.%i\n", major, minor);


	glClearColor(0.0f, 0.0f, 0.25f, 1);
	glEnable(GL_DEPTH_TEST); // enables the depth buffer

	Gizmos::create(65335U, 65535U, 65535U, 65535U);

	camera.radius = 1;

	StartScene();

	return true;
}

void PhysicsApplication::shutdown()
{
	Gizmos::destroy();

	glfwDestroyWindow(window);


	glfwTerminate();
}

bool PhysicsApplication::update()
{
	camera.update(window);

	float dt = 1.0f / 60.0f;

	if (glfwGetKey(window, GLFW_KEY_P))
	{
		StartScene();
		return true;
	}
	
	// Iterate through each physics object & call update
	for (auto& iter = m_physObjects.begin(); iter != m_physObjects.end(); iter++)
	{
		PhysicsObject* obj = (*iter);
		obj->Update(dt);

		// Check for collisions against all objects further up the list
		for (auto iter2 = iter; iter2 != m_physObjects.end(); iter2++)
		{
			if (iter != iter2)
			{
				PhysicsObject* obj2 = (*iter2);
				obj2->CheckCollisions(obj);
			}
		}
	}
	
	return (glfwWindowShouldClose(window) == false && glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS);
}

void PhysicsApplication::draw()
{
	mat4 view = camera.getView();
	mat4 projection = camera.getProjection();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	Gizmos::clear();

	Gizmos::addTransform(glm::mat4(1));
	vec4 orange(1, 0.7f, 0.2f, 1.0f);
	vec4 red(1, 0, 0, 1);
	vec4 white(1);
	vec4 black(0, 0, 0, 1);
	for (int i = 0; i < 21; ++i) 
	{
		Gizmos::add2DLine(vec2(-10 + i, 10), vec2(-10 + i, -10), i == 10 ? orange : white);
		Gizmos::add2DLine(vec2(10, -10 + i), vec2(-10, -10 + i), i == 10 ? orange : white);
	}

	// Iterate through & draw each physics object
	for (auto& iter = m_physObjects.begin(); iter != m_physObjects.end(); iter++)
	{
		(*iter)->Draw();
	}

	Gizmos::draw2D(projection * view);
	
	glfwSwapBuffers(window);
	glfwPollEvents();

	day++;
}
