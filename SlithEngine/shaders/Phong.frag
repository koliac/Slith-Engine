#version 330 core

in vec3 worldTangent;
in vec3 worldNormal;
in vec3 worldBitangent;
in vec2 uv;

out vec4 FragColor;

uniform sampler2D diffuse;
uniform sampler2D normal;
uniform sampler2D roughness;

uniform mat4 view;

vec3 normalFromTexture(sampler2D normalMap){
  vec3 tn = texture(normalMap, uv).rgb;
  tn = tn*2.0 - 1.0;
  return mat3(worldTangent, worldBitangent, worldNormal) * tn;
}

vec3 reflect(vec3 L, vec3 N){
  return (2.0* dot(L, N)*N) - L;
}

void main()
{

   vec3 L = vec3(0.5774, 0.5774, 0.5774);
   vec3 N = normalFromTexture(normal);

   vec3 viewL = vec3(view*vec4(L.xyz,0.0));
   vec3 viewE = vec3(0.0,0.0,1.0f);
   vec3 viewN = vec3(view*vec4(N.xyz,0.0));

   float NdotL = dot(viewN, viewL);
   vec3 viewH = normalize(viewL + viewE);
   float NdotH = dot(viewN, viewH);

    float specular = 1.0 - texture(roughness, uv).r;
   float specularIntensity =  pow( clamp( NdotH,0.0,1.0 ), 64 )*specular;


   vec4 d = texture(diffuse,uv);
  
   //vec4 d = vec4(0.8,0.8,0.8,1.0);
   FragColor = (0.2*vec4(135.0/255.0,206.0/255.0,235.0/255.0,1.0))+d*NdotL + vec4(1.0,1.0,1.0,1.0)*specularIntensity;
   //FragColor = specular;
}
