#include "KeyboardMovementController.h"
#include "ScopObject.h"
#include "math.h"
#include <glm/gtc/matrix_transform.hpp>

namespace scop
{
	void KeyboardMovementController::moveInPlaneXZ(GLFWwindow* window, float dt, ScopObject& gameObject)
	{
		mymath::Vec3 rotate{ 0 };
		if (glfwGetKey(window, keys.lookRight) == GLFW_PRESS) rotate._y += 1.f;
		if (glfwGetKey(window, keys.lookLeft) == GLFW_PRESS) rotate._y -= 1.f;
		if (glfwGetKey(window, keys.lookUp) == GLFW_PRESS) rotate._x += 1.f;
		if (glfwGetKey(window, keys.lookDown) == GLFW_PRESS) rotate._x -= 1.f;

		if (rotate.dot(rotate) > std::numeric_limits<float>::epsilon()) {
			gameObject._transform.rotation += lookSpeed * dt * rotate.normalize();
		}


		// limit pitch values between about +/- 85ish degrees
		gameObject._transform.rotation._x = mymath::clamp(gameObject._transform.rotation._x, -1.5f, 1.5f);
		gameObject._transform.rotation._y = mymath::mod(gameObject._transform.rotation._y, mymath::two_pi<float>());

		float yaw = gameObject._transform.rotation._y;
		const mymath::Vec3 forwardDir{ sin(yaw), 0.f, cos(yaw) };
		const mymath::Vec3 rightDir{ forwardDir._z, 0.f, -forwardDir._x };
		const mymath::Vec3 upDir{ 0.f, -1.f, 0.f };

		mymath::Vec3 moveDir{ 0.f };
		if (glfwGetKey(window, keys.moveForward) == GLFW_PRESS) moveDir += forwardDir;
		if (glfwGetKey(window, keys.moveBackward) == GLFW_PRESS) moveDir -= forwardDir;
		if (glfwGetKey(window, keys.moveRight) == GLFW_PRESS) moveDir += rightDir;
		if (glfwGetKey(window, keys.moveLeft) == GLFW_PRESS) moveDir -= rightDir;
		if (glfwGetKey(window, keys.moveUp) == GLFW_PRESS) moveDir += upDir;
		if (glfwGetKey(window, keys.moveDown) == GLFW_PRESS) moveDir -= upDir;
		
		if (moveDir.dot(moveDir) > std::numeric_limits<float>::epsilon()) {
			gameObject._transform.translation += moveSpeed * dt * moveDir.normalize();
		}
	}

	void KeyboardMovementController::myMoveInPlaneXZ(GLFWwindow* window, float dt, ScopObject& gameObject)
	{
		mymath::Vec3 rotate{ 0 };
		if (glfwGetKey(window, keys.lookRight) == GLFW_PRESS) rotate._y += 1.f;
		if (glfwGetKey(window, keys.lookLeft) == GLFW_PRESS) rotate._y -= 1.f;
		if (glfwGetKey(window, keys.lookUp) == GLFW_PRESS) rotate._x += 1.f;
		if (glfwGetKey(window, keys.lookDown) == GLFW_PRESS) rotate._x -= 1.f;

		if (rotate.dot(rotate) > std::numeric_limits<float>::epsilon()) {
			mymath::Vec3 offset = lookSpeed * dt * rotate.normalize();
			gameObject.moveRotation(offset._x, offset._y, offset._z);

			//gameObject._transform.rotation += lookSpeed * dt * glm::normalize(rotate);
		}


		// limit pitch values between about +/- 85ish degrees
		gameObject._transform.rotation._x = mymath::clamp(gameObject._transform.rotation._x, -1.5f, 1.5f);
		gameObject._transform.rotation._y = mymath::mod(gameObject._transform.rotation._y, mymath::two_pi<float>());

		float yaw = gameObject._transform.rotation._y;
		const mymath::Vec3 forwardDir{ sin(yaw), 0.f, cos(yaw) };
		const mymath::Vec3 rightDir{ forwardDir._z, 0.f, -forwardDir._x };
		const mymath::Vec3 upDir{ 0.f, -1.f, 0.f };

		mymath::Vec3 moveDir{ 0.f };
		if (glfwGetKey(window, keys.moveForward) == GLFW_PRESS) moveDir += forwardDir;
		if (glfwGetKey(window, keys.moveBackward) == GLFW_PRESS) moveDir -= forwardDir;
		if (glfwGetKey(window, keys.moveRight) == GLFW_PRESS) moveDir += rightDir;
		if (glfwGetKey(window, keys.moveLeft) == GLFW_PRESS) moveDir -= rightDir;
		if (glfwGetKey(window, keys.moveUp) == GLFW_PRESS) moveDir += upDir;
		if (glfwGetKey(window, keys.moveDown) == GLFW_PRESS) moveDir -= upDir;

		if (moveDir.dot(moveDir) > std::numeric_limits<float>::epsilon()) {
			mymath::Vec3 offset = moveSpeed * dt * moveDir.normalize();
			gameObject.moveTranslation(offset._x, offset._y, offset._z);

			//gameObject._transform.translation += moveSpeed * dt * glm::normalize(moveDir);
		}
	}

