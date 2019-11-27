#version 330 core

in vec4 worldNormal;
in vec2 uv;
out vec4 FragColor;

uniform sampler2D diffuse1;
uniform sampler2D diffuse2;


void main()
{

   vec3 L = vec3(0.5774, 0.5774, 0.5774);
   float NdotL = dot(worldNormal.xyz, L);
   vec4 d1 = texture(diffuse1,uv);
   vec4 d2 = texture(diffuse2,uv);
   FragColor = mix(d1,d2,NdotL);
}
