#pragma once

#ifndef SCENE_READER_H_INCLUDED
#define SCENE_READER_H_INCLUDED

#include "NonCopyable.h"

#include "Colour.h"
#include "Material.h"
#include "Scene.h"

#include <map>
#include <queue>
#include <string>

/** \file
 * \brief SceneReader header file.
 */

/** 
 * \brief Simple parser for Scene description files.
 * 
 * This class implements a simple parser for Scene description files.
 * A SceneReader is linked to a Scene object, and can then read one or more
 * text files describing Scene properties, Cameras, LightSources, and Objects.
 * These files are defined in blocks, and are case-insensitive. 
 * Whitespace is contracted, so new lines, spaces, and tabs are all just token separators.
 * Comments are introduced with \c #, and continue to the end of the line.
 *
 * There are five main types of block:
 * - Scene blocks
 * - Camera blocks
 * - Light blocks
 * - Material blocks
 * - Object blocks
 * Each block begins with a keyword (the type of block) and ends with the token 'End'
 * Details of each block type are given below.
 *
 * <b>Scene Blocks</b>
 *
 * Example:
\verbatim
 Scene
   renderSize 800 600
   backgroundColour 0.5 0.5 0.5
   filename output.png
   rayDepth 5
 End
 \endverbatim
 *
 * Allowed elements within a Scene block are:
 * - <tt>renderSize [width] [height]</tt>: Set the Scene's \c renderWidth and \c renderHeight properties to the given values.
 * - <tt>backgroundColour [red] [green] [blue]</tt>: Set the Scene's \c backgroundColour property to the given Colour.
 * - <tt>filename [file]</tt>: Set the Scene's \c filename property to the given value.
 * - <tt>rayDepth [number]</tt>: Set the Scene's \c rayDepth property to the given value.
 *
 * <b>Camera Blocks</b>
 *
 * Example:
\verbatim
Camera PinholeCamera 1.5
  Rotate X -15
  Translate 0 -8 -15
End
\endverbatim
 *
 * A Camera block starts with a line giving the type of Camera and a parameter for that Camera if one can be provided.
 * In the example, a PinholeCamera is created with a focalLength of 1.5
 * Allowed elements within a Scene block are:
 * - <tt>Rotate [axis] [angle]</tt>: Apply a rotation of angle degrees about the specified axis (one of X, Y, or Z) to the Camera.
 * - <tt>Translate [x] [y] [z]</tt>: Apply the specified translation to the Camera.
 * - <tt>Scale [s]</tt>: Apply a uniform scaling by the given value to the Camera.
 * - <tt>Scale3 [sx] [sy] [sz]</tt>: Apply different scaling factors along the three axes to the Camera.
 *
 * <b>Light Blocks</b>
 *
 * Examples:
\verbatim
Light PointLight
  Location 1 2 3
  Colour 20 30 40
End

Light SpotLight
  Location 0 -5 0
  Colour 10 10 10
  Direction 0 1 0
  Angle 30
End
\endverbatim
 *
 * A Light block starts with a line giving the type of Light.
 *
 * Allowed elements within a Light block are:
 * - <tt>Colour [red] [green] [blue]</tt>: Set the Light's \c colour property to the given Colour.
 * - <tt>Location [x] [y] [z]</tt>: Set the Light's \c location property to the given co-ordinates.
 * - <tt>Direction [x] [y] [z]</tt>: Set the Light's \c location property to the given co-ordinates.
 * - <tt>Angle [x]</tt>
 *
 * Not all light source types use all parameters, and any surplus elements are ignored:
 * - <tt>AmbientLight</tt>s use <tt>Colour</tt> only
 * - <tt>PointLight</tt>s use <tt>Colour</tt> and <tt>Location</tt>
 * - <tt>DirectionalLight</tt>s use <tt>Colour</tt> and <tt>Direction</tt>
 * - <tt>SpotLight</tt>s use <tt>Colour</tt>, <tt>Location</tt>, <tt>Direction</tt>, and <tt>Angle</tt>
 *
 * <b> Material Blocks </b>
 *
 * Example:
 *
\verbatim
Material Gold
  Colour 1 0.88 0.25
  Specular 1 1 1 100
  Mirror 1 0.88 0.25
End
\endverbatim
 *
 * A Material block starts with a line giving the name of a Material (in this example, 'Gold').
 * Allowed elements within a Material block are:
 * - <tt>Ambient [red] [green] [blue]</tt>: Set the Material's \c ambientColour property to the given Colour.
 * - <tt>Diffuse [red] [green] [blue]</tt>: Set the Material's \c diffuseColour property to the given Colour.
 * - <tt>Colour [red] [green] [blue]</tt>: Set the Material's \c ambientColour and \c diffuseColour properties to the given Colour.
 * - <tt>Specular [red] [green] [blue] [exponent]</tt>: Set the Material's \c specularColour property to the given Colour, and its \c specularExponent to the given value.
 * - <tt>Mirror [red] [green] [blue]</tt>: Set the Material's \c diffuseColour property to the given Colour.
 *
 * <b> Object Blocks </b>
 *
 * Example:
\verbatim
Object Sphere
  Material Gold
  Specular 0.7 0.7 0.7 75 
  Scale3 1 2 3
  Rotate Y 45
  Translate 0 0 3
\endverbatim
 *
 * An Object block starts with a line giving the type of Object to create (a Sphere in this case).
 * Allowed elements within an Object block are:
 * - <tt>Rotate [axis] [angle]</tt>: Apply a rotation of angle degrees about the specified axis (one of X, Y, or Z) to the Object.
 * - <tt>Translate [x] [y] [z]</tt>: Apply the specified translation to the Object.
 * - <tt>Scale [s]</tt>: Apply a uniform scaling by the given value to the Object.
 * - <tt>Scale3 [sx] [sy] [sz]</tt>: Apply different scaling factors along the three axes to the Object.
 * - <tt>Material [name]</tt>: Copy the properties of a previously defined Material to the Object.
 * - <tt>Ambient [red] [green] [blue]</tt>: Set the \c ambientColour property of the Object's Material to the given Colour.
 * - <tt>Diffuse [red] [green] [blue]</tt>: Set the \c diffuseColour property of the Object's Material to the given Colour.
 * - <tt>Colour [red] [green] [blue]</tt>: Set the \c ambientColour and \c diffuseColour properties of the Object's Material to the given Colour.
 * - <tt>Specular [red] [green] [blue] [exponent]</tt>: Set the\c specularColour property to the given Colour, and its \c specularExponent to the given value.
 * - <tt>Mirror [red] [green] [blue]</tt>: Set the \c diffuseColour property of the Object's Material to the given Colour.
 * Object types that can be read are: Cube, Cylinder, Octahedron, Plane, and Sphere 
 */
class SceneReader : private NonCopyable {

public:
	/** \brief SceneReader constructor. 
	 * 
	 * \param scene A pointer to the Scene which data will be read into.
	 */
	SceneReader(Scene* scene);

	/** \brief SceneReader destructor. */
	~SceneReader();

	/** \brief Read Scene data from a file.
	 *
	 * This method reads data from a file in the format discussed above. 
	 * It adds information to the Scene linked to this SceneReader, and so 
	 * multiple files can be combined into one Scene.
	 *
	 * If an error is encountered parsing the file, the program is terminated.
	 *
	 * \param filename The name of the file to read.
	 */
	void read(const std::string& filename);

private:

	/** \brief Parse a block of tokens. 
	 *
	 * When reading a file, it is separated into a squence of tokens (words and numbers)
	 * separated by whitespace. These are formed into blocks, each ending with the token 'End'.
	 * This method determines how to interpret each block.
	 *
	 * \param tokenBlock A sequence of tokens to be interpreted.
	 */
	void parseTokenBlock(std::queue<std::string>& tokenBlock);

	/** \brief Read Colour information from a block of tokens.
	 *
	 * This tries to interpret the next three items in a block of tokens as
	 * a Colour. Three tokens are taken from the block and interpreted as 
	 * red, green, and blue values for the Colour. 
	 * 
	 * If there is a problem in this process, the program is terminated.
	 *
	 * \param tokenBlock A sequence of tokens to read the Colour from.
	 * \return The Colour read from the block of tokens..
	 */
	Colour parseColour(std::queue<std::string>& tokenBlock);

	/** \brief Read a number information from a block of tokens.
	 *
	 * This tries to interpret the next item in a block of tokens as
	 * a number. The token is removed from the block.
	 * 
	 * If there is a problem in this process, the program is terminated.
	 *
	 * \param tokenBlock A sequence of tokens to read the Colour from.
	 * \return The Colour read from the block of tokens..
	 */
	double parseNumber(std::queue<std::string>& tokenBlock);

	/** \brief Parse a block of tokens representing a Scene. 
	 *
	 * This method reads Scene information from a block of tokens.
	 * The format for Scene blocks is described above, and any errors 
	 * in parsing the block will terminate the program.
	 *
	 * \param tokenBlock A sequence of tokens to be interpreted.
	 */
	void parseSceneBlock(std::queue<std::string>& tokenBlock);

	/** \brief Parse a block of tokens representing a Scene. 
	 *
	 * This method reads Scene information from a block of tokens.
	 * The format for Scene blocks is described above, and any errors 
	 * in parsing the block will terminate the program.
	 *
	 * \param tokenBlock A sequence of tokens to be interpreted.
	 */
	void parseCameraBlock(std::queue<std::string>& tokenBlock);

	/** \brief Parse a block of tokens representing a Camera. 
	 *
	 * This method reads Camera information from a block of tokens.
	 * The format for Camera blocks is described above, and any errors 
	 * in parsing the block will terminate the program.
	 *
	 * \param tokenBlock A sequence of tokens to be interpreted.
	 */
	void parseLightBlock(std::queue<std::string>& tokenBlock);

	/** \brief Parse a block of tokens representing a LightSource. 
	 *
	 * This method reads LightSource information from a block of tokens.
	 * The format for LightSource blocks is described above, and any errors 
	 * in parsing the block will terminate the program.
	 *
	 * \param tokenBlock A sequence of tokens to be interpreted.
	 */
	std::shared_ptr<Object> parseObjectBlock(std::queue<std::string>& tokenBlock);

	/** \brief Parse a block of tokens representing a Material. 
	 *
	 * This method reads Material information from a block of tokens.
	 * The format for Material blocks is described above, and any errors 
	 * in parsing the block will terminate the program.
	 *
	 * \param tokenBlock A sequence of tokens to be interpreted.
	 */
	void parseMaterialBlock(std::queue<std::string>& tokenBlock);

	Scene* scene_; //!< The Scene which information is read to.
	int startLine_; //!< The first line of the current block being parsed, for error reporting.
	std::map<std::string, Material> materials_; //!< A dictionary of Material types that have been read, and which can be used for subsequent Object properties.
};

#endif
