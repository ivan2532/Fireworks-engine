#version 460 core

vec3 lightDirection = vec3(1.0f, 1.0f, 1.0f);
vec4 materialColor = vec4(0.8f, 0.8f, 0.8f, 1.0f);
vec4 lightColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);

uniform sampler2D tex_diffuse1;
uniform sampler2D tex_diffuse2;
uniform sampler2D tex_specular1;
uniform sampler2D tex_specular2;
uniform sampler2D tex_normal1;
uniform sampler2D tex_normal2;

in vec3 normal;
in vec2 texCoord;

out vec4 color;

void main()
{
	float lightStrength = max(0.0f, dot(normal, normalize(lightDirection)));
	color = vec4(0.2f, 0.2f, 0.2f, 1.0f) + lightColor * lightStrength * materialColor;
}