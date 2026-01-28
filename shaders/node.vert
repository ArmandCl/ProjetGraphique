#version 330 core

in vec3 position;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 fragColor;

void main() {

    fragColor = position;
    gl_Position = projection * view * model * vec4(position, 1);
}