	void KeyboardMovementController::moveObjects(GLFWwindow* window, float dt, std::vector<ScopObject*>& gameObjects)
	{
		for (size_t i = 0; i < gameObjects.size(); ++i) {

			auto obj = gameObjects[i];

			if (glfwGetKey(window, moves.spinOnX) == GLFW_PRESS && obj->_transform.spinOnX == false)
			{
				obj->_transform.spinAtX = !obj->_transform.spinAtX;
				obj->_transform.spinOnX = true;
			}
			if (glfwGetKey(window, moves.spinOnX) == GLFW_RELEASE)
				obj->_transform.spinOnX = false;

			if (glfwGetKey(window, moves.spinOnY) == GLFW_PRESS && obj->_transform.spinOnY == false)
			{
				obj->_transform.spinAtY = !obj->_transform.spinAtY;
				obj->_transform.spinOnY = true;
			}
			if (glfwGetKey(window, moves.spinOnY) == GLFW_RELEASE)
				obj->_transform.spinOnY = false;

			if (glfwGetKey(window, moves.spinOnZ) == GLFW_PRESS && obj->_transform.spinOnZ == false)
			{
				obj->_transform.spinAtZ = !obj->_transform.spinAtZ;
				obj->_transform.spinOnZ = true;
			}
			if (glfwGetKey(window, moves.spinOnZ) == GLFW_RELEASE)
				obj->_transform.spinOnZ = false;

		mymath::Vec3 rotate{ 0 };
			if (obj->_transform.spinAtX)
				rotate._x += 0.01f;
			if (obj->_transform.spinAtY)
				rotate._y += 0.01f;
			if (obj->_transform.spinAtZ)
				rotate._z += 0.01f;

			if (rotate.dot(rotate) > std::numeric_limits<float>::epsilon()) {
				obj->_transform.rotation += lookSpeed * dt * rotate.normalize();
			}


			const mymath::Vec3 forwardDir{ 0.f, 0.f, 1.f };
			//const glm::vec3 rightDir{ forwardDir.z, 0.f, -forwardDir.x };
			mymath::Vec3 upDir{ 0.f, -1.f, 0.f };

			//const mymath::Vec3 rightDir = upDir.cross(forwardDir).normalize();
			
			//const mymath::Vec3 rightDir = forwardDir.cross(upDir).normalize();
			
			const mymath::Vec3 rightDir{ 1.0f, 0.0f, 0.0f };
			//upDir = forwardDir.cross(rightDir).normalize();
			//upDir = rightDir.cross(forwardDir).normalize();

			mymath::Vec3 moveDir{ 0.f };
			if (glfwGetKey(window, moves.moveUpInZ) == GLFW_PRESS) {
				std::cout << forwardDir._x << forwardDir._y << forwardDir._z << std::endl;
				moveDir += forwardDir;
				std::cout << moveDir._x << moveDir._y << moveDir._z << std::endl;
			}
			if (glfwGetKey(window, moves.moveDownInZ) == GLFW_PRESS) moveDir -= forwardDir;
			if (glfwGetKey(window, moves.moveRightInX) == GLFW_PRESS) moveDir += rightDir;
			if (glfwGetKey(window, moves.moveLeftInX) == GLFW_PRESS) moveDir -= rightDir;
			if (glfwGetKey(window, moves.moveRightInY) == GLFW_PRESS) moveDir += upDir;
			if (glfwGetKey(window, moves.moveLeftInY) == GLFW_PRESS) moveDir -= upDir;

			if (moveDir.dot(moveDir) > std::numeric_limits<float>::epsilon()) {
				obj->_transform.translation += moveSpeed * dt * moveDir.normalize();
				std::cout << obj->_transform.translation._x << obj->_transform.translation._y << obj->_transform.translation._z << std::endl;
			}
		}
	}
	void KeyboardMovementController::switchColoring(GLFWwindow* window, std::vector<ScopObject*>& gameObjects)
	{
		for (size_t i = 0; i < gameObjects.size(); ++i) {

			auto obj = gameObjects[i];

			if (glfwGetKey(window, coloring.rainbow) == GLFW_PRESS)
			{
				obj->_colorMode = 0;
			}
			else if (glfwGetKey(window, coloring.gray) == GLFW_PRESS)
			{
				obj->_colorMode = 1;
			}
			else if (glfwGetKey(window, coloring.texture) == GLFW_PRESS)
			{
				obj->_colorMode = 2;
			}
			else if (glfwGetKey(window, coloring.original) == GLFW_PRESS)
			{
				obj->_colorMode = 3;
			}
		}
	}
}