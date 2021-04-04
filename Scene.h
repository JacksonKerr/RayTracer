#pragma once

#ifndef SCENE_H_INCLUDED
#define SCENE_H_INCLUDED

#include <memory>
#include <string>
#include <vector>

#include "Camera.h"
#include "Colour.h"
#include "LightSource.h"
#include "Material.h"
#include "NonCopyable.h"
#include "Object.h"
#include "Ray.h"
#include "RayIntersection.h"

/** \file
 * \brief Scene class header file.
 */

/**
 * \brief A Scene to be ray traced.
 *
 * A Scene collects Object, LightSource, and Camera data together for rendering.
 * It also includes basic information about the environment (backgroundColour, ambientLight),
 * the image to be produced (renderWidth, renderHeight, filename), and the maximum number of 
 * reflections to allow (maxRayDepth). 
 *
 * Internally it provides the methods required to render an image, such as intersecting a Ray
 * with the Objects in the Scene, and computing the Colour from a Ray.
 *
 * Scene makes use of std::shared_ptr, which is a 'smart pointer' available in C++ since the 2011
 * revision. These provide a safer alternative to raw pointers, since deallocation is handled
 * automatically. You can treat them mostly like normal pointers though, so if you have a shared 
 * pointer to a Sphere, you can get it's properties like this:
 * \code
 * // sphere_ptr is a std::shared_ptr<Sphere>
 * sphere_ptr->transform.rotateX(30);
 * \endcode
 *
 */
class Scene : private NonCopyable  {

public:

	/** \brief Default Scene constructor.
	 * This creates an empty Scene, with a black background and no ambient light.
	 * By default the images are rendered at 800x600 pixel resolution, saved
	 * to \c render.png, and allow for up to 3 reflected rays.
	 */
	Scene();

	/** \brief Scene destructor. */
	~Scene();

	/** \brief Set the Scene's Camera.
	 *
	 * Since there may be different types of Camera, a smart pointer is 
	 * used to refer to the Camera. For example:
	 * \code
	 *   Scene scene;
	 *   std::shared_ptr<PinholeCamera> camera(new PinholeCamera(1));
	 *   scene.setCamera(camera);
	 * \endcode
	 *
	 * Note that a Scene has only one Camera, so calling setCamera() will
	 * replace any existing camera.
	 * 
	 * \param camera The new Camera to add.
	 */
	void setCamera(std::shared_ptr<Camera> camera) {
		camera_ = camera;
	}

	/** \brief Add a new Object.
	 *
	 * Note that the Scene has a collection of Objects, and there is no 
	 * way to remove an Object once added. 
	 *
	 * \param object A \c std::shared_ptr to the new Object to add.
	 */
	void addObject(std::shared_ptr<Object> object) {
		objects_.push_back(object);
	}

	/** \brief Add a new LightSource.
	 *
	 * Note that the Scene has a collection of LightSources, and there is no 
	 * way to remove an LightSource once added. 
	 *
	 * \param light A \c std::shared_ptr to the new LightSource to add.
	 */
	void addLight(std::shared_ptr<LightSource> light) {
		lights_.push_back(light);
	}


	/** \brief Render an image of the Scene.
	 * 
	 * This method renders an image of the Scene. The size of the
	 * image is (renderWidth x renderHeight), and it is saved to a file specified by
	 * the Scene's filename property. The format of the file is determined by its
	 * extension. 
	 *
	 * Attempts to render a Scene with no Camera will end badly.
	 */
	void render() const;

	Colour backgroundColour; //!< Colour for any Ray that does not hit an Object.

	Colour ambientLight; //!< Ambient light level and Colour in the Scene.

	unsigned int maxRayDepth; //!< Maximum number of reflected Rays to trace.

	/** \brief Check if the Scene has a Camera.
	 *
	 * To render a scene, a Camera is required. It is possible (although
	 * possibly not interesting) to render a scene with no Object or LightSource.
	 * This method checks to see if there is a Camera.
	 *
	 * \return true if the Scene has a Camera, false otherwise.
	 */
	bool hasCamera() const; 

	unsigned int renderWidth;  //!< Width in pixels of the image to render.
	unsigned int renderHeight; //!< Height in pixels of the image to render.
	std::string filename;      //!< File to save the image to.

private:

	std::shared_ptr<Camera> camera_;                      //!< Camera to render the image with.
	std::vector<std::shared_ptr<Object>> objects_;       //!< Collection of Objects in the Scene.
	std::vector<std::shared_ptr<LightSource>> lights_;   //!< Collection of LightSources in the Scene.

	/** \brief Intersect a Ray with the Objects in a Scene
	 *
	 * This intersects the Ray with all of the Objects in the Scene and returns
	 * the first hit. If there is no hit, then a RayIntersection with infinite distance
	 * is returned.
	 *
	 * \param ray The Ray to intersect with the Objects.
	 * \return The first intersection of the Ray with the Scene.
	 */
	RayIntersection intersect(const Ray& ray) const;

	/** \brief Compute the Colour seen by a Ray in the Scene.
	 * 
	 * The Colour seen by a Ray depends on the ligthing, the first Object that it
	 * hits, and the Material properties of that Object. This method performs these
	 * computations and comptues the observed Colour. For some Objects it may be necessary
	 * to cast other Rays to deal with reflections. This can conceivably recurse forever,
	 * so a maximum number of reflections is set.
	 *
	 * If the Ray does not hit any Object, then the Scene's backgroundColour should be 
	 * returned.
	 *
	 * \todo Currently this method only deals with ambient lighting and Material properties.
	 *       It needs to be extended to account for the diffuse, specular, and mirror properties
	 *       of the Material of the Object that is hit. Recall from lectures that the formula for
	 *       a combination of ambient, diffuse, and specular lighting is
	 *       \f[ I = I_ak_a + \sum_j{I_j\left( k_d(\hat{\mathbf{\ell}}_j\cdot\hat{\mathbf{n}}) + k_s(\hat{\mathbf{e}}\cdot\hat{\mathbf{r}}_j)^n \right)},\f]
	 *       where the sum is over the LightSources in the Scene.
	 *       You will also need to cast additional Rays to check for shadows.
	 * 
	 * \param ray The Ray to intersect with the Objects in the Scene.
	 * \param rayDepth The maximum number of reflection Rays that can be cast.
	 * \return The Colour observed by the viewRay.
	 */
	Colour computeColour(const Ray& ray, unsigned int rayDepth = 0) const;

};

#endif
