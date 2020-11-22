#include <iostream>
#include <window/window.h>
#include <core/graphics_core.h>


using namespace undicht;
using namespace graphics;


int main() {
	
	GraphicsCore graphics;
	graphics.init();

	Window window;
	if (!window.open(800, 600, "HELO WORLD!")) {

		return -1;
	}

	while (!window.shouldClose()) {

		window.update();
	}

	graphics.terminate();

	return 0;
}