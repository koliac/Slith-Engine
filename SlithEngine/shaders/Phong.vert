 #version 330 core
 layout (location = 0) in vec3 aPos;
 layout (location = 1) in vec3 aNormal;
 layout (location = 2) in vec2 aUV;
 layout (location = 3) in vec3 aTangent;

 uniform mat4 model;
 uniform mat4 view;
 uniform mat4 projection;

 out vec3 worldNormal;
 out vec3 worldTangent;
 out vec3 worldBitangent;
 out vec2 uv;


 void main()
 {
    worldNormal = vec3(normalize(model*vec4(aNormal,0.0)));
    worldTangent = vec3(normalize(model*vec4(aTangent,0.0)));
    worldTangent = normalize(worldTangent - dot(worldTangent, worldNormal)*worldNormal);
    worldBitangent = cross(worldNormal, worldTangent);
    uv = aUV;
    gl_Position = projection * view  * model* vec4(aPos, 1.0f);
    

 }