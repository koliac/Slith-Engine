 #version 330 core
 layout (location = 0) in vec3 aPos;
 layout (location = 1) in vec3 aNormal;

 uniform mat4 model;
 uniform mat4 view;
 uniform mat4 projection;

 out vec4 worldNormal;
 void main()
 {
    worldNormal = model*vec4(aNormal,0.0);
    gl_Position = projection * view  * model* vec4(aPos, 1.0f);
    

 }