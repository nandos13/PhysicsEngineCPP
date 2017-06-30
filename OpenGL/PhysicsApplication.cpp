#include "PhysicsApplication.h"

#include "Physics.h"
#include "Time.h"
#include "Plane.h"
#include "Rigidbody.h"
#include "Circle.h"
#include "Box.h"
#include "Spring.h"

using namespace glm;

unsigned int PhysicsApplication::GetWindowWidth() const
{
	int w = 0, h = 0;
	glfwGetWindowSize(window, &w, &h);
	return w;
}

unsigned int PhysicsApplication::GetWindowHeight() const
{
	int w = 0, h = 0;
	glfwGetWindowSize(window, &w, &h);
	return h;
}

void PhysicsApplication::StartScene()
{
	//m_selectedObject = 0;
	Physics::SetGravity(9.81f);

	// Loop through & delete all objects
	for (auto& iter = m_physObjects.begin(); iter != m_physObjects.cend(); iter++)
	{
		PhysicsObject* obj = *iter;
		delete obj;
	}

	m_physObjects.clear();

	// Add objects
	Circle* c1 = new Circle(glm::vec2(0), glm::vec2(1, 5), 1.0f, 0.5f);
	Circle* c2 = new Circle(glm::vec2(-3, 1), glm::vec2(0), 1.0f, 0.5f);
	Circle* c3 = new Circle(glm::vec2(-5, -1), glm::vec2(2, 0), 20.0f, 0.9f);
	Circle* c4 = new Circle(glm::vec2(-4, 0), glm::vec2(0, 3), 0.5f, 0.9f);
	Circle* c5 = new Circle(glm::vec2(0, -5), glm::vec2(0, -3), 1.0f, 0.9f);
	Circle* c6 = new Circle(glm::vec2(4, -5), glm::vec2(0, -3), 5.0f, 0.9f);
	//c2->SetRestitution(0.2f);
	c1->SetRestitution(0.9f);
	c3->SetRestitution(0.25f);
	c2->SetDebugState(true);
	//m_physObjects.push_back( c1 );
	//m_physObjects.push_back( c2 );
	//m_physObjects.push_back( c3 );
	//m_physObjects.push_back( c4 );
	//m_physObjects.push_back( c5 );
	//m_physObjects.push_back( c6 );

	Box* b1 = new Box(glm::vec2(-4, 4), glm::vec2(0), 5.0f, glm::vec2(2.2f, 1.4f));
	Box* b2 = new Box(glm::vec2(2, 0), glm::vec2(0), 10.0f, glm::vec2(2.2f, 1.4f));
	Box* b3 = new Box(glm::vec2(-3, -1), glm::vec2(0), 20.0f, glm::vec2(2.0f, 1.8f));
	b1->SetAngle(10);
	b3->SetAngle(268);
	b1->SetDebugState(true);
	b3->SetDebugState(true);
	b3->SetRestitution(0.4f);
	b1->SetRestitution(0.2f);
	//b1->SetKinematicState(true);
	//b3->SetKinematicState(true);
	m_physObjects.push_back( b1 );
	//m_physObjects.push_back( b2 );
	m_physObjects.push_back( b3 );

	m_physObjects.push_back( new Spring(*b1, *c1, glm::vec2(0), glm::vec2(0), 1, 1, 1) );

	m_physObjects.push_back( new Plane(glm::vec2(0, -3.2f), glm::vec2(0, 1)) );
	//m_physObjects.push_back( new Plane(glm::vec2(0, -8.2f), glm::vec2(0, 1)) );
	m_physObjects.push_back( new Plane(glm::vec2(9.5f, 0), glm::vec2(-5, 1)) );
	m_physObjects.push_back( new Plane(glm::vec2(-9.5f, 0), glm::vec2(5, 1)) );
}

void PhysicsApplication::updatePoolCue()
{
	bool mouseDown = (glfwGetMouseButton(window, 0) == 1);
	double x0, y0;
	glfwGetCursorPos(window, &x0, &y0);
	glm::mat4 view = camera.getView();
	glm::mat4 projection = camera.getProjection(GetWindowWidth(), GetWindowHeight());

	glm::vec3 windowCoordinates = glm::vec3(x0, y0, 0);
	glm::vec4 viewport = glm::vec4(0.0f, 0.0f, 1280, 720);
	glm::vec3 worldCoordinates = glm::unProject(windowCoordinates, view, projection, viewport);

	m_mousePoint = glm::vec2(worldCoordinates.x * camera.getDistance(), worldCoordinates.y * (-camera.getDistance()));

	if (mouseDown != m_isMouseDown)
	{
		if (mouseDown)		// The mouse button has been clicked this frame.
			m_contactPoint = m_mousePoint;
		else		// The mouse button has been released this frame.
		{
			for (auto iter = m_physObjects.begin(); iter != m_physObjects.end(); iter++)
			{
				PhysicsObject* obj = *iter;
				if (obj->ContainsPoint(m_mousePoint))
				{
					// Cast object to Rigidbody
					Rigidbody* rb = (Rigidbody*)obj;
					if (!rb->GetKinematicState())
					{
						rb->WakeUp();	// Ensure the object is awake & able to receive force
						rb->ApplyForce(2.0f*(m_mousePoint - m_contactPoint), m_contactPoint - rb->GetPosition());
					}
				}
			}
		}

		m_isMouseDown = mouseDown;
	}
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

	m_pause = false;
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
	Gizmos::clear();

	float dt = 1.0f / 300.0f;

	camera.update(window, dt);

	// Scene Restart
	if (glfwGetKey(window, GLFW_KEY_P))
		StartScene();

	// Time Pause functionality
	{
		static bool spacePressed = false;
		if (glfwGetKey(window, GLFW_KEY_SPACE))
		{
			if (spacePressed == false)
				m_pause = !m_pause;

			spacePressed = true;
		}
		else
			spacePressed = false;

		if (m_pause)
			dt = 0;

		Time::SetDeltaTime(dt);
		Time::SetPause(m_pause);
	}

	// Pool Cue functionality
	updatePoolCue();

	// Iterate through each physics object & call update
	for (auto& iter = m_physObjects.begin(); iter != m_physObjects.end(); iter++)
	{
		PhysicsObject* obj = (*iter);
		obj->Update(dt);
	}

	for (auto& iter = m_physObjects.begin(); iter != m_physObjects.end(); iter++)
	{
		PhysicsObject* obj = (*iter);

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
	mat4 projection = camera.getProjection(GetWindowWidth(), GetWindowHeight());

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	Gizmos::addTransform(glm::mat4(1));
	vec4 orange(1, 0.7f, 0.2f, 1.0f);
	vec4 red(0.7f, 0, 0, 1);
	vec4 white(0.7f, 0.7f, 0.7f, 1);
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

	// Draw pool cue line
	if (m_isMouseDown)
		Gizmos::add2DLine(m_mousePoint, m_contactPoint, white);

	Gizmos::draw2D(projection * view);
	
	glfwSwapBuffers(window);
	glfwPollEvents();

	day++;
}
