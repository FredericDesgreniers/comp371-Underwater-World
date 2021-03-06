#pragma once

#include "Renderable.h"


class Coral : public Renderable
{
    
    public:
    Coral(glm::vec3 position);
    
    void render(Shader* shader);
    void animate(float deltaTime);
    
    std::vector<glm::vec3> normals;
    
    glm::vec3 position;
    
    private:
    
    void tree(glm::vec3 v0, glm::vec3 v1, glm::vec3 v2, int r, float lc, float wc);
    void pushTriangle(glm::vec3 v0, glm::vec3 v1, glm::vec3 v2, glm::vec3 n);
    void loadBuffers();
    
    bool load();
    void unload();
    GLfloat * data;
    
    // Variables for periodic animations
    GLfloat totalTime = 0.0f;
    GLfloat oscOffset;
};
