#include "viewer.h"

#include <iostream>
#include <glm/glm.hpp>
#include "glm/ext.hpp"
#include <glm/gtc/matrix_transform.hpp>

// Variables globales pour les callbacks (on utilise le user pointer à la place)
// Mais on garde une référence pour les touches
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
    // Initializez le tableau keys
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

    win = glfwCreateWindow(width, height, "Viewer - Caméra Libre", NULL, NULL);

    if (win == NULL) {
        std::cerr << "Failed to create window" << std::endl;
        glfwTerminate();
    }

    glfwMakeContextCurrent(win);

    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        glfwTerminate();
    }

    // Set user pointer pour accéder à l'instance dans les callbacks
    glfwSetWindowUserPointer(win, this);

    // Enregistrement des callbacks
    glfwSetKeyCallback(win, key_callback_static);
    glfwSetCursorPosCallback(win, mouse_callback_static);
    glfwSetScrollCallback(win, scroll_callback_static);
    glfwSetFramebufferSizeCallback(win, framebuffer_size_callback_static);

    // Cache le curseur et le capture
    glfwSetInputMode(win, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Message d'information
    std::cout << glGetString(GL_VERSION) << ", GLSL "
        << glGetString(GL_SHADING_LANGUAGE_VERSION) << ", Renderer "
        << glGetString(GL_RENDERER) << std::endl;

    // Initialisation OpenGL
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    // Initialisation de la scène
    scene_root = new Node();

    // Initialisation des vecteurs de la caméra
    update_camera_vectors();
}

Viewer::~Viewer() {
    delete scene_root;
    glfwTerminate();
}

void Viewer::run() {
    while (!glfwWindowShouldClose(win)) {
        // Calcul du delta time
        float current_frame = glfwGetTime();
        delta_time = current_frame - last_frame;
        last_frame = current_frame;

        // Traitement des inputs
        process_movement();

        // Nettoyage du buffer
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Matrice modèle
        glm::mat4 model = glm::mat4(1.0f);

        // Matrice vue (caméra)
        glm::mat4 view = glm::lookAt(
            camera_position,
            camera_position + camera_front,
            camera_up
        );

        // Matrice projection
        glm::mat4 projection = glm::perspective(
            glm::radians(zoom),
            (float)window_width / (float)window_height,
            0.1f,
            100.0f
        );

        // Dessin de la scène
        scene_root->draw(model, view, projection);

        // Échange des buffers et poll des événements
        glfwSwapBuffers(win);
        glfwPollEvents();
    }
}

// ============ CALLBACKS STATIQUES ============

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

// ============ MÉTHODES D'INSTANCE ============

void Viewer::on_key(int key, int action) {
    // Quitter avec Échap
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(win, GLFW_TRUE);
    }

    // Libérer/attraper le curseur avec la touche M
    if (key == GLFW_KEY_M && action == GLFW_PRESS) {
        if (glfwGetInputMode(win, GLFW_CURSOR) == GLFW_CURSOR_DISABLED) {
            glfwSetInputMode(win, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            first_mouse = true;
        }
        else {
            glfwSetInputMode(win, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        }
    }

    // Enregistrer l'état des touches
    if (key >= 0 && key < 1024) {
        if (action == GLFW_PRESS) {
            keys[key] = true;
        }
        else if (action == GLFW_RELEASE) {
            keys[key] = false;
        }
    }
}

void Viewer::on_mouse(double xpos, double ypos) {
    if (first_mouse) {
        last_x = xpos;
        last_y = ypos;
        first_mouse = false;
    }

    float xoffset = xpos - last_x;
    float yoffset = last_y - ypos; // Inversé car les coordonnées y vont de bas en haut
    last_x = xpos;
    last_y = ypos;

    xoffset *= mouse_sensitivity;
    yoffset *= mouse_sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    // Limiter l'angle de vue pour éviter le flip
    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    // Mettre à jour les vecteurs de la caméra
    update_camera_vectors();
}

void Viewer::on_scroll(double xoffset, double yoffset) {
    zoom -= (float)yoffset;
    if (zoom < 1.0f)
        zoom = 1.0f;
    if (zoom > 45.0f)
        zoom = 45.0f;
}

void Viewer::on_framebuffer_size(int width, int height) {
    window_width = width;
    window_height = height;
    glViewport(0, 0, width, height);
}

void Viewer::process_movement() {
    float velocity = camera_speed * delta_time;

    if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP])
        camera_position += camera_front * velocity;
    if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN])
        camera_position -= camera_front * velocity;
    if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT])
        camera_position -= camera_right * velocity;
    if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT])
        camera_position += camera_right * velocity;
    if (keys[GLFW_KEY_SPACE])
        camera_position += world_up * velocity;
    if (keys[GLFW_KEY_LEFT_CONTROL] || keys[GLFW_KEY_RIGHT_CONTROL])
        camera_position -= world_up * velocity;

    // Vitesse accrue avec Shift
    if (keys[GLFW_KEY_LEFT_SHIFT] || keys[GLFW_KEY_RIGHT_SHIFT]) {
        velocity *= 2.0f;
    }
}

void Viewer::update_camera_vectors() {
    // Calculer le nouveau vecteur front
    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    camera_front = glm::normalize(front);

    // Recalculer right et up
    camera_right = glm::normalize(glm::cross(camera_front, world_up));
    camera_up = glm::normalize(glm::cross(camera_right, camera_front));
}