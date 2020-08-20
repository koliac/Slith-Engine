#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aUV;
layout (location = 3) in vec3 aTangent;

out vec2 TexCoords;
out vec3 WorldPos;
out vec3 Normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	TexCoords = aUV;
	Normal = vec3(normalize(model*vec4(aNormal,0.0)));
	WorldPos = vec3(model* vec4(aPos, 1.0f));

	gl_Position = projection * view  * model* vec4(WorldPos, 1.0f);
}