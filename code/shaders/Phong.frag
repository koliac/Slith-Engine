#version 330 core

in vec4 worldNormal;
out vec4 FragColor;

void main()
{

   vec3 L = vec3(0.5774, 0.5774, 0.5774);
   float NdotL = dot(worldNormal.xyz, L);
   FragColor = vec4(NdotL);
}
