#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D hdrBuffer;
uniform int isACES;

const mat3 aces_input_matrix =
mat3(
    vec3(0.59719f, 0.35458f, 0.04823f),
    vec3(0.07600f, 0.90834f, 0.01566f),
    vec3(0.02840f, 0.13383f, 0.83777f)
);

const mat3 aces_output_matrix =
mat3(
    vec3( 1.60475f, -0.53108f, -0.07367f),
    vec3(-0.10208f,  1.10813f, -0.00605f),
    vec3(-0.00327f, -0.07276f,  1.07602f)
);


vec3 rtt_and_odt_fit(vec3 v)
{
    vec3 a = v * (v + 0.0245786) - 0.000090537;
    vec3 b = v * (0.983729 * v + 0.4329510) + 0.238081;
    return a / b;
}

vec3 aces_fitted(vec3 v)
{
    v = aces_input_matrix*v;
    v = rtt_and_odt_fit(v);
    return aces_output_matrix*v;
}

void main()
{
        const float gamma = 2.2;
        vec3 hdrColor = texture(hdrBuffer, TexCoords).rgb;

        // reinhard
        vec3 reinhard = hdrColor / (hdrColor + vec3(1.0));
        
        // aces
        vec3 aces = aces_fitted(hdrColor);
        vec3 finalResult = isACES ==1? aces : reinhard;
        
        // gamma correction
        finalResult = pow(finalResult, vec3(1.0 / gamma));
        FragColor = vec4(finalResult, 1.0);
        //FragColor = texture(hdrBuffer, TexCoords);
        //FragColor = vec4(1.0,0.0,0.0,1.0);
}
