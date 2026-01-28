#version 330 core
layout (location = 0) in vec3 aPos;     
layout (location = 1) in vec2 aTexCoord; 
layout (location = 2) in vec3 aNormal;

out vec3 FragPos;
out vec3 Normal;
out vec3 LightPos;
out vec2 TexCoords;       
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
    LightPos = lightPos; 

    FragPos = vec3(model * vec4(aPos, 1.0));
    Normal = mat3(transpose(inverse(model))) * aNormal;  
    
    TexCoords = aTexCoord; 
    
    FragPosLightSpace = lightSpaceMatrix * vec4(FragPos, 1.0);
    
    gl_Position = projection * view * vec4(FragPos, 1.0);
}