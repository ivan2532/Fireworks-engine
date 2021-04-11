#version 460 core
layout (location = 0) in vec3 inPos;
layout (location = 1) in vec3 inNormal;
layout (location = 2) in vec2 inTexCoord;

out vec2 texCoord;
out vec3 vertexPos;
out vec3 normal;

uniform mat4 model;
uniform mat4 viewProj;

void main()
{
	texCoord = inTexCoord;
	normal = mat3(transpose(inverse(model))) * inNormal;
	vertexPos = inPos;
	gl_Position = viewProj * model * vec4(inPos, 1.0f);
}