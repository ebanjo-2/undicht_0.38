#include <iostream>
#include <window/window.h>
#include <core/graphics_core.h>
#include <core/renderer.h>


using namespace undicht;
using namespace graphics;


int main() {
	

	Window window(800, 600, "HELLO WORLD!");

	Renderer renderer;

	while (!window.shouldClose()) {

		renderer.clearFramebuffer(0.1, 0.6, 0.1, 1.0F);

		window.update();
	}


	window.close();
		
	return 0;
}