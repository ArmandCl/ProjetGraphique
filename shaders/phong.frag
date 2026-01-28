#version 330 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec3 LightPos;
in vec2 TexCoords;
in vec4 FragPosLightSpace; // <--- NOUVEAU : Reçu du Vertex Shader

uniform vec3 lightColor;
uniform sampler2D diffuse_map;
uniform sampler2D shadowMap; // <--- NOUVEAU : La texture de profondeur

// --- FONCTION DE CALCUL D'OMBRE ---
float ShadowCalculation(vec4 fragPosLightSpace)
{
    // 1. Division perspective
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // 2. Transformer l'intervalle [-1,1] en [0,1]
    projCoords = projCoords * 0.5 + 0.5;
    
    // Si on est en dehors de la carte d'ombre, on ne fait rien
    if(projCoords.z > 1.0)
        return 0.0;

    // 3. Lire la profondeur enregistrée dans la texture (point le plus proche de la lumière)
    float closestDepth = texture(shadowMap, projCoords.xy).r; 
    // 4. Profondeur actuelle du pixel
    float currentDepth = projCoords.z;
    
    // 5. Comparaison avec un petit biais (bias) pour éviter les rayures noires
    float bias = 0.005;
    float shadow = currentDepth - bias > closestDepth ? 1.0 : 0.0;

    return shadow;
}

void main()
{
    // 1. Paramètres de lumière
    float ambientIntensity = 0.2;
    float diffuseIntensity = 0.8;

    // 2. Ambiant (L'ombre ne touche JAMAIS l'ambiant)
    vec3 ambient = ambientIntensity * lightColor;

    // 3. Diffus
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(LightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * diffuseIntensity * lightColor;

    // 4. Texture
    vec3 texColor = texture(diffuse_map, TexCoords).rgb;
    
    // --- 5. CALCUL DE L'OMBRE ---
    float shadow = ShadowCalculation(FragPosLightSpace);

    // --- 6. COMBINAISON FINALE ---
    // Formule : (Ambiant + (1.0 - Ombre) * Diffus) * CouleurTexture
    vec3 result = (ambient + (1.0 - shadow) * diffuse) * texColor;
    
    FragColor = vec4(result, 1.0);
}