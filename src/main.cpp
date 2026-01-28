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
    glEnable(GL_BLEND); // pour la transparence de la vitre
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    Viewer viewer;
    std::string shader_dir = SHADER_DIR;

    Shader* texture_shader = new Shader(shader_dir + "texture.vert", shader_dir + "texture.frag");
    Shader* phong_color_shader = new Shader(shader_dir + "phong.vert", shader_dir + "phong_color.frag");
    Shader* phong_texture_shader = new Shader(shader_dir + "phong.vert", shader_dir + "phong.frag");

    Texture* floor_texture = new Texture("textures\\floor.jpg");
    glBindTexture(GL_TEXTURE_2D, floor_texture->getGLid());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glBindTexture(GL_TEXTURE_2D, 0);
    Texture* wall_texture = new Texture("textures\\wall.jpg");
    glBindTexture(GL_TEXTURE_2D, wall_texture->getGLid());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glBindTexture(GL_TEXTURE_2D, 0);
    Texture* ceiling_texture = new Texture("textures\\wood.jpg");
    Texture* wood_texture = new Texture("textures\\wood.jpg");
    Texture* computer_texture = new Texture("textures\\black.jpg");
    Texture* lit_texture = new Texture("textures\\Lit_Atlas.png");
    Texture* framed_texture = new Texture("textures\\csgo_screen.png");

    Texture* metal_texture = new Texture("textures\\metal.jpg");

    float scale_factor = 0.35f;
    float room_width = 5.0f * scale_factor;
    float room_height = 4.0f * scale_factor;
    float room_depth = 5.0f * scale_factor;
    float wall_thickness = 0.15f;

    Shape* room = new Room(phong_texture_shader,
        floor_texture,
        wall_texture,
        ceiling_texture,
        room_width,
        room_height,
        room_depth,
        wall_thickness);

    glm::mat4 room_mat = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -0.875f));
    Node* room_node = new Node(room_mat);
    room_node->add(room);
    viewer.scene_root->add(room_node);

    float hd = room_depth * 0.5f;
    float win_w = (room_width * 0.35f); 
    float win_h = (room_height * 0.45f);
    float x_offset_trou = room_width * 0.2f;

    Rectangle* glass = new Rectangle(phong_color_shader, win_w, win_h, 0.01f);
    glass->setColor(glm::vec4(0.5f, 0.8f, 1.0f, 0.2f));

    float z_local_vitre = -hd - (wall_thickness * 0.5f) - 0.001f;
    float z_global_final = -0.875f + z_local_vitre; 

    glm::mat4 glass_mat = glm::translate(glm::mat4(1.0f), 
        glm::vec3(x_offset_trou, 0.0f, z_global_final));

    Node* glass_node = new Node(glass_mat);
    glass_node->add(glass);

    Shape* sphere = new LightingSphere(phong_color_shader,
        glm::vec3(0.0f, 0.7f, 0.3f),  // position lumière
        glm::vec3(1.0f, 1.0f, 1.0f),  // couleur lumière
        glm::vec3(1.0f, 1.0f, 1.0f)); // couleur de l'ampoule

    glm::mat4 sphere_mat = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.35f, -0.875f))
        * glm::scale(glm::mat4(1.0f), glm::vec3(0.1f, 0.1f, 0.1f));

    Node* sphere_node = new Node(sphere_mat);
    sphere_node->add(sphere);
    viewer.scene_root->add(sphere_node);

    Shape* desk_top = new Rectangle(phong_texture_shader, wood_texture, 2.0f * scale_factor, 1.0f * scale_factor, 0.1f * scale_factor);
    glm::mat4 desk_mat = glm::translate(glm::mat4(1.0f), glm::vec3(-0.5f, -0.45f, -1.57f))
        * glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

    Node* desk_node = new Node(desk_mat);
    desk_node->add(desk_top);
    viewer.scene_root->add(desk_node);

    float leg_height = 0.65f * scale_factor;
    float leg_radius = 0.05f * scale_factor;
    int slices = 32;

    std::vector<glm::vec3> leg_positions = {
        glm::vec3(-0.75f, -0.58f, -1.45f),
        glm::vec3(-0.2275f, -0.58f, -1.45f),

        glm::vec3(-0.75f, -0.58f, -1.71f),
        glm::vec3(-0.2275f, -0.58f, -1.71f)
    };

    for (const auto& pos : leg_positions) {
        Shape* leg = new Cylinder(texture_shader, leg_height, leg_radius, slices, metal_texture);

        glm::mat4 leg_mat = glm::translate(glm::mat4(1.0f), pos)
            * glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

        Node* leg_node = new Node(leg_mat);
        leg_node->add(leg);
        viewer.scene_root->add(leg_node);
    }

    Texture* poster_texture = new Texture("textures\\csgo_poster.png");

    Shape* poster = new Rectangle(phong_texture_shader, poster_texture, 0.8f * scale_factor, 1.0f * scale_factor, 0.02f * scale_factor);

    glm::mat4 poster_mat = glm::translate(glm::mat4(1.0f), glm::vec3(-0.865f, 0.0f, -0.875f))
        * glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f)); 


    Node* poster_node = new Node(poster_mat);
    poster_node->add(poster);
    viewer.scene_root->add(poster_node);

    Mesh* lamp = new Mesh(phong_texture_shader, "models\\lamp.obj", wood_texture);
    Mesh* chevet = new Mesh(phong_texture_shader, "models\\chevet.obj", wood_texture);

    Mesh* computer = new Mesh(phong_texture_shader, "models\\computer.obj", computer_texture, "Cube");
    Mesh* fan1_computer = new Mesh(phong_texture_shader, "models\\computer.obj", computer_texture, "Cylinder.001");
    Mesh* fan2_computer = new Mesh(phong_texture_shader, "models\\computer.obj", computer_texture, "Cylinder.002");
    Mesh* fan3_computer = new Mesh(phong_texture_shader, "models\\computer.obj", computer_texture, "Cylinder.003");

    Mesh* chair = new Mesh(phong_texture_shader, "models\\chair.obj", computer_texture);

    Mesh* keyboard = new Mesh(phong_texture_shader, "models\\keyboard.obj", computer_texture);
    Mesh* lit = new Mesh(phong_texture_shader, "models\\lit.obj", lit_texture);
    Mesh* monitor = new Mesh(phong_texture_shader, "models\\monitor.obj", computer_texture);
    Mesh* mouse = new Mesh(phong_texture_shader, "models\\mouse.obj", computer_texture);

    //integration lamp
    glm::mat4 mesh_lamp = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.7f, -0.875f))
        * glm::scale(glm::mat4(1.0f), glm::vec3(0.5f, 0.5f, 0.5f));

    Node* mesh_node_lamp = new Node(mesh_lamp);
    mesh_node_lamp->add(lamp);
    viewer.scene_root->add(mesh_node_lamp);

    //integration chevet
    glm::mat4 mesh_chevet = glm::translate(glm::mat4(1.0f), glm::vec3(0.65f, -0.7f, -0.675f))
        * glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0.0f, -1.0f, 0.0f))
        * glm::scale(glm::mat4(1.0f), glm::vec3(0.08f, 0.08f, 0.08f));

    Node* mesh_node_chevet = new Node(mesh_chevet);
    mesh_node_chevet->add(chevet);
    viewer.scene_root->add(mesh_node_chevet);

    //integration chair
    glm::mat4 mesh_chair = glm::translate(glm::mat4(1.0f), glm::vec3(-0.5f, -0.58f, -1.15f))
        * glm::rotate(glm::mat4(1.0f), glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f)) // Rotation 180° pour faire face au bureau
        * glm::scale(glm::mat4(1.0f), glm::vec3(0.01f, 0.01f, 0.01f)); // Échelle ajustée (0.1 était souvent trop gros pour les .obj bruts)

    Node* mesh_node_chair = new Node(mesh_chair);
    mesh_node_chair->add(chair);
    viewer.scene_root->add(mesh_node_chair);

    //integration computer
    glm::mat4 mesh_computer = glm::translate(glm::mat4(1.0f), glm::vec3(-0.78f, -0.34f, -1.58f))
        * glm::scale(glm::mat4(1.0f), glm::vec3(0.04f, 0.04f, 0.04f));

    Node* mesh_node_computer = new Node(mesh_computer);
    mesh_node_computer->add(computer);
    viewer.scene_root->add(mesh_node_computer);

    Node* fan1_node = new Node(glm::mat4(1.0f));
    fan1_node->add(fan1_computer);
    mesh_node_computer->add(fan1_node);

    Node* fan2_node = new Node(glm::mat4(1.0f));
    fan2_node->add(fan2_computer);
    mesh_node_computer->add(fan2_node);

    Node* fan3_node = new Node(glm::mat4(1.0f));
    fan3_node->add(fan3_computer);
    mesh_node_computer->add(fan3_node);

    viewer.fan_nodes.push_back(fan1_node);
    viewer.fan_nodes.push_back(fan2_node);
    viewer.fan_nodes.push_back(fan3_node);

    //integration keyboard
    glm::mat4 mesh_keyboard = glm::translate(glm::mat4(1.0f), glm::vec3(-0.55f, -0.425f, -1.52f))
        * glm::rotate(glm::mat4(1.0f), glm::radians(20.0f), glm::vec3(0.0f, -1.0f, 0.0f))
        * glm::scale(glm::mat4(1.0f), glm::vec3(0.012f, 0.012f, 0.012f));

    Node* mesh_node_keyboard = new Node(mesh_keyboard);
    mesh_node_keyboard->add(keyboard);
    viewer.scene_root->add(mesh_node_keyboard);

    //integration mouse
    glm::mat4 mesh_mouse = glm::translate(glm::mat4(1.0f), glm::vec3(-0.35f, -0.43f, -1.52f))
        * glm::rotate(glm::mat4(1.0f), glm::radians(190.0f), glm::vec3(0.0f, 1.0f, 0.0f))
        * glm::scale(glm::mat4(1.0f), glm::vec3(0.012f, 0.012f, 0.012f));

    Node* mesh_node_mouse = new Node(mesh_mouse);
    mesh_node_mouse->add(mouse);
    viewer.scene_root->add(mesh_node_mouse);
    
    //integration lit
    glm::mat4 mesh_lit = glm::translate(glm::mat4(1.0f), glm::vec3(0.58f, -0.56f, -0.25f))
        * glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0.0f, -1.0f, 0.0f));

    Node* mesh_node_lit = new Node(mesh_lit);
    mesh_node_lit->add(lit);
    viewer.scene_root->add(mesh_node_lit);
    
    //integration ecran
    glm::mat4 mesh_ecran = glm::translate(glm::mat4(1.0f), glm::vec3(-0.5f, -0.3132f, -1.65f)) 
        * glm::rotate(glm::mat4(1.0f), glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f))
        * glm::scale(glm::mat4(1.0f), glm::vec3(0.1f, 0.1f, 0.1f));

    Node* mesh_node_ecran = new Node(mesh_ecran);
    mesh_node_ecran->add(monitor);
    viewer.scene_root->add(mesh_node_ecran);

    float screen_w = 0.31f;
    float screen_h = 0.169f;

    Shape* screen_display = new Rectangle(texture_shader, framed_texture, screen_w, screen_h, 0.0001f);
    glm::mat4 screen_mat = glm::translate(glm::mat4(1.0f), glm::vec3(-0.5f, -0.285f, -1.63f)) 
        * glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 1.0f));

    Node* screen_node = new Node(screen_mat);
    screen_node->add(screen_display);
    viewer.scene_root->add(screen_node);

    glm::vec3 lp(0.0f, 0.7f, -0.875f);
    glm::vec3 lc(1.0f, 1.0f, 1.0f);

    chair->setLight(lp, lc);
    lamp->cast_shadow = false;
    lamp->setLight(lp, lc);
    ((Rectangle*)desk_top)->setLight(lp, lc);
    ((Room*)room)->setLight(lp, lc);
    ((Rectangle*)poster)->setLight(lp, lc);
    lit->setLight(lp, lc);
    computer->setLight(lp, lc);
    monitor->setLight(lp, lc);
    keyboard->setLight(lp, lc);
    mouse->setLight(lp, lc);
    chevet->setLight(lp, lc);
    glass->setLight(lp, lc);

    viewer.scene_root->add(glass_node);

    viewer.run();

    delete texture_shader;
    delete wood_texture;
    delete metal_texture;
    delete floor_texture;
    delete wall_texture;
    delete ceiling_texture;
    delete poster_texture;
    delete framed_texture;
    delete computer_texture;
    delete phong_color_shader;
    delete phong_texture_shader;
    
    return 0;
}