#version 330 core
layout (location = 0) in vec3 aPos;       // Renommé 'position' en 'aPos' pour correspondre au main
layout (location = 1) in vec2 aTexCoord;  // Renommé 'aTexCoords' en 'aTexCoord' (singulier)
layout (location = 2) in vec3 aNormal;

out vec3 FragPos;
out vec3 Normal;
out vec3 LightPos;
out vec2 TexCoords;       // Gardé au pluriel pour correspondre au Fragment Shader
out vec4 FragPosLightSpace;

uniform vec3 lightPos;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 lightSpaceMatrix;

void main()
{
    // Attention : lightPos est déjà en World Space dans le C++, pas besoin de multiplier par 'view'
    // Si tes ombres sont bizarres, essaie : LightPos = lightPos;
    LightPos = vec3(view * vec4(lightPos, 1.0)); 

    FragPos = vec3(model * vec4(aPos, 1.0));
    Normal = mat3(transpose(inverse(model))) * aNormal;  
    
    // Correction des noms ici :
    TexCoords = aTexCoord; 
    
    // Calcul de la position pour l'ombre
    FragPosLightSpace = lightSpaceMatrix * vec4(FragPos, 1.0);
    
    gl_Position = projection * view * vec4(FragPos, 1.0);
}