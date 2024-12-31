#include "Scop.h"
  
namespace scop
{
	Keymovement Scop::_keymovement;

	Scop::Scop()
	{

	}

	Scop::~Scop()
	{
		deleteScopObjects();
	}

	void Scop::run()
	{
		while (!_window.shouldClose()) {

			glfwPollEvents();
			_renderer.update();
			_renderer.updateUniformBuffer();
			render();
		}

		vkDeviceWaitIdle(_renderer.getVulkanDevice()->getLogicalDevice());
	}

	void Scop::prepare()
	{
		_renderer.initVulkan();
		createScopObject();
		//_renderer.buildBasicCommandBuffers();
	}

	void Scop::render()
	{

		VkResult result = _renderer.beginRender();
		if (result != VK_SUCCESS) return;
		_renderer.beginRenderPass();
		// update uniform buffer 
		// render object
		for (size_t i = 0; i < ScopObjects.size(); i++)
		{
			ScopObjects[i]->Render();
		}
		_renderer.endRenderPass();
		result = _renderer.endRender();
		if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR)
			return ;
	}

	/*void Scop::render()
	{

		VkResult result = _renderer.prepareFrame();
		if (result == VK_ERROR_OUT_OF_DATE_KHR)
		{
			_renderer.buildBasicCommandBuffers();
		}

		result = _renderer.submitFrame();
		if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR)
		{
			_renderer.buildBasicCommandBuffers();
		}

	}*/

	ScopObject* Scop::createScopObject()
	{
		ScopObject* obj = new ScopObject;
		obj->Initialize(this);
		ScopObjects.push_back(obj);

		return obj;
	}

	void Scop::buttonRotation(Keymovement::RotationInput rotationInput)
	{
		std::cout << rotationInput << std::endl;
		switch (rotationInput) {
		case Keymovement::RotationInput::ROTATION_ADD_X: {
			_keymovement.moves.spin_x = true;
			
			break;
		}
		case Keymovement::RotationInput::ROTATION_SUB_X: {
			
			break;
		}
		case Keymovement::RotationInput::ROTATION_ADD_Y: {
			_keymovement.moves.spin_y = true;
			break;
		}
		case Keymovement::RotationInput::ROTATION_SUB_Y: {
			
			break;
		}
		case Keymovement::RotationInput::ROTATION_ADD_Z: {
			_keymovement.moves.spin_z = true;
			break;
		}
		case Keymovement::RotationInput::ROTATION_SUB_Z: {
			
			break;
		}
		default:
			
			return;
		}
	}

	void Scop::unbuttonRotation(Keymovement::RotationInput rotationInput)
	{
		switch (rotationInput) {
		case Keymovement::RotationInput::ROTATION_ADD_X: {
			_keymovement.moves.spin_x = false;
			break;
		}
		case Keymovement::RotationInput::ROTATION_SUB_X: {

			break;
		}
		case Keymovement::RotationInput::ROTATION_ADD_Y: {
			_keymovement.moves.spin_y = false;
			break;
		}
		case Keymovement::RotationInput::ROTATION_SUB_Y: {

			break;
		}
		case Keymovement::RotationInput::ROTATION_ADD_Z: {
			_keymovement.moves.spin_z = false;
			break;
		}
		case Keymovement::RotationInput::ROTATION_SUB_Z: {

			break;
		}
		default:
			return;
		}
	}

	void Scop::deleteScopObjects()
	{
		for (ScopObject* obj : ScopObjects) {
			delete obj;
		}
		ScopObjects.clear();
	}
	

}
