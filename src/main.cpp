#include "viewer.h"
#include "textured_sphere.h"
#include "lighting_sphere.h"
#include "texture.h"
#include "node.h"
#include "shader.h"
#include "rectangle.h"
#include "cylinder.h"
#include "room.h"
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

    // Textures pour la pièce
    // NOTE: Tu dois créer ces fichiers ou utiliser des textures existantes
    Texture* floor_texture = new Texture("C:\\Users\\Armand\\Source\\Repos\\ArmandCl\\ProjetGraphique\\textures\\floor.jpg");
    Texture* wall_texture = new Texture("C:\\Users\\Armand\\Source\\Repos\\ArmandCl\\ProjetGraphique\\textures\\wall.jpg");  // Utilise wood.jpg temporairement
    Texture* ceiling_texture = new Texture("C:\\Users\\Armand\\Source\\Repos\\ArmandCl\\ProjetGraphique\\textures\\wood.jpg"); // Utilise wood.jpg temporairement
    Texture* wood_texture = new Texture("C:\\Users\\Armand\\Source\\Repos\\ArmandCl\\ProjetGraphique\\textures\\wood.jpg");     // Texture bois pour le bureau

    // Texture métal pour les pieds
    Texture* metal_texture = new Texture("C:\\Users\\Armand\\Source\\Repos\\ArmandCl\\ProjetGraphique\\textures\\metal.jpg");

    // === PIÈCE COMPLÈTE ===
    Shape* room = new Room(texture_shader,
        floor_texture,
        wall_texture,
        ceiling_texture,
        5.0f,  // largeur
        4.0f,  // hauteur
        8.0f); // profondeur

    // La pièce est centrée à l'origine
    glm::mat4 room_mat = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -3.0f));
    Node* room_node = new Node(room_mat);
    room_node->add(room);
    viewer.scene_root->add(room_node);

    // === SPHÈRE AVEC LUMIÈRE ===
    Shader* phong_shader = new Shader(shader_dir + "phong.vert", shader_dir + "phong.frag");
    Shape* sphere = new LightingSphere(phong_shader,
        glm::vec3(0.0f, 2.0f, 2.0f),  // Position lumière (dans la pièce)
        glm::vec3(1.0f, 1.0f, 1.0f),  // Couleur lumière
        glm::vec3(0.2f, 0.5f, 0.8f)); // Couleur objet

    glm::mat4 sphere_mat = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.5f, -3.0f))
        * glm::scale(glm::mat4(1.0f), glm::vec3(0.3f, 0.3f, 0.3f));

    Node* sphere_node = new Node(sphere_mat);
    sphere_node->add(sphere);
    viewer.scene_root->add(sphere_node);

    // === BUREAU (dans la pièce) ===
    Shape* desk_top = new Rectangle(texture_shader, wood_texture, 2.0f, 1.0f, 0.1f);
    glm::mat4 desk_mat = glm::translate(glm::mat4(1.0f), glm::vec3(-1.5f, -1.4f, -6.499f))
        * glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

    Node* desk_node = new Node(desk_mat);
    desk_node->add(desk_top);
    viewer.scene_root->add(desk_node);

    // === PIEDS DE BUREAU ===
    float leg_height = 0.7f;
    float leg_radius = 0.05f;
    int slices = 32;

    // Positions des pieds (ajustées pour que le bureau touche le sol)
    std::vector<glm::vec3> leg_positions = {
        // Pieds avant (côté centre de la pièce) : Z = position_bureau + petit offset
        glm::vec3(-2.35f, -1.75f, -6.1f),   // Avant gauche (Z = -6.5 + 0.1)

        glm::vec3(-0.65f, -1.75f, -6.1f),   // Avant droit (Z = -6.5 + 0.1)

        // Pieds arrière (côté mur) : Z = position_bureau - plus grand offset
        glm::vec3(-2.35f, -1.75f, -6.9f),   // Arrière gauche (Z = -6.5 - 0.4) - proche du mur

        glm::vec3(-0.65f, -1.75f, -6.9f)    // Arrière droit (Z = -6.5 - 0.4) - proche du mur
    };

    for (const auto& pos : leg_positions) {
        Shape* leg = new Cylinder(texture_shader, leg_height, leg_radius, slices, metal_texture);

        glm::mat4 leg_mat = glm::translate(glm::mat4(1.0f), pos)
            * glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

        Node* leg_node = new Node(leg_mat);
        leg_node->add(leg);
        viewer.scene_root->add(leg_node);
    }


    // === AFFICHE SUR LE MUR ===
    // Crée une texture pour l'affiche
    Texture* poster_texture = new Texture("C:\\Users\\Armand\\Source\\Repos\\ArmandCl\\ProjetGraphique\\textures\\poster.jpg");

    // Crée un rectangle mince pour l'affiche (comme un tableau/cadre)
    Shape* poster = new Rectangle(texture_shader, poster_texture, 1.0f, 1.5f, 0.02f); // Largeur 1.5, hauteur 2.0, épaisseur 0.02

    // Position sur le mur droit (X = +4.0 pour être sur le mur, Y à hauteur des yeux, Z au milieu)
    glm::mat4 poster_mat = glm::translate(glm::mat4(1.0f), glm::vec3(3.95f, 0.0f, -3.0f))  // Presque collé au mur droit
        * glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f)) // Tourné pour faire face à l'intérieur
        *glm::rotate(glm::mat4(1.0f), glm::radians(180.0f), glm::vec3(1.0f, 0.0f, 0.0f)); // Rotation supplémentaire de 180° sur X


    Node* poster_node = new Node(poster_mat);
    poster_node->add(poster);
    viewer.scene_root->add(poster_node);

    viewer.run();

    // Nettoyage
    delete texture_shader;
    delete phong_shader;
    delete wood_texture;
    delete metal_texture;
    delete floor_texture;
    delete wall_texture;
    delete ceiling_texture;
    delete poster_texture;

    return 0;
}
