#version 330 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec3 LightPos;
in vec2 TexCoords;
<<<<<<< Updated upstream
in vec4 FragPosLightSpace; 

uniform vec3 lightColor;
uniform sampler2D diffuse_map;
uniform sampler2D shadowMap; 
=======
in vec4 FragPosLightSpace;

uniform vec3 lightColor;
uniform sampler2D diffuse_map;
uniform sampler2D shadowMap;
>>>>>>> Stashed changes

float ShadowCalculation(vec4 fragPosLightSpace)
{
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
<<<<<<< Updated upstream
    projCoords = projCoords * 0.5 + 0.5;
    
    if(projCoords.z > 1.0)
        return 0.0;

    float closestDepth = texture(shadowMap, projCoords.xy).r; 
    float currentDepth = projCoords.z;
    
    float bias = 0.005;
=======
    
    // 2. Transformer l'intervalle [-1,1] en [0,1]
    projCoords = projCoords * 0.5 + 0.5;
    
    // Si on est en dehors de la carte d'ombre (derrière la lumière), pas d'ombre
    if(projCoords.z > 1.0)
        return 0.0;

    // 3. Lire la profondeur enregistrée dans la texture
    float closestDepth = texture(shadowMap, projCoords.xy).r; 
    
    // 4. Profondeur actuelle du pixel
    float currentDepth = projCoords.z;
    
    // 5. Biais pour éviter l'acné d'ombre (Rayures noires)
    // Avec le Front Face Culling activé dans viewer.cpp, un petit biais suffit
    float bias = 0.001;
    
    // Comparaison
>>>>>>> Stashed changes
    float shadow = currentDepth - bias > closestDepth ? 1.0 : 0.0;

    return shadow;
}

void main()
{
    // Paramètres de lumière
    float ambientIntensity = 0.2;
    float diffuseIntensity = 0.8;

<<<<<<< Updated upstream
    // Ambiant (TOUCHE PAS A CA JE GERE CETTE PARTIE SINON CA CASSE TOUT)
=======
    // 2. Ambiant
>>>>>>> Stashed changes
    vec3 ambient = ambientIntensity * lightColor;

    // Diffus
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(LightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * diffuseIntensity * lightColor;

    // Texture
    vec3 texColor = texture(diffuse_map, TexCoords).rgb;
    
<<<<<<< Updated upstream
    float shadow = ShadowCalculation(FragPosLightSpace);

    // Formule : (Ambiant + (1.0 - Ombre) * Diffus) * CouleurTexture
=======
    // 5. Calcul de l'ombre
    float shadow = ShadowCalculation(FragPosLightSpace);

    // 6. Combinaison finale
    // L'ombre n'affecte que le diffus (et le spéculaire si tu en as), pas l'ambiant
>>>>>>> Stashed changes
    vec3 result = (ambient + (1.0 - shadow) * diffuse) * texColor;
    
    FragColor = vec4(result, 1.0);
}