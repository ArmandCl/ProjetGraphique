#version 330 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec3 LightPos;
in vec2 TexCoords;

uniform vec3 lightColor;
uniform sampler2D diffuse_map;

void main()
{
    // 1. Paramètres de lumière (sans le 'f')
    float ambientIntensity = 0.2; 
    float diffuseIntensity = 0.8;

    // 2. Ambiant
    vec3 ambient = ambientIntensity * lightColor;

    // 3. Diffus (Calcul des normales)
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(LightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * diffuseIntensity * lightColor;

    // 4. Texture
    vec3 texColor = texture(diffuse_map, TexCoords).rgb;

    // 5. Combinaison finale
    vec3 result = (ambient + diffuse) * texColor;
    FragColor = vec4(result, 1.0);
}