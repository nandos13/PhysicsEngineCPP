#include "PhysicsApplication.h"

#include "Physics.h"
#include "Plane.h"
#include "Rigidbody.h"
#include "Circle.h"
#include "Box.h"

using namespace glm;

void PhysicsApplication::StartScene()
{
	//m_selectedObject = 0;

	Physics::SetGravity(9.81f);

	m_physObjects.clear();

	// Add objects
	Circle* c1 = new Circle(glm::vec2(0), glm::vec2(3, 15), 1.0f, 0.5f);
	Circle* c2 = new Circle(glm::vec2(3, -1), glm::vec2(-1, 3), 1.0f, 0.5f);
	Circle* c3 = new Circle(glm::vec2(-5, -1), glm::vec2(3, 0), 20.0f, 0.9f);
	Circle* c4 = new Circle(glm::vec2(-4, -5), glm::vec2(0, -3), 0.5f, 0.9f);
	Circle* c5 = new Circle(glm::vec2(0, -5), glm::vec2(0, -3), 1.0f, 0.9f);
	Circle* c6 = new Circle(glm::vec2(4, -5), glm::vec2(0, -3), 5.0f, 0.9f);
	c2->SetRestitution(0.2f);
	c3->SetRestitution(0.25f);
	c3->SetDebugState(true);
	//m_physObjects.push_back( c1 );
	//m_physObjects.push_back( c2 );
	m_physObjects.push_back( c3 );
	//m_physObjects.push_back( c4 );
	//m_physObjects.push_back( c5 );
	//m_physObjects.push_back( c6 );

	Box* b1 = new Box(glm::vec2(-4, 0), glm::vec2(0), 5.0f, glm::vec2(2.2f, 1.4f));
	b1->SetAngle(45);
	b1->SetRestitution(0.1f);
	b1->SetKinematicState(true);
	m_physObjects.push_back( b1 );

	m_physObjects.push_back( new Plane(glm::vec2(0, -3.2f), glm::vec2(0, 1)) );
	m_physObjects.push_back( new Plane(glm::vec2(0, -8.2f), glm::vec2(0, 1)) );
	m_physObjects.push_back( new Plane(glm::vec2(9.5f, 0), glm::vec2(-5, 1)) );
	m_physObjects.push_back( new Plane(glm::vec2(-9.5f, 0), glm::vec2(5, 1)) );
}

//void PhysicsApplication::DrawHUD()
//{
//	unsigned int numberOfPhysObjects = (unsigned int)m_physObjects.size();
//	if (numberOfPhysObjects > 0)
//	{
//		ImGui::Begin("Object Control");
//		{
//			// Slider to select an object
//			ImGui::SliderInt("Object#", &m_selectedObject, 0, numberOfPhysObjects - 1);
//
//			// Get a reference to the selected physics object
//			auto& objIter= std::next(m_physObjects.cbegin(), m_selectedObject);
//			if (objIter == m_physObjects.cend())	return;
//			PhysicsObject* obj = *objIter;
//
//			if (obj != nullptr)
//			{
//				if (ImGui::CollapsingHeader("Editable Options"))
//				{
//					// Debug-Mode toggle
//					bool debugMode = obj->GetDebugState();
//					ImGui::Checkbox("Debug Mode", &debugMode);
//					obj->SetDebugState(debugMode);
//				}
//
//				// 
//			}
//		}
//		ImGui::End();
//	}
//}

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

	camera.update(window);

	float dt = 1.0f / 60.0f;

	if (glfwGetKey(window, GLFW_KEY_P))
		StartScene();

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
