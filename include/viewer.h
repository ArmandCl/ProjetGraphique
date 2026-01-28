#ifndef VIEWER_H
#define VIEWER_H

#include <vector>
#include <string>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "shader.h"
#include "node.h"

class Viewer {
public:
    Viewer(int width = 1280, int height = 720);
    ~Viewer();

    std::vector<Node*> fan_nodes;
    // Les positions exactes des fans dans Blender (à noter manuellement !)
    std::vector<glm::vec3> fan_centers = {
        glm::vec3(0.0f, -1.11f, -2.9f), // fan3
        glm::vec3(0.0f, 1.8f, -2.9f), // fan1
        glm::vec3(0.0f, 0.34f, -2.9f) // fan2
    };

    void run();

    Node* scene_root;

private:
    // callback statiques
    static void key_callback_static(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void mouse_callback_static(GLFWwindow* window, double xpos, double ypos);
    static void scroll_callback_static(GLFWwindow* window, double xoffset, double yoffset);
    static void framebuffer_size_callback_static(GLFWwindow* window, int width, int height);

    // instance
    void on_key(int key, int action);
    void on_mouse(double xpos, double ypos);
    void on_scroll(double xoffset, double yoffset);
    void on_framebuffer_size(int width, int height);
    void process_movement();
    void update_camera_vectors();

    // Variables membres
    GLFWwindow* win;
    int window_width;
    int window_height;
    GLuint depthMapFBO;
    GLuint depthMap;
    const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
    Shader* depthShader;

    // Variables de cam
    glm::vec3 camera_position;
    glm::vec3 camera_front;
    glm::vec3 camera_up;
    glm::vec3 camera_right;
    glm::vec3 world_up;

    // Variables de mouvement
    float camera_speed;
    float mouse_sensitivity;
    float zoom;

    // Angles d'Euler
    float yaw;
    float pitch;

    // etat des touches
    bool keys[1024];

    // Souris
    bool first_mouse;
    float last_x;
    float last_y;

    // Timing
    float delta_time;
    float last_frame;

    bool is_free_camera = false; // Par défaut, elle tourne
    float orbit_radius = 3.0f;
    float orbit_speed = 0.5f;
    float orbit_height = 1.0f;
};

#endif // VIEWER_H