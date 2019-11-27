 #version 330 core
 layout (location = 0) in vec3 aPos;
 layout (location = 1) in vec3 aNormal;
 layout (location = 2) in vec2 aUV;
 layout (location = 3) in vec3 aTangent;

 uniform mat4 model;
 uniform mat4 view;
 uniform mat4 projection;

 out vec4 worldNormal;

 out vec2 uv;
 out vec3 modelTangent;
 out vec3 modelNormal;

 void main()
 {
    worldNormal = model*vec4(aNormal,0.0);
    uv = aUV;
    modelNormal = aNormal;
    modelTangent = aTangent;
    gl_Position = projection * view  * model* vec4(aPos, 1.0f);
    

 }