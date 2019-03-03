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
	// Calculate the light's intensity.

	float intensity = dot(light, normal_attribute);

	// Pass the color attribute input from the vertex shader to OpenGL, after
	// calculating diffuse and specular lighting.

	fragment_color = vec4(color_attribute + (vec3(1.0f, 1.0f, 1.0f) * pow(intensity, 50.0f)), 1.0f) * intensity;
}