#version 330 core

// Vertex attributes.

layout (location = 0) in vec3 attribute_position;

layout (location = 1) in vec3 attribute_color;

layout (location = 2) in vec3 attribute_normal;

// Input matrices.

uniform mat4 matrix_projection;

uniform mat4 matrix_view;

uniform mat4 matrix_model;

// Output to the fragment shader.

out vec3 position_attribute;

out vec3 color_attribute;

out vec3 normal_attribute;

out vec3 light;

// Main shader code.

void main()
{
	mat3 normal_matrix = mat3(matrix_model);

	// Multiply the vertex position attribute by the projection, view, and
	// model matrices to find the final position.

	gl_Position = matrix_projection * matrix_view * matrix_model * vec4(attribute_position, 1.0f);

	// Pass the position attribute, color attribute, and normal attribute to
	// the fragment shader.

	position_attribute = attribute_position;

	color_attribute = attribute_color;

	normal_attribute = attribute_normal;

	// Rotate the light's direction to ensure that the light is always facing
	// from the camera towards the planet. Pass the light direction to the 
	// fragment shader.

	light = normalize(vec3(0.0f, 0.0f, 1.0f) * normal_matrix);
}