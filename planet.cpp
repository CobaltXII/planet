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

/*

Load a shader program from two files.

*/

GLuint load_shader_program
(
	std::string shader_path_0,
	std::string shader_path_1,

	GLenum shader_type_0,
	GLenum shader_type_1
)
{
	// Open shader_path_0 and shader_path_1 as input file streams.

	std::ifstream shader_file_0(shader_path_0);
	std::ifstream shader_file_1(shader_path_1);

	if (!shader_file_0.is_open())
	{
		std::cout << "Could not open file \"" << shader_path_0 << "\"." << std::endl;

		exit(EXIT_FAILURE);
	}
	else if (!shader_file_1.is_open())
	{
		std::cout << "Could not open file \"" << shader_path_1 << "\"." << std::endl;

		exit(EXIT_FAILURE);
	}

	// Load the text context of shader_path_0 and shader_path_1 into 
	// shader_buffer_0 and shader_buffer_1.

	std::stringstream shader_buffer_0;
	std::stringstream shader_buffer_1;

	shader_buffer_0 << shader_file_0.rdbuf() << "\0";
	shader_buffer_1 << shader_file_1.rdbuf() << "\0";

	// Convert shader_buffer_0 and shader_buffer_1 from std::stringstream to
	// std::string, and then to const GLchar* (const char*).

	std::string shader_text_0 = shader_buffer_0.str();
	std::string shader_text_1 = shader_buffer_1.str();

	const GLchar* shader_data_0 = shader_text_0.c_str();
	const GLchar* shader_data_1 = shader_text_1.c_str();

	// Create shader_0 and shader_1 with the types shader_type_0 and 
	// shader_type_1, then source them with shader_data_0 and shader_data_1.

	GLuint shader_0 = glCreateShader(shader_type_0);
	GLuint shader_1 = glCreateShader(shader_type_1);

	glShaderSource(shader_0, 1, &shader_data_0, NULL);
	glShaderSource(shader_1, 1, &shader_data_1, NULL);

	// Compile shader_0 and shader_1.

	glCompileShader(shader_0);
	glCompileShader(shader_1);

	// Check if shader_0 or shader_1 failed compilation. If so, print out the
	// error message provided by OpenGL.

	GLint success_0 = 0;
	GLint success_1 = 0;

	GLchar crash_information_0[16 * 1024];
	GLchar crash_information_1[16 * 1024];

	glGetShaderiv(shader_0, GL_COMPILE_STATUS, &success_0);
	glGetShaderiv(shader_1, GL_COMPILE_STATUS, &success_1);

	if (!success_0)
	{
		std::cout << "Could not compile shader loaded from \"" << shader_path_0 << "\"." << std::endl;

		glGetShaderInfoLog(shader_0, 16 * 1024, NULL, crash_information_0);

		std::cout << crash_information_0;

		exit(EXIT_FAILURE);
	}
	else if (!success_1)
	{
		std::cout << "Could not compile shader loaded from \"" << shader_path_1 << "\"." << std::endl;

		glGetShaderInfoLog(shader_1, 16 * 1024, NULL, crash_information_1);

		std::cout << crash_information_1;

		exit(EXIT_FAILURE);
	}

	// Create an empty shader program.

	GLuint shader_program = glCreateProgram();

	// Attach shader_0 and shader_1 to shader_program, and then link 
	// shader_program.

	glAttachShader(shader_program, shader_0);
	glAttachShader(shader_program, shader_1);

	glLinkProgram(shader_program);

	// Check if shader_program failed linkage. If so, print out the error 
	// message provied by OpenGL.

	GLint success_program = 0;

	glGetProgramiv(shader_program, GL_LINK_STATUS, &success_program);

	if (!success_program)
	{
		std::cout << "Could not link shader program loaded from \"" << shader_path_0 << "\" and \"" << shader_path_1 << "\"." << std::endl;

		GLchar crash_information_program[16 * 1024];

		glGetProgramInfoLog(shader_program, 16 * 1024, NULL, crash_information_program);

		std::cout << crash_information_program;

		exit(EXIT_FAILURE);
	}

	// Delete shader_0 and shader_1, then return shader_program.

	glDeleteShader(shader_0);
	glDeleteShader(shader_1);

	return shader_program;
}

/*

Entry point.

*/

int main(int argc, char** argv)
{
	// Initialize SDL.

	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		std::cout << "Could not initialize SDL." << std::endl;

		return EXIT_FAILURE;
	}

	// Create a SDL_Window*.

	int sdl_x_res = 960;
	int sdl_y_res = 960;

	SDL_Window* sdl_window = SDL_CreateWindow
	(
		"SDL 2.0 with OpenGL 3.3 Core",

		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,

		sdl_x_res,
		sdl_y_res,

		SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL
	);

	// Make sure the SDL_Window* was created successfully.

	if (!sdl_window)
	{
		std::cout << "Could not create a SDL_Window*." << std::endl;

		return EXIT_FAILURE;
	}

	// Request OpenGL 3.3 Core.

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	// Create a SDL_GLContext.

	SDL_GLContext gl_context = SDL_GL_CreateContext(sdl_window);

	// Make sure the SDL_GLContext was created successfully.

	if (!gl_context)
	{
		std::cout << "Could not create a SDL_GLContext." << std::endl;

		return EXIT_FAILURE;
	}

	// Load all OpenGL 3.3 Core functions using GLAD.

	if (!gladLoadGLLoader(SDL_GL_GetProcAddress)) 
	{
		std::cout << "Could not load OpenGL 3.3 Core functions using GLAD." << std::endl;

		return EXIT_FAILURE;
	}

	// Make sure the OpenGL version that was loaded by GLAD is greater than or
	// equal to OpenGL 3.3.

	if (GLVersion.major * 10 + GLVersion.minor < 33)
	{
		std::cout << "Could not load OpenGL 3.3 Core functions using GLAD." << std::endl;

		return EXIT_FAILURE;
	}

	// Create and initialize a noise::module::Perlin. This noise module will
	// dictate the general shape of the islands on the planet.

	noise::module::Perlin noise_1;

	{
		// Set the seed to the current time, so that the output noise will be
		// slightly different every time.

		noise_1.SetSeed(time(NULL));

		// Set the octave count to 16 for a high level of detail.

		noise_1.SetOctaveCount(16);

		// Set the frequency to 2.0f to make the noise more random and less
		// coherent.

		noise_1.SetFrequency(2.0f);
	}

	// Create and initialize a noise::module::RidgedMulti. This noise module 
	// will create round basins and sharp mountain ranges.

	noise::module::RidgedMulti noise_2;

	{
		// Set the seed to the current time, so that the output noise will be
		// slightly different every time.

		noise_1.SetSeed(time(NULL));

		// Set the octave count to 16 for a high level of detail.

		noise_1.SetOctaveCount(16);

		// Set the frequency to 2.0f to make the noise more random and less
		// coherent.

		noise_1.SetFrequency(2.0f);
	}

	// Create a gradient to define the color of points on the planet based the
	// point's elevation.

	noise::utils::GradientColor color_map;

	color_map.Clear();
	
	color_map.AddGradientPoint(0.0f - 1.0000f, noise::utils::Color(0x00, 0x00, 0x80, 0xFF));
	color_map.AddGradientPoint(0.0f - 0.2500f, noise::utils::Color(0x00, 0x00, 0xFF, 0xFF));
	color_map.AddGradientPoint(0.0f + 0.0000f, noise::utils::Color(0x00, 0x80, 0xFF, 0xFF));
	color_map.AddGradientPoint(0.0f + 0.0625f, noise::utils::Color(0xF0, 0xF0, 0x40, 0xFF));
	color_map.AddGradientPoint(0.0f + 0.1250f, noise::utils::Color(0x20, 0xA0, 0x00, 0xFF));
	color_map.AddGradientPoint(0.0f + 0.3750f, noise::utils::Color(0xE0, 0xE0, 0x00, 0xFF));
	color_map.AddGradientPoint(0.0f + 0.7500f, noise::utils::Color(0x80, 0x80, 0x80, 0xFF));
	color_map.AddGradientPoint(0.0f + 1.0000f, noise::utils::Color(0xFF, 0xFF, 0xFF, 0xFF));

	// Generate the base icosphere.

	std::vector<glm::vec3> icosphere_managed_vertices = create_icosphere(8);

	// Allocate space to hold the vertex data of the icosphere.

	float* icosphere_vertices = (float*)malloc(icosphere_managed_vertices.size() * (9 * sizeof(float)));

	// Perturb the terrain using the noise modules by iterating through each 
	// triangle rather than each vertex. This is done to make it easy to 
	// calculate triangle normals.

	for (int i = 0; i < icosphere_managed_vertices.size(); i += 3)
	{
		// Create an array to hold the noise values at the three vertices of 
		// the current triangle.

		float noise_map[3];

		for (int j = 0; j < 3; j++)
		{
			// Get the current vertex.

			glm::vec3 vertex = icosphere_managed_vertices[i + j];

			// Get the noise value at the current vertex.

			float actual_noise_value = noise_1.GetValue(vertex.x, vertex.y, vertex.z) * (noise_2.GetValue(vertex.x, vertex.y, vertex.z) + 0.2f);

			// Clamp the noise value to create smooth, flat water.

			float noise_value = std::max(0.0f, actual_noise_value);

			noise_map[j] = actual_noise_value;

			// Perturb the current vertex by the noise value.

			icosphere_managed_vertices[i + j] = vertex * (1.0f + noise_value * 0.075f);
		}

		// Calculate the triangle's normal.

		glm::vec3 edge_1 = icosphere_managed_vertices[i + 1] - icosphere_managed_vertices[i];
		glm::vec3 edge_2 = icosphere_managed_vertices[i + 2] - icosphere_managed_vertices[i];

		glm::vec3 normal = glm::normalize(glm::cross(edge_1, edge_2));

		float nx = normal.x;
		float ny = normal.y;
		float nz = normal.z;
		
		// Generate the vertex data.

		for (int j = 0; j < 3; j++)
		{
			utils::Color color = color_map.GetColor(noise_map[j]);

			// Write the position of the current vertex.

			icosphere_vertices[(i + j) * 9 + 0] = icosphere_managed_vertices[i + j].x;
			icosphere_vertices[(i + j) * 9 + 1] = icosphere_managed_vertices[i + j].y;
			icosphere_vertices[(i + j) * 9 + 2] = icosphere_managed_vertices[i + j].z;

			// Write the color of the current vertex.

			icosphere_vertices[(i + j) * 9 + 3] = color.red / 255.0f;

			icosphere_vertices[(i + j) * 9 + 4] = color.green / 255.0f;

			icosphere_vertices[(i + j) * 9 + 5] = color.blue / 255.0f;

			// Write the surface normal of the current vertex.

			icosphere_vertices[(i + j) * 9 + 6] = nx;
			icosphere_vertices[(i + j) * 9 + 7] = ny;
			icosphere_vertices[(i + j) * 9 + 8] = nz;
		}
	}

	// Generate a VAO and a VBO for the icosphere.

	GLuint icosphere_vao;
	GLuint icosphere_vbo;

	glGenVertexArrays(1, &icosphere_vao);

	glGenBuffers(1, &icosphere_vbo);

	// Bind the VAO and the VBO of the icosphere to the current state.

	glBindVertexArray(icosphere_vao);

	glBindBuffer(GL_ARRAY_BUFFER, icosphere_vbo);

	// Upload the icosphere data to the VBO.

	glBufferData(GL_ARRAY_BUFFER, icosphere_managed_vertices.size() * (9 * sizeof(float)), icosphere_vertices, GL_STATIC_DRAW);

	// Enable the required vertex attribute pointers.

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(0 * sizeof(float)));
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3 * sizeof(float)));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(6 * sizeof(float)));

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	// Unbind the VAO and the VBO of the icosphere from the current state.

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

	// Load the default shader program.

	GLuint default_shader_program = load_shader_program("default_vertex.glsl", "default_fragment.glsl", GL_VERTEX_SHADER, GL_FRAGMENT_SHADER);

	// Define variables to hold the state of the mouse and the application's
	// state.

	int sdl_mouse_x = 0;
	int sdl_mouse_y = 0;

	bool sdl_mouse_l = false;
	bool sdl_mouse_r = false;

	bool sdl_running = true;
	// Exit successfully.

	return EXIT_SUCCESS;
}