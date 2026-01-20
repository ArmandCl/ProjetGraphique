#include "viewer.h"
#include "textured_sphere.h"
#include "lighting_sphere.h"
#include "texture.h"
#include "node.h"
#include "shader.h"
#include "rectangle.h"  // Ajouter cet include
#include <string>

#ifndef SHADER_DIR
#error "SHADER_DIR not defined"
#endif

int main()
{
    // create window, add shaders & scene objects, then run rendering loop
    Viewer viewer;

    // get shader directory
    std::string shader_dir = SHADER_DIR;

    Shader* texture_shader = new Shader(shader_dir + "texture.vert", shader_dir + "texture.frag");

    // === SPHERE AVEC TEXTURE ===
    Texture* texture = new Texture("C:\\Users\\Arman\\Documents\\Projet_Git\\ProjetGraphique\\textures\\texture1.png");  // Chemin relatif
    Shape* sphere1 = new TexturedSphere(texture_shader, texture);
    glm::mat4 sphere1_mat = glm::translate(glm::mat4(1.0f), glm::vec3(-1.0f, 1.0f, -4.0f))
        * glm::scale(glm::mat4(1.0f), glm::vec3(0.5f, 0.5f, 0.5f));

    Node* sphere1_node = new Node(sphere1_mat);
    sphere1_node->add(sphere1);
    viewer.scene_root->add(sphere1_node);

    // === SPHERE AVEC LUMIÈRE ===
    Shader* phong_shader = new Shader(shader_dir + "phong.vert", shader_dir + "phong.frag");
    Shape* sphere2 = new LightingSphere(phong_shader, glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    glm::mat4 sphere2_mat = glm::translate(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, -4.0f))
        * glm::scale(glm::mat4(1.0f), glm::vec3(0.5f, 0.5f, 0.5f));

    Node* sphere2_node = new Node(sphere2_mat);
    sphere2_node->add(sphere2);
    viewer.scene_root->add(sphere2_node);

    // === BUREAU (RECTANGLE) ===
    // Texture bois pour le bureau
    Texture* wood_texture = new Texture("C:\\Users\\Arman\\Documents\\Projet_Git\\ProjetGraphique\\textures\\wood.jpg");    // ajouter une texture bois

    // Plateau du bureau (2m de large, 1m de profondeur, orienté horizontalement)
    Shape* desk_top = new Rectangle(texture_shader, wood_texture, 2.0f, 1.0f);
    glm::mat4 desk_mat = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -0.8f, -3.0f))
        * glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));  // Rotation pour qu'il soit horizontal

    Node* desk_node = new Node(desk_mat);
    desk_node->add(desk_top);
    viewer.scene_root->add(desk_node);

    // === PIEDS DE BUREAU ===
    // Pied avant gauche
    Shape* leg1 = new Rectangle(texture_shader, wood_texture, 0.1f, 0.7f);  // Pied étroit et haut
    glm::mat4 leg1_mat = glm::translate(glm::mat4(1.0f), glm::vec3(-0.85f, -1.15f, -2.65f))
        * glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

    Node* leg1_node = new Node(leg1_mat);
    leg1_node->add(leg1);
    viewer.scene_root->add(leg1_node);

    // Pied avant droit
    Shape* leg2 = new Rectangle(texture_shader, wood_texture, 0.1f, 0.7f);
    glm::mat4 leg2_mat = glm::translate(glm::mat4(1.0f), glm::vec3(0.85f, -1.15f, -2.65f))
        * glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

    Node* leg2_node = new Node(leg2_mat);
    leg2_node->add(leg2);
    viewer.scene_root->add(leg2_node);

    // Pied arrière gauche
    Shape* leg3 = new Rectangle(texture_shader, wood_texture, 0.1f, 0.7f);
    glm::mat4 leg3_mat = glm::translate(glm::mat4(1.0f), glm::vec3(-0.85f, -1.15f, -3.35f))
        * glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

    Node* leg3_node = new Node(leg3_mat);
    leg3_node->add(leg3);
    viewer.scene_root->add(leg3_node);

    // Pied arrière droit
    Shape* leg4 = new Rectangle(texture_shader, wood_texture, 0.1f, 0.7f);
    glm::mat4 leg4_mat = glm::translate(glm::mat4(1.0f), glm::vec3(0.85f, -1.15f, -3.35f))
        * glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

    Node* leg4_node = new Node(leg4_mat);
    leg4_node->add(leg4);
    viewer.scene_root->add(leg4_node);

    viewer.run();
}