#version 330 core

// Input from the vertex shader.

in vec3 position_attribute;

in vec3 color_attribute;

in vec3 normal_attribute;

in vec3 light;

// Output to OpenGL.

out vec4 fragment_color;

// Shader code.

void main()
{
	// Diffuse lighting.

	vec3 diffuse = max(dot(normal_attribute, light), 0.0f) * vec3(1.0f, 1.0f, 1.0f);

	// Specular lighting.

	vec3 view_direction = normalize(position_attribute);

	vec3 reflected_direction = reflect(-light, normal_attribute);

	vec3 specular = pow(max(dot(view_direction, reflected_direction), 0.0f), 128.0f) * vec3(1.0f, 1.0f, 1.0f);

	// Combined diffuse and specular lighting.

	fragment_color = vec4(diffuse * color_attribute + specular, 1.0f);
}