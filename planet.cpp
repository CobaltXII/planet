/*

GLM header include directives. Please use GLM 0.9.9.3 or greater for known 
results. Previous versions of GLM are not guaranteed to work correctly.

*/

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include <glm/mat4x4.hpp>

#include <glm/gtc/matrix_transform.hpp>

/*

libnoise header include directives. libnoise is used to generate coherent 
noise for generating procedural planets.

*/

#include <noise/noise.h>

/*

noiseutils header include directives. noiseutils is used as a utility library
on top of libnoise.

*/

#include "noiseutils.h"

/*

GLAD header include directives. GLAD is used to load OpenGL 3.3 Core 
functions.

*/

#include "glad.h"

/*

SDL header include directives. Please use SDL 2.0.9 or greater for known 
results. Previous versions of SDL are not guaranteed to work correctly.

*/

#include <SDL2/SDL.h>

/*

Standard header include directives.

*/

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <tuple>

/*

A std::tuple<int, int, int> is used to represent a triangle defined by indices
in a list of vertices.

*/

typedef std::tuple<int, int, int> triangle_indices;

/*

Add a vertex to a std::vector<glm::vec3> while ensuring that the vertex lies
on the unit sphere.

*/

int add_vertex(std::vector<glm::vec3>& vector, glm::vec3 vertex)
{
	vector.push_back(vertex / glm::length(vertex));

	return vector.size() - 1;
}
