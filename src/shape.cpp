// shape.cpp

#include "shape.h"

Shape::Shape(Shader *shader_program) : shader_program_(shader_program->get_id()) {
    
}

void Shape::drawShadow(Shader* shader, glm::mat4 model) {
    shader->use();
    shader->setMat4("model", model);
    // On assume que le Mesh configure son VAO avant. 
    // ATTENTION : Pour Mesh.cpp, il faudra surcharger cette méthode car Shape ne connait pas vertexCount
}

void Shape::draw(glm::mat4& model, glm::mat4& view, glm::mat4& projection, glm::mat4& lightSpaceMatrix, GLuint shadowMap) {
    
    glUniformMatrix4fv(glGetUniformLocation(shader_program_, "model"), 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(glGetUniformLocation(shader_program_, "view"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(shader_program_, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
    
    // Envoi de la matrice de vue de la lumière
    glUniformMatrix4fv(glGetUniformLocation(shader_program_, "lightSpaceMatrix"), 1, GL_FALSE, glm::value_ptr(lightSpaceMatrix));

    // Envoi de la texture d'ombre
    glActiveTexture(GL_TEXTURE1); // On utilise l'unité de texture 1 (la 0 est pour la texture de l'objet)
    glBindTexture(GL_TEXTURE_2D, shadowMap);
    glUniform1i(glGetUniformLocation(shader_program_, "shadowMap"), 1); // On dit au shader de lire l'unité 1
}