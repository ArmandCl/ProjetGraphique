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

    void run();

    Node* scene_root;

private:
    // M�thodes de callback statiques
    static void key_callback_static(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void mouse_callback_static(GLFWwindow* window, double xpos, double ypos);
    static void scroll_callback_static(GLFWwindow* window, double xoffset, double yoffset);
    static void framebuffer_size_callback_static(GLFWwindow* window, int width, int height);

    // M�thodes d'instance
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

    // Variables de cam�ra
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

    // �tat des touches
    bool keys[1024];

    // Souris
    bool first_mouse;
    float last_x;
    float last_y;

    // Timing
    float delta_time;
    float last_frame;
};

#endif // VIEWER_H