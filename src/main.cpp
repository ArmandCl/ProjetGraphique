#include "viewer.h"
#include "textured_sphere.h"
#include "lighting_sphere.h"
#include "texture.h"
#include "node.h"
#include "shader.h"
#include "rectangle.h"
#include "cylinder.h"
#include "mesh.h"
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
    Texture* floor_texture = new Texture("C:\\Users\\Armand\\Source\\Repos\\ArmandCl\\ProjetGraphique\\textures\\floor.jpg");
    Texture* wall_texture = new Texture("C:\\Users\\Armand\\Source\\Repos\\ArmandCl\\ProjetGraphique\\textures\\wall.jpg");
    Texture* ceiling_texture = new Texture("C:\\Users\\Armand\\Source\\Repos\\ArmandCl\\ProjetGraphique\\textures\\wood.jpg");
    Texture* wood_texture = new Texture("C:\\Users\\Armand\\Source\\Repos\\ArmandCl\\ProjetGraphique\\textures\\wood.jpg");

    // Texture métal pour les pieds
    Texture* metal_texture = new Texture("C:\\Users\\Armand\\Source\\Repos\\ArmandCl\\ProjetGraphique\\textures\\metal.jpg");

    // === PIÈCE COMPLÈTE (plus carrée) ===
    float scale_factor = 0.35f; // Réduction de 65% = 35% de la taille originale
    float room_width = 5.0f * scale_factor;   // largeur: 1.75
    float room_height = 4.0f * scale_factor;  // hauteur: 1.4
    float room_depth = 5.0f * scale_factor;   // profondeur: 1.75 (plus carré!)

    Shape* room = new Room(texture_shader,
        floor_texture,
        wall_texture,
        ceiling_texture,
        room_width,
        room_height,
        room_depth);

    // La pièce est centrée à l'origine
    glm::mat4 room_mat = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -0.875f)); // Centre ajusté
    Node* room_node = new Node(room_mat);
    room_node->add(room);
    viewer.scene_root->add(room_node);

    // === SPHÈRE AVEC LUMIÈRE ===
    Shader* phong_color_shader = new Shader(shader_dir + "phong.vert", shader_dir + "phong_color.frag");
    Shader* phong_texture_shader = new Shader(shader_dir + "phong.vert", shader_dir + "phong.frag");
    // On change le dernier paramètre (object_color) en blanc
    Shape* sphere = new LightingSphere(phong_color_shader,
        glm::vec3(0.0f, 0.7f, 0.3f),  // Position lumière
        glm::vec3(1.0f, 1.0f, 1.0f),  // Couleur lumière
        glm::vec3(1.0f, 1.0f, 1.0f)); // Couleur de l'ampoule (BLANC)

    glm::mat4 sphere_mat = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.35f, -0.875f)) // Ajusté
        * glm::scale(glm::mat4(1.0f), glm::vec3(0.1f, 0.1f, 0.1f)); // Plus petite

    Node* sphere_node = new Node(sphere_mat);
    sphere_node->add(sphere);
    viewer.scene_root->add(sphere_node);

    // === BUREAU (dans la pièce) ===
    Shape* desk_top = new Rectangle(texture_shader, wood_texture, 2.0f * scale_factor, 1.0f * scale_factor, 0.1f * scale_factor);
    // Collé au mur arrière (qui est maintenant à Z = -0.875 - 0.875 = -1.75)
    glm::mat4 desk_mat = glm::translate(glm::mat4(1.0f), glm::vec3(-0.5f, -0.49f, -1.58f)) // Juste devant le mur arrière
        * glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

    Node* desk_node = new Node(desk_mat);
    desk_node->add(desk_top);
    viewer.scene_root->add(desk_node);

    // === PIEDS DE BUREAU ===
    float leg_height = 0.7f * scale_factor;  // 0.245
    float leg_radius = 0.05f * scale_factor; // 0.0175
    int slices = 32;

    std::vector<glm::vec3> leg_positions = {
        // Pieds "avant" (côté centre) - les DEUX du même côté
        glm::vec3(-0.8225f, -0.6125f, -1.45f),   // Gauche avant
        glm::vec3(-0.2275f, -0.6125f, -1.45f),   // Droite avant

        // Pieds "arrière" (côté mur) - mais pas trop loin
        glm::vec3(-0.8225f, -0.6125f, -1.71f),   // Gauche arrière
        glm::vec3(-0.2275f, -0.6125f, -1.71f)    // Droite arrière
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
    Texture* poster_texture = new Texture("C:\\Users\\Armand\\Source\\Repos\\ArmandCl\\ProjetGraphique\\textures\\poster.jpg");

    // Affiche plus petite proportionnellement
    Shape* poster = new Rectangle(texture_shader, poster_texture, 0.8f * scale_factor, 1.0f * scale_factor, 0.02f * scale_factor);


    glm::mat4 poster_mat = glm::translate(glm::mat4(1.0f), glm::vec3(0.865f, 0.0f, -0.875f))  // SUR le mur droit
        * glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f))      // Tourné vers l'intérieur
        * glm::rotate(glm::mat4(1.0f), glm::radians(180.0f), glm::vec3(1.0f, 0.0f, 0.0f));    // Correction verticale

    Node* poster_node = new Node(poster_mat);
    poster_node->add(poster);
    viewer.scene_root->add(poster_node);



    Mesh* monModele = new Mesh(phong_texture_shader, "C:\\Users\\Armand\\Source\\Repos\\ArmandCl\\ProjetGraphique\\models\\lamp.obj", metal_texture);

    // Intégration au graphe de scène
    glm::mat4 mesh_mat = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.7f, -0.875f))
        * glm::scale(glm::mat4(1.0f), glm::vec3(0.5f, 0.5f, 0.5f));

    Node* mesh_node = new Node(mesh_mat);
    mesh_node->add(monModele);
    viewer.scene_root->add(mesh_node);

    GLuint texShaderID = phong_texture_shader->get_id();
    glUseProgram(texShaderID);

    // On envoie la position et la couleur de la lumière (les mêmes que la sphère)
    glUniform3f(glGetUniformLocation(texShaderID, "lightPos"), 0.0f, 0.7f, 0.3f);
    glUniform3f(glGetUniformLocation(texShaderID, "lightColor"), 1.0f, 1.0f, 1.0f);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    viewer.run();

    // Nettoyage
    delete texture_shader;
    delete wood_texture;
    delete metal_texture;
    delete floor_texture;
    delete wall_texture;
    delete ceiling_texture;
    delete poster_texture;

    return 0;
}
