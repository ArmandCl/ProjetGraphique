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
    
    if(projCoords.z > 1.0)
        return 0.0;

    float closestDepth = texture(shadowMap, projCoords.xy).r; 
    float currentDepth = projCoords.z;
    
    float bias = 0.005;
    float shadow = currentDepth - bias > closestDepth ? 1.0 : 0.0;

    return shadow;
}

void main()
{
    // Paramètres de lumière
    float ambientIntensity = 0.2;
    float diffuseIntensity = 0.8;

    // Ambiant (TOUCHE PAS A CA JE GERE CETTE PARTIE SINON CA CASSE TOUT)
    vec3 ambient = ambientIntensity * lightColor;

    // Diffus
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(LightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * diffuseIntensity * lightColor;

    // Texture
    vec3 texColor = texture(diffuse_map, TexCoords).rgb;
    
    float shadow = ShadowCalculation(FragPosLightSpace);

    // Formule : (Ambiant + (1.0 - Ombre) * Diffus) * CouleurTexture
    vec3 result = (ambient + (1.0 - shadow) * diffuse) * texColor;
    
    FragColor = vec4(result, 1.0);
}