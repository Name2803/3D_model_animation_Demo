#version 330 core
out vec4 FragColor;
in VS_OUT {
    vec3 FragPos;
    vec2 TexCoords;
    vec3 TangentLightPos;
    vec3 TangentViewPos;
    vec3 TangentFragPos;
} fs_in;

uniform sampler2D normalMap;

void main()
{
    // Sample the normal from the normal map
    vec3 normal = texture(normalMap, fs_in.TexCoords).rgb;
    normal = normalize(normal * 2.0 - 1.0); //Transform from 0-1 to -1-1

    //Perform lighting calculations in tangent space
    vec3 lightDir = normalize(TangentLightPos - TangentFragPos);
    vec3 viewDir = normalize(TangentViewPos - TangentFragPos);
    vec3 result = calculateLighting(normal, lightDir, viewDir);

    FragColor = vec4(result, 1.0);
}