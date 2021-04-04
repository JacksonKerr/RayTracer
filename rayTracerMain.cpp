#include "Scene.h"
#include "SceneReader.h"

/**
 * \mainpage COSC 342 Ray Tracer 2021.
 *
 * This is the documentation for the ray tracer for COSC 342, Computer Graphics, 2021. 
 * The ray tracer is written to demonstrate the principles of ray tracing, and so 
 * favours clarity over efficiency. The project is quite large, but consists of a few
 * basic components.
 *
 * At the core of the ray tracer is a Scene to be rendered. A Scene typically consists
 * of a Camera, one or more Object%s, and one or more LightSource%s. Each of these is
 * represented by a class hierarchy, so there is an abstract Object base class, which
 * then has a number of specific sub-classes such as Sphere, Plane, and Cube.
 *
 * The Scene has a render() method, which casts Ray%s into the scene for each pixel.
 * These Ray%s are tested against each Object in the Scene, via the Object's intersect()
 * method.
 *
 * There are several parts of the ray tracer that have not yet been implemented.
 * These are listed on the \ref todo.
 */

/** \file
 * \brief Main file for the ray tracer program.
 */

/** \brief Create a Scene, read input from files, and render the Scene.
 *
 * The main function creates an empty Scene then uses a SceneReader
 * to add information based on input files specified as command line
 * arguments. Multiple scene files can be specified, and they are read
 * in the order provided.
 *
 * The scene is then rendered and saved to file, as long as there
 * is a Camera specified.
 * 
 */
int main (int argc, char *argv[]) {

	Scene scene;
	
	SceneReader reader(&scene);
	
	for (int i = 1; i < argc; ++i) {
		reader.read(argv[i]);
	}

	if (scene.hasCamera()) {
		scene.render();
	} else {
		std::cerr << "Cannot render a scene with no camera!" << std::endl;
	}

	return 0;
}
