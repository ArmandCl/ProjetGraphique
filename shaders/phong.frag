#version 330 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec3 LightPos;
in vec2 TexCoords;
in vec4 FragPosLightSpace;

uniform vec3 lightColor;
uniform sampler2D diffuse_map;
uniform sampler2D shadowMap;

float ShadowCalculation(vec4 fragPosLightSpace)
{
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;
    
    if(projCoords.z > 1.0) return 0.0;
    
    float currentDepth = projCoords.z;
    float bias = max(0.005 * (1.0 - dot(Normal, normalize(LightPos - FragPos))), 0.0005);
    
    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
    for(int x = -1; x <= 1; ++x) {
        for(int y = -1; y <= 1; ++y) {
            float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r; 
            shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;        
        }    
    }
    shadow /= 9.0;
    
    return shadow;
}

void main()
{
    vec3 texColor = texture(diffuse_map, TexCoords).rgb;
    vec3 norm = normalize(Normal);
    
    // ambiant
    vec3 ambient = 0.2 * lightColor;
    
    // diffus
    vec3 lightDir = normalize(LightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * 0.8 * lightColor;
    
    // ombre
    float shadow = ShadowCalculation(FragPosLightSpace);
    
    vec3 result = (ambient + (1.0 - shadow) * diffuse) * texColor;
    
    FragColor = vec4(result, 1.0);
}