#version 330 core

in vec4 worldNormal;
in vec2 uv;
out vec3 modelTangent;
out vec3 modelNormal;

out vec4 FragColor;

uniform sampler2D diffuse;
uniform sampler2D normal;


void main()
{

   vec3 L = vec3(0.5774, 0.5774, 0.5774);
   float NdotL = dot(worldNormal.xyz, L);
   vec4 d1 = texture(diffuse,uv);
   vec4 d2 = texture(normal,uv);
   FragColor = d2;
}
