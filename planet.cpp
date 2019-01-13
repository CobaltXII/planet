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

/*

Return the index of a vertex in the middle of p_1 and p_2.

*/

int get_middle_point(std::vector<glm::vec3>& vector, int p_1, int p_2)
{
	glm::vec3 pt_1 = vector[p_1];
	glm::vec3 pt_2 = vector[p_2];

	glm::vec3 pt_middle = (pt_1 + pt_2) / 2.0f;

	int i = add_vertex(vector, pt_middle);

	return i;
}

/*

Create an icosphere with the given amount of subdivisions.

*/

std::vector<glm::vec3> create_icosphere(int subdivisions = 8)
{ 
	// Generate the icosphere's vertices.

	std::vector<glm::vec3> icosphere_vertices;

	// Generate the 12 vertices of an icosahedron.

	float t = (1.0f + sqrt(5.0f)) / 2.0f;

	add_vertex(icosphere_vertices, glm::vec3(-1.0f,  t, 0.0f));
	add_vertex(icosphere_vertices, glm::vec3( 1.0f,  t, 0.0f));
	add_vertex(icosphere_vertices, glm::vec3(-1.0f, -t, 0.0f));
	add_vertex(icosphere_vertices, glm::vec3( 1.0f, -t, 0.0f));

	add_vertex(icosphere_vertices, glm::vec3(0.0f, -1.0f,  t));
	add_vertex(icosphere_vertices, glm::vec3(0.0f,  1.0f,  t));
	add_vertex(icosphere_vertices, glm::vec3(0.0f, -1.0f, -t));
	add_vertex(icosphere_vertices, glm::vec3(0.0f,  1.0f, -t));

	add_vertex(icosphere_vertices, glm::vec3( t, 0.0f, -1.0f));
	add_vertex(icosphere_vertices, glm::vec3( t, 0.0f,  1.0f));
	add_vertex(icosphere_vertices, glm::vec3(-t, 0.0f, -1.0f));
	add_vertex(icosphere_vertices, glm::vec3(-t, 0.0f,  1.0f));

	// Generate the 20 faces of an icosahedron.

	std::vector<triangle_indices> icosphere_indices;

	icosphere_indices.push_back(triangle_indices(0x0, 0xB, 0x5));
	icosphere_indices.push_back(triangle_indices(0x0, 0x5, 0x1));
	icosphere_indices.push_back(triangle_indices(0x0, 0x1, 0x7));
	icosphere_indices.push_back(triangle_indices(0x0, 0x7, 0xA));
	icosphere_indices.push_back(triangle_indices(0x0, 0xA, 0xB));

	icosphere_indices.push_back(triangle_indices(0x1, 0x5, 0x9));
	icosphere_indices.push_back(triangle_indices(0x5, 0xB, 0x4));
	icosphere_indices.push_back(triangle_indices(0xB, 0xA, 0x2));
	icosphere_indices.push_back(triangle_indices(0xA, 0x7, 0x6));
	icosphere_indices.push_back(triangle_indices(0x7, 0x1, 0x8));

	icosphere_indices.push_back(triangle_indices(0x3, 0x9, 0x4));
	icosphere_indices.push_back(triangle_indices(0x3, 0x4, 0x2));
	icosphere_indices.push_back(triangle_indices(0x3, 0x2, 0x6));
	icosphere_indices.push_back(triangle_indices(0x3, 0x6, 0x8));
	icosphere_indices.push_back(triangle_indices(0x3, 0x8, 0x9));

	icosphere_indices.push_back(triangle_indices(0x4, 0x9, 0x5));
	icosphere_indices.push_back(triangle_indices(0x2, 0x4, 0xB));
	icosphere_indices.push_back(triangle_indices(0x6, 0x2, 0xA));
	icosphere_indices.push_back(triangle_indices(0x8, 0x6, 0x7));
	icosphere_indices.push_back(triangle_indices(0x9, 0x8, 0x1));

	// Subdivide the icosphere.

	for (int i = 0; i < subdivisions; i++)
	{
		// Generate a temporary mesh to hold the result of the next 
		// subdivision.

		std::vector<triangle_indices> new_icosphere_indices;

		// Subdivide each triangle in the current mesh.

		for (int j = 0; j < icosphere_indices.size(); j++)
		{
			triangle_indices tri = icosphere_indices[j];

			int a = get_middle_point(icosphere_vertices, std::get<0>(tri), std::get<1>(tri));
			int b = get_middle_point(icosphere_vertices, std::get<1>(tri), std::get<2>(tri));
			int c = get_middle_point(icosphere_vertices, std::get<2>(tri), std::get<0>(tri));

			// Add the 4 new triangles to the temporary mesh.

			new_icosphere_indices.push_back(triangle_indices(std::get<0>(tri), a, c));
			new_icosphere_indices.push_back(triangle_indices(std::get<1>(tri), b, a));
			new_icosphere_indices.push_back(triangle_indices(std::get<2>(tri), c, b));

			new_icosphere_indices.push_back(triangle_indices(a, b, c));
		}

		// Replace the current mesh with the temporary mesh.

		icosphere_indices = new_icosphere_indices;
	}

	// Convert the icosphere's structured triangle_indices vector to a list of
	// ordered vertices.

	std::vector<glm::vec3> icosphere_mesh;

	for (int i = 0; i < icosphere_indices.size(); i++)
	{
		icosphere_mesh.push_back(icosphere_vertices[std::get<0>(icosphere_indices[i])]);
		icosphere_mesh.push_back(icosphere_vertices[std::get<1>(icosphere_indices[i])]);
		icosphere_mesh.push_back(icosphere_vertices[std::get<2>(icosphere_indices[i])]);
	}

	// Return the icosphere's mesh.

	return icosphere_mesh;
};
