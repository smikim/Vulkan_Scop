#include "KeyboardMovementController.h"
#include "ScopObject.h"

#include <glm/gtc/matrix_transform.hpp>

namespace scop
{
	void KeyboardMovementController::moveInPlaneXZ(GLFWwindow* window, float dt, ScopObject& gameObject)
	{
		glm::vec3 rotate{ 0 };
		if (glfwGetKey(window, keys.lookRight) == GLFW_PRESS) rotate.y += 1.f;
		if (glfwGetKey(window, keys.lookLeft) == GLFW_PRESS) rotate.y -= 1.f;
		if (glfwGetKey(window, keys.lookUp) == GLFW_PRESS) rotate.x += 1.f;
		if (glfwGetKey(window, keys.lookDown) == GLFW_PRESS) rotate.x -= 1.f;

		if (glm::dot(rotate, rotate) > std::numeric_limits<float>::epsilon()) {
			gameObject._transform.rotation += lookSpeed * dt * glm::normalize(rotate);
		}


		// limit pitch values between about +/- 85ish degrees
		gameObject._transform.rotation.x = glm::clamp(gameObject._transform.rotation.x, -1.5f, 1.5f);
		gameObject._transform.rotation.y = glm::mod(gameObject._transform.rotation.y, glm::two_pi<float>());

		float yaw = gameObject._transform.rotation.y;
		const glm::vec3 forwardDir{ sin(yaw), 0.f, cos(yaw) };
		const glm::vec3 rightDir{ forwardDir.z, 0.f, -forwardDir.x };
		const glm::vec3 upDir{ 0.f, -1.f, 0.f };

		glm::vec3 moveDir{ 0.f };
		if (glfwGetKey(window, keys.moveForward) == GLFW_PRESS) moveDir += forwardDir;
		if (glfwGetKey(window, keys.moveBackward) == GLFW_PRESS) moveDir -= forwardDir;
		if (glfwGetKey(window, keys.moveRight) == GLFW_PRESS) moveDir += rightDir;
		if (glfwGetKey(window, keys.moveLeft) == GLFW_PRESS) moveDir -= rightDir;
		if (glfwGetKey(window, keys.moveUp) == GLFW_PRESS) moveDir += upDir;
		if (glfwGetKey(window, keys.moveDown) == GLFW_PRESS) moveDir -= upDir;

		if (glm::dot(moveDir, moveDir) > std::numeric_limits<float>::epsilon()) {
			gameObject._transform.translation += moveSpeed * dt * glm::normalize(moveDir);
		}
	}

	void KeyboardMovementController::myMoveInPlaneXZ(GLFWwindow* window, float dt, ScopObject& gameObject)
	{
		glm::vec3 rotate{ 0 };
		if (glfwGetKey(window, keys.lookRight) == GLFW_PRESS) rotate.y += 1.f;
		if (glfwGetKey(window, keys.lookLeft) == GLFW_PRESS) rotate.y -= 1.f;
		if (glfwGetKey(window, keys.lookUp) == GLFW_PRESS) rotate.x += 1.f;
		if (glfwGetKey(window, keys.lookDown) == GLFW_PRESS) rotate.x -= 1.f;

		if (glm::dot(rotate, rotate) > std::numeric_limits<float>::epsilon()) {
			glm::vec3 offset = lookSpeed * dt * glm::normalize(rotate);
			gameObject.moveRotation(offset.x, offset.y, offset.z);

			//gameObject._transform.rotation += lookSpeed * dt * glm::normalize(rotate);
		}


		// limit pitch values between about +/- 85ish degrees
		gameObject._transform.rotation.x = glm::clamp(gameObject._transform.rotation.x, -1.5f, 1.5f);
		gameObject._transform.rotation.y = glm::mod(gameObject._transform.rotation.y, glm::two_pi<float>());

		float yaw = gameObject._transform.rotation.y;
		const glm::vec3 forwardDir{ sin(yaw), 0.f, cos(yaw) };
		const glm::vec3 rightDir{ forwardDir.z, 0.f, -forwardDir.x };
		const glm::vec3 upDir{ 0.f, -1.f, 0.f };

		glm::vec3 moveDir{ 0.f };
		if (glfwGetKey(window, keys.moveForward) == GLFW_PRESS) moveDir += forwardDir;
		if (glfwGetKey(window, keys.moveBackward) == GLFW_PRESS) moveDir -= forwardDir;
		if (glfwGetKey(window, keys.moveRight) == GLFW_PRESS) moveDir += rightDir;
		if (glfwGetKey(window, keys.moveLeft) == GLFW_PRESS) moveDir -= rightDir;
		if (glfwGetKey(window, keys.moveUp) == GLFW_PRESS) moveDir += upDir;
		if (glfwGetKey(window, keys.moveDown) == GLFW_PRESS) moveDir -= upDir;

		if (glm::dot(moveDir, moveDir) > std::numeric_limits<float>::epsilon()) {
			glm::vec3 offset = moveSpeed * dt * glm::normalize(moveDir);
			gameObject.moveTranslation(offset.x, offset.y, offset.z);

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

			glm::vec3 rotate{ 0 };
			if (obj->_transform.spinAtX)
				rotate.x += 0.01f;
			if (obj->_transform.spinAtY)
				rotate.y += 0.01f;
			if (obj->_transform.spinAtZ)
				rotate.z += 0.01f;

			if (glm::dot(rotate, rotate) > std::numeric_limits<float>::epsilon()) {
				obj->_transform.rotation += lookSpeed * dt * glm::normalize(rotate);
			}


			const glm::vec3 forwardDir{ 0.f, 0.f, 1.f };
			//const glm::vec3 rightDir{ forwardDir.z, 0.f, -forwardDir.x };
			glm::vec3 upDir{ 0.f, -1.f, 0.f };

			const glm::vec3 rightDir = glm::normalize(glm::cross(upDir, forwardDir));
			//const glm::vec3 rightDir{ 1.0f, 0.0f, 0.0f };
			upDir = glm::normalize(glm::cross(forwardDir, rightDir));

			glm::vec3 moveDir{ 0.f };
			if (glfwGetKey(window, moves.moveUpInZ) == GLFW_PRESS) moveDir += forwardDir;
			if (glfwGetKey(window, moves.moveDownInZ) == GLFW_PRESS) moveDir -= forwardDir;
			if (glfwGetKey(window, moves.moveRightInX) == GLFW_PRESS) moveDir += rightDir;
			if (glfwGetKey(window, moves.moveLeftInX) == GLFW_PRESS) moveDir -= rightDir;
			if (glfwGetKey(window, moves.moveRightInY) == GLFW_PRESS) moveDir += upDir;
			if (glfwGetKey(window, moves.moveLeftInY) == GLFW_PRESS) moveDir -= upDir;

			if (glm::dot(moveDir, moveDir) > std::numeric_limits<float>::epsilon()) {
				obj->_transform.translation += moveSpeed * dt * glm::normalize(moveDir);
			}
		}
	}
}