#include "Scop.h"

namespace scop
{
	Scop::Scop()
	{

	}

	Scop::~Scop()
	{
	}

	void Scop::run()
	{
		while (!_window.shouldClose()) {

			glfwPollEvents();
		}
	}

	void Scop::prepare()
	{
	}

	void Scop::render()
	{
		
	}
	

}
