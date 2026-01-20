#include "viewer.h"
#include "textured_sphere.h"
#include "lighting_sphere.h"
#include "texture.h"
#include "node.h"
#include "shader.h"
#include "rectangle.h"
#include "cylinder.h" 
#include <string>

#ifndef SHADER_DIR
#error "SHADER_DIR not defined"
#endif

int main()
{
    Viewer viewer;
    std::string shader_dir = SHADER_DIR;

    // Shader pour les objets texturés
    Shader* texture_shader = new Shader(shader_dir + "texture.vert", shader_dir + "texture.frag");

    // Texture bois
    Texture* wood_texture = new Texture("C:\\Users\\Armand\\Source\\Repos\\ArmandCl\\ProjetGraphique\\textures\\wood.jpg");

    // Texture métal pour les pieds (optionnel)
    Texture* metal_texture = new Texture("C:\\Users\\Armand\\Source\\Repos\\ArmandCl\\ProjetGraphique\\textures\\metal.jpg"); 
    // === SPHERE AVEC LUMIÈRE ===
    Shader* phong_shader = new Shader(shader_dir + "phong.vert", shader_dir + "phong.frag");
    Shape* sphere = new LightingSphere(phong_shader,
        glm::vec3(2.0f, 2.0f, 2.0f),  // Position lumière
        glm::vec3(1.0f, 1.0f, 1.0f),  // Couleur lumière
        glm::vec3(0.2f, 0.5f, 0.8f)); // Couleur objet

    glm::mat4 sphere_mat = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 1.0f, -4.0f))
        * glm::scale(glm::mat4(1.0f), glm::vec3(0.5f, 0.5f, 0.5f));

    Node* sphere_node = new Node(sphere_mat);
    sphere_node->add(sphere);
    viewer.scene_root->add(sphere_node);

    // === BUREAU ===
    Shape* desk_top = new Rectangle(texture_shader, wood_texture, 2.0f, 1.0f, 0.1f);
    glm::mat4 desk_mat = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -0.8f, -3.0f))
        * glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

    Node* desk_node = new Node(desk_mat);
    desk_node->add(desk_top);
    viewer.scene_root->add(desk_node);

    // === PIEDS DE BUREAU (CYLINDRES TEXTURÉS) ===
    float leg_height = 0.7f;
    float leg_radius = 0.05f;
    int slices = 32;

    // Pieds avec texture bois (ou métal si vous préférez)
    Texture* leg_texture = metal_texture; // Ou metal_texture

    // Créer les 4 pieds
    std::vector<glm::vec3> leg_positions = {
        glm::vec3(-0.85f, -1.15f, -2.65f),  // Avant gauche
        glm::vec3(0.85f, -1.15f, -2.65f),   // Avant droit
        glm::vec3(-0.85f, -1.15f, -3.35f),  // Arrière gauche
        glm::vec3(0.85f, -1.15f, -3.35f)    // Arrière droit
    };

    for (const auto& pos : leg_positions) {
        // Cylindre avec texture
        Shape* leg = new Cylinder(texture_shader, leg_height, leg_radius, slices, leg_texture);

        glm::mat4 leg_mat = glm::translate(glm::mat4(1.0f), pos)
            * glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

        Node* leg_node = new Node(leg_mat);
        leg_node->add(leg);
        viewer.scene_root->add(leg_node);
    }

    viewer.run();

    // Nettoyage
    delete texture_shader;
    delete phong_shader;
    delete wood_texture;
    delete metal_texture; // Si créé

    return 0;
}