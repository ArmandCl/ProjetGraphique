#include "viewer.h"

#include <iostream>
#include <glm/glm.hpp>
#include "glm/ext.hpp"
#include <glm/gtc/matrix_transform.hpp>

bool keys[1024] = { false };

Viewer::Viewer(int width, int height)
    : window_width(width), window_height(height),
    camera_position(0.0f, 0.0f, 0.0f),
    camera_front(0.0f, 0.0f, -1.0f),
    world_up(0.0f, 1.0f, 0.0f),
    camera_up(0.0f, 1.0f, 0.0f),
    camera_speed(2.5f),
    mouse_sensitivity(0.1f),
    zoom(45.0f),
    yaw(-90.0f),
    pitch(0.0f),
    first_mouse(true),
    last_x(width / 2.0f),
    last_y(height / 2.0f),
    delta_time(0.0f),
    last_frame(0.0f)
{
    for (int i = 0; i < 1024; i++) {
        keys[i] = false;
    }

    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        glfwTerminate();
    }

    // Version hints
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
    win = glfwCreateWindow(width, height, "Viewer - Camera Libre", NULL, NULL);

    if (win == NULL) {
        std::cerr << "Failed to create window" << std::endl;
        glfwTerminate();
    }

    glfwMakeContextCurrent(win);

    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        glfwTerminate();
    }

    glfwSetWindowUserPointer(win, this);

    glfwSetKeyCallback(win, key_callback_static);
    glfwSetCursorPosCallback(win, mouse_callback_static);
    glfwSetScrollCallback(win, scroll_callback_static);
    glfwSetFramebufferSizeCallback(win, framebuffer_size_callback_static);

    glfwSetInputMode(win, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    std::cout << glGetString(GL_VERSION) << ", GLSL "
        << glGetString(GL_SHADING_LANGUAGE_VERSION) << ", Renderer "
        << glGetString(GL_RENDERER) << std::endl;

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    depthShader = new Shader("shaders/shadow_depth.vert", "shaders/shadow_depth.frag");

    glGenFramebuffers(1, &depthMapFBO);
    glGenTextures(1, &depthMap);
    glBindTexture(GL_TEXTURE_2D, depthMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    float borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    scene_root = new Node();
    update_camera_vectors();
}

Viewer::~Viewer() {
    delete scene_root;
    glfwTerminate();
}

void Viewer::run() {
    while (!glfwWindowShouldClose(win)) {
        float current_frame = glfwGetTime();
        delta_time = current_frame - last_frame;
        last_frame = current_frame;

        float rotation_speed = 13.0f;
        float angle = current_frame * rotation_speed;

        for (size_t i = 0; i < fan_nodes.size(); i++) {
            if (i >= fan_centers.size()) break;
            glm::vec3 center = fan_centers[i];
            glm::mat4 transform = glm::translate(glm::mat4(1.0f), center) 
                                * glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0, 0, 1))
                                * glm::translate(glm::mat4(1.0f), -center);
            fan_nodes[i]->set_transform(transform);
        }

        if (is_free_camera) {
            process_movement();
        } else {
            float time = glfwGetTime() * orbit_speed;
            camera_position.x = sin(time) * orbit_radius;
            camera_position.z = cos(time) * orbit_radius - 0.875f;
            camera_position.y = orbit_height;
            glm::vec3 target = glm::vec3(0.0f, 0.0f, -0.875f);
            camera_front = glm::normalize(target - camera_position);
            camera_right = glm::normalize(glm::cross(camera_front, world_up));
            camera_up = glm::normalize(glm::cross(camera_right, camera_front));
        }

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view = glm::lookAt(camera_position, camera_position + camera_front, camera_up);
        glm::mat4 projection = glm::perspective(glm::radians(zoom), (float)window_width / (float)window_height, 0.1f, 100.0f);

        glm::vec3 lightPos(0.0f, 0.6f, -0.875f); 
        glm::vec3 lightTarget(0.0f, -1.0f, -0.875f); // vise le sol

        float aspect = (float)SHADOW_WIDTH / (float)SHADOW_HEIGHT;
        float near_plane = 0.1f; 
        float far_plane = 10.0f;
        glm::mat4 lightProjection = glm::perspective(glm::radians(80.0f), aspect, near_plane, far_plane);

        // 4. Vue
        glm::mat4 lightView = glm::lookAt(lightPos, lightTarget, glm::vec3(0.0f, 0.0f, 1.0f)); 
        glm::mat4 lightSpaceMatrix = lightProjection * lightView;


        glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
        glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
        glClear(GL_DEPTH_BUFFER_BIT);

        depthShader->use();
        depthShader->setMat4("lightSpaceMatrix", lightSpaceMatrix);
        scene_root->drawShadow(depthShader, glm::mat4(1.0f)); 

        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        glViewport(0, 0, window_width, window_height);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        scene_root->draw(model, view, projection, lightSpaceMatrix, depthMap);

        glfwSwapBuffers(win);
        glfwPollEvents();
    }
}

void Viewer::key_callback_static(GLFWwindow* window, int key, int scancode, int action, int mods) {
    Viewer* viewer = static_cast<Viewer*>(glfwGetWindowUserPointer(window));
    viewer->on_key(key, action);
}
void Viewer::mouse_callback_static(GLFWwindow* window, double xpos, double ypos) {
    Viewer* viewer = static_cast<Viewer*>(glfwGetWindowUserPointer(window));
    viewer->on_mouse(xpos, ypos);
}
void Viewer::scroll_callback_static(GLFWwindow* window, double xoffset, double yoffset) {
    Viewer* viewer = static_cast<Viewer*>(glfwGetWindowUserPointer(window));
    viewer->on_scroll(xoffset, yoffset);
}
void Viewer::framebuffer_size_callback_static(GLFWwindow* window, int width, int height) {
    Viewer* viewer = static_cast<Viewer*>(glfwGetWindowUserPointer(window));
    viewer->on_framebuffer_size(width, height);
}

void Viewer::on_key(int key, int action) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(win, GLFW_TRUE);

    if (key == GLFW_KEY_C && action == GLFW_PRESS) {
        is_free_camera = !is_free_camera;

        if (is_free_camera) {
            pitch = glm::degrees(asin(camera_front.y));

            yaw = glm::degrees(atan2(camera_front.z, camera_front.x));

            glfwSetInputMode(win, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            first_mouse = true; 
        } else {
            glfwSetInputMode(win, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }
    }

    if (key >= 0 && key < 1024) {
        if (action == GLFW_PRESS) keys[key] = true;
        else if (action == GLFW_RELEASE) keys[key] = false;
    }
}

void Viewer::on_mouse(double xpos, double ypos) {
    if (!is_free_camera) return;
    if (first_mouse) { last_x = xpos; last_y = ypos; first_mouse = false; }
    float xoffset = xpos - last_x;
    float yoffset = last_y - ypos; 
    last_x = xpos; last_y = ypos;
    xoffset *= mouse_sensitivity; yoffset *= mouse_sensitivity;
    yaw += xoffset; pitch += yoffset;
    if (pitch > 89.0f) pitch = 89.0f;
    if (pitch < -89.0f) pitch = -89.0f;
    update_camera_vectors();
}

void Viewer::on_scroll(double xoffset, double yoffset) {
    zoom -= (float)yoffset;
    if (zoom < 1.0f) zoom = 1.0f;
    if (zoom > 45.0f) zoom = 45.0f;
}

void Viewer::on_framebuffer_size(int width, int height) {
    window_width = width; window_height = height;
    glViewport(0, 0, width, height);
}

void Viewer::process_movement() {
    float velocity = camera_speed * delta_time;
    if (keys[GLFW_KEY_W]) camera_position += camera_front * velocity;
    if (keys[GLFW_KEY_S]) camera_position -= camera_front * velocity;
    if (keys[GLFW_KEY_A]) camera_position -= camera_right * velocity;
    if (keys[GLFW_KEY_D]) camera_position += camera_right * velocity;
    if (keys[GLFW_KEY_SPACE]) camera_position += world_up * velocity;
    if (keys[GLFW_KEY_LEFT_CONTROL]) camera_position -= world_up * velocity;
    if (keys[GLFW_KEY_LEFT_SHIFT]) velocity *= 2.0f;
}

void Viewer::update_camera_vectors() {
    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    camera_front = glm::normalize(front);
    camera_right = glm::normalize(glm::cross(camera_front, world_up));
    camera_up = glm::normalize(glm::cross(camera_right, camera_front));
}