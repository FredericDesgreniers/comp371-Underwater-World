#pragma once

#include <GLM\gtc\matrix_transform.hpp>
#include <GLM\gtc\type_ptr.hpp>
#include <random>

#include "Coral.h"

Coral::Coral(glm::vec3 position) : position(position)
{
    // Random devices and distributions
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> randLength(0.5, 1.5);
    std::uniform_real_distribution<> randWidth(1.0, 3.0);
    std::uniform_real_distribution<> u(0.0, 1.0);
    std::uniform_real_distribution<> v(-0.2, 0.2);
    
    float wc = randWidth(gen) * 0.5;
    float lc = wc*5.0f*randLength(gen);
    
    // Base triangle
    glm::vec3 v0 = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 v1 = glm::vec3(wc, 0.0f, 0.0f);
    glm::vec3 v2 = glm::vec3(wc/2, 0.0f, wc * sin(glm::radians(60.0f)));
    
    glm::vec3 normal = glm::normalize(glm::cross(v1 - v0, v2 - v0));
    
    //Push base triangle
    pushTriangle(v0, v1, v2, normal);
    
    tree(v0, v1, v2, 4, lc, wc);
    
    loadBuffers();
    
    model = glm::translate(glm::mat4(1.0f), -position);
    
    oscOffset = u(gen)*3.14159265;
    
    float baseColor = v(gen);
    glm::vec3 color = glm::vec3(u(gen) + baseColor, u(gen) + baseColor, u(gen) + baseColor);
    
    // Assign material 
    material = Material(glm::vec3(0.25f), color, glm::vec3(0.25f), 0.4f);
    
    
}

void Coral::render(Shader * shader)
{
    //shader->use();
    glm::mat3 normalMatrix = glm::transpose(glm::inverse(model));
    
    // Broadcast the uniform values to the shaders
    GLuint modelLoc = glGetUniformLocation(shader->program, "model");
    GLint matAmbientLoc = glGetUniformLocation(shader->program, "material.ambient");
    GLint matDiffuseLoc = glGetUniformLocation(shader->program, "material.diffuse");
    GLint matSpecularLoc = glGetUniformLocation(shader->program, "material.specular");
    GLint matShineLoc = glGetUniformLocation(shader->program, "material.shininess");
    GLint normalMatrixLoc = glGetUniformLocation(shader->program, "normalMatrix");
    //GLuint viewLoc = glGetUniformLocation(shader->program, "view");
    //GLuint projectionLoc = glGetUniformLocation(shader->program, "projection");
    
    
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniform3f(matAmbientLoc, material.ambient.x, material.ambient.y, material.ambient.z);
    glUniform3f(matDiffuseLoc, material.diffuse.x, material.diffuse.y, material.diffuse.z);
    glUniform3f(matSpecularLoc, material.specular.x, material.specular.y, material.specular.z);
    glUniform1f(matShineLoc, material.shininess);
    glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));
    //glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    //glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
    
    // Draw object
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, vertices.size());
    glBindVertexArray(0);
}

void Coral::animate(float deltaTime)
{
    totalTime += deltaTime;
    
    // Model transformations
    glm::mat4 tempModel = glm::translate(glm::mat4(1.0f), -position);
    
    float sx = sin(totalTime + oscOffset) / 25.0f;
    float sz = sin(totalTime+ oscOffset + 1.584) / 25.153f;
    glm::mat4 shearMatrix = {
        1.0f, sx, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, sz, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    };
    tempModel = tempModel * shearMatrix;
    
    model = tempModel;
}

void Coral::tree(glm::vec3 v0, glm::vec3 v1, glm::vec3 v2, int r, float lc, float wc)
{
    // Random devices and distributions
    std::random_device rd;
    std::mt19937 gen(rd());
    std::poisson_distribution<> p(10);
    std::uniform_real_distribution<> u(0.5, 0.9);
    std::uniform_real_distribution<> u3(0.1, 0.3);
    std::uniform_real_distribution<> u1(0.0, 1.0);
    std::uniform_real_distribution<> u2(5.0, 15.0);
    
    float lv = u(gen);
    float wv = u3(gen);
    
    
    glm::vec3 centroid = glm::vec3((v0.x + v1.x + v2.x) / 3, (v0.y + v1.y + v2.y) / 3, (v0.z + v1.z + v2.z) / 3);
    glm::vec3 tnormal = glm::normalize(glm::cross(v2 - v0, v1 - v0));
    
    glm::vec3 v3 = v0 + lc * tnormal;
    glm::vec3 v4 = v1 + lc * tnormal;
    glm::vec3 v5 = v2 + lc * tnormal;
    glm::vec3 c2 = glm::vec3((v3.x + v4.x + v5.x) / 3, (v3.y + v4.y + v5.y) / 3, (v3.z + v4.z + v5.z) / 3);
    
    //Reduce width
    v3 = v3 + wv * (c2 - v3);
    v4 = v4 + wv * (c2 - v4);
    v5 = v5 + wv * (c2 - v5);
    
    //Side face 1
    glm::vec3 n1 = glm::normalize(glm::cross(v4 - v0, v1 - v0));
    pushTriangle(v0, v1, v4, n1);
    pushTriangle(v0, v4, v3, n1);
    
    //Side face 2
    glm::vec3 n2 = glm::normalize(glm::cross(v5 - v1, v2 - v1));
    pushTriangle(v1, v2, v5, n2);
    pushTriangle(v1, v5, v4, n2);
    
    //Side face 3
    glm::vec3 n3 = glm::normalize(glm::cross(v3 - v2, v0 - v2));
    pushTriangle(v2, v0, v3, n3);
    pushTriangle(v2, v3, v5, n3);
    
    float edgeLength = glm::distance(v4, v5);
    
    //Cap vertex
    glm::vec3 v6 = c2 + (edgeLength * float(sqrt(6)) / 3) * tnormal;
    
    //Cap face 1
    glm::vec3 n4 = glm::normalize(glm::cross(v6 - v3, v4 - v3));
    pushTriangle(v3, v4, v6, n4);
    
    //Cap face 2
    glm::vec3 n5 = glm::normalize(glm::cross(v6 - v4, v5 - v4));
    pushTriangle(v4, v5, v6, n5);
    
    //Cap face 3
    glm::vec3 n6 = glm::normalize(glm::cross(v6 - v5, v3 - v5));
    pushTriangle(v5, v3, v6, n6);
    
    
    
    if (r > 0)
    {
        if (u1(gen) < 0.9f)
            tree(v3, v4, v6, r - 1, lc * u(gen), edgeLength);
        if (u1(gen) < 0.9f)
            tree(v4, v5, v6, r - 1, lc * u(gen), edgeLength);
        if (u1(gen) < 0.9f)
            tree(v5, v3, v6, r - 1, lc * u(gen), edgeLength);
        if (u1(gen) < 0.9f)
            tree(v3, v4, v5, r - 1, lc * u(gen), edgeLength);
    }
    
}



void Coral::pushTriangle(glm::vec3 v0, glm::vec3 v1, glm::vec3 v2, glm::vec3 n)
{
    vertices.push_back(v0);
    vertices.push_back(v1);
    vertices.push_back(v2);
    normals.push_back(n);
    normals.push_back(n);
    normals.push_back(n);
}

bool Coral::load()
{
    if(VAO == 0)
    {
        // Generate buffers
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        //glGenBuffers(1, &EBO);
        
        // Buffer object data
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * (sizeof(GLfloat) * 6), data, GL_STATIC_DRAW);
        
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
        glEnableVertexAttribArray(1);
        
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
        
        true;
        
    }
    return false;
}

void Coral::unload()
{
    
    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);
    
    VBO = 0;
    VAO = 0;
}

void Coral::loadBuffers()
{
    
    data = new GLfloat[vertices.size() * 6];
    
    for (int i = 0; i < vertices.size(); i++)
    {
        data[(i * 6)] = vertices[i].x;
        data[(i * 6) + 1] = vertices[i].y;
        data[(i * 6) + 2] = vertices[i].z;
        data[(i * 6) + 3] = normals[i].x;
        data[(i * 6) + 4] = normals[i].y;
        data[(i * 6) + 5] = normals[i].z;
    }
}
