#pragma once

#include "Rock.h"
#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.inl>
#include <random>

#define PI 3.14159265358979323846

Shader* Rock::rockShader = nullptr;

glm::vec3 Rock::calculateNormal(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3)
{
	//Edge1, Edge2
	glm::vec3 e1, e2;  
	glm::vec3 normal;

	//Find vectors for two edges sharing V1
	e1 = p2 - p1;
	e2 = p3 - p1;

	//Begin calculating determinant - also used to calculate u parameter
	normal = glm::normalize(glm::cross(e2, e1));

	return normal;
}

Rock::Rock(glm::vec3 position)
{
    // randomize whether colour will be shade of brown or grey
    int colorType = rand() % 10;
    // initialize placeholder for colour float value
    double color;
    
    float const X = 0.525731112119133606f;
    float const Z = 0.850650808352039932f;
    
    std::random_device rd;
    std::mt19937 gen(rd());
    // uniform distribution for the rocks
    std::uniform_real_distribution<> dis(0, 1);
	std::uniform_real_distribution<> nudge(-0.3, 0.3);
    
	rockVertices = {
/*0*/	glm::vec3(-X + nudge(gen), 0 + nudge(gen), Z + nudge(gen)),
/*1*/	glm::vec3(X + nudge(gen), 0 + nudge(gen), Z + nudge(gen)),
/*2*/	glm::vec3(-X + nudge(gen), 0 + nudge(gen), -Z + nudge(gen)),
/*3*/	glm::vec3(X + nudge(gen), 0 + nudge(gen), -Z + nudge(gen)),
/*4*/	glm::vec3(0 + nudge(gen), Z + nudge(gen), X + nudge(gen)),
/*5*/	glm::vec3(0 + nudge(gen), Z + nudge(gen), -X + nudge(gen)),
/*6*/	glm::vec3(0 + nudge(gen), -Z + nudge(gen), X + nudge(gen)),
/*7*/	glm::vec3(0 + nudge(gen), -Z + nudge(gen), -X + nudge(gen)),
/*8*/	glm::vec3(Z + nudge(gen), X + nudge(gen), 0 + nudge(gen)),
/*9*/	glm::vec3(-Z + nudge(gen), X + nudge(gen), 0 + nudge(gen)),
/*10*/	glm::vec3(Z + nudge(gen), -X + nudge(gen), 0 + nudge(gen)),
/*11*/	glm::vec3(-Z + nudge(gen), -X + nudge(gen), 0 + nudge(gen)),
	};

    // brown rock
//  if (colorType >= 7) 
//  {
//      // pick a shade
//      std::uniform_real_distribution<> colorDis(0.15, 0.5);
//      color = colorDis(gen);
//  }
	// grey rock
//  else
//      // pick a shade
//      std::uniform_real_distribution<> colorDis(0.15, 0.4);
//      color = colorDis(gen);
//  }

	glm::vec3 surfaceNormals[20] = {
		calculateNormal(rockVertices[1],rockVertices[4],rockVertices[0]),
		calculateNormal(rockVertices[4],rockVertices[9],rockVertices[0]),
		calculateNormal(rockVertices[4],rockVertices[5],rockVertices[9]),
		calculateNormal(rockVertices[8],rockVertices[5],rockVertices[4]),
		calculateNormal(rockVertices[1],rockVertices[8],rockVertices[4]),
		calculateNormal(rockVertices[1],rockVertices[10],rockVertices[8]),
		calculateNormal(rockVertices[10],rockVertices[3],rockVertices[8]),
		calculateNormal(rockVertices[8],rockVertices[3],rockVertices[5]),
		calculateNormal(rockVertices[3],rockVertices[2],rockVertices[5]),
		calculateNormal(rockVertices[3],rockVertices[7],rockVertices[2]),
		calculateNormal(rockVertices[3],rockVertices[10],rockVertices[7]),
		calculateNormal(rockVertices[10],rockVertices[6],rockVertices[7]),
		calculateNormal(rockVertices[6],rockVertices[11],rockVertices[7]),
		calculateNormal(rockVertices[6],rockVertices[0],rockVertices[11]),
		calculateNormal(rockVertices[6],rockVertices[1],rockVertices[0]),
		calculateNormal(rockVertices[10],rockVertices[1],rockVertices[6]),
		calculateNormal(rockVertices[11],rockVertices[0],rockVertices[9]),
		calculateNormal(rockVertices[2],rockVertices[11],rockVertices[9]),
		calculateNormal(rockVertices[5],rockVertices[2],rockVertices[9]),
		calculateNormal(rockVertices[11],rockVertices[2],rockVertices[7])
	};

	int surface = 0;
	vertices = 
	{
		/*1*/	rockVertices[1], surfaceNormals[surface],
		/*4*/	rockVertices[4], surfaceNormals[surface],
		/*0*/	rockVertices[0], surfaceNormals[surface++],

		/*4*/	rockVertices[4], surfaceNormals[surface],
		/*9*/	rockVertices[9], surfaceNormals[surface],
		/*0*/	rockVertices[0], surfaceNormals[surface++],

		/*4*/	rockVertices[4], surfaceNormals[surface],
		/*5*/	rockVertices[5], surfaceNormals[surface],
		/*9*/	rockVertices[9], surfaceNormals[surface++],

		/*8*/	rockVertices[8], surfaceNormals[surface],
		/*5*/	rockVertices[5], surfaceNormals[surface],
		/*4*/	rockVertices[4], surfaceNormals[surface++],

		/*1*/	rockVertices[1], surfaceNormals[surface],
		/*8*/	rockVertices[8], surfaceNormals[surface],
		/*4*/	rockVertices[4], surfaceNormals[surface++],

		/*1*/	rockVertices[1], surfaceNormals[surface],
		/*10*/	rockVertices[10], surfaceNormals[surface],
		/*8*/	rockVertices[8], surfaceNormals[surface++],

		/*10*/	rockVertices[10], surfaceNormals[surface],
		/*3*/	rockVertices[3], surfaceNormals[surface],
		/*8*/	rockVertices[8], surfaceNormals[surface++],

		/*8*/	rockVertices[8], surfaceNormals[surface],
		/*3*/	rockVertices[3], surfaceNormals[surface],
		/*5*/	rockVertices[5], surfaceNormals[surface++],

		/*3*/	rockVertices[3], surfaceNormals[surface],
		/*2*/	rockVertices[2], surfaceNormals[surface],
		/*5*/	rockVertices[5], surfaceNormals[surface++],

		/*3*/	rockVertices[3], surfaceNormals[surface],
		/*7*/	rockVertices[7], surfaceNormals[surface],
		/*2*/	rockVertices[2], surfaceNormals[surface++],

		/*3*/	rockVertices[3], surfaceNormals[surface],
		/*10*/	rockVertices[10], surfaceNormals[surface],
		/*7*/	rockVertices[7], surfaceNormals[surface++],

		/*10*/	rockVertices[10], surfaceNormals[surface],
		/*6*/	rockVertices[6], surfaceNormals[surface],
		/*7*/	rockVertices[7], surfaceNormals[surface++],

		/*6*/	rockVertices[6], surfaceNormals[surface],
		/*11*/	rockVertices[11], surfaceNormals[surface],
		/*7*/	rockVertices[7], surfaceNormals[surface++],

		/*6*/	rockVertices[6], surfaceNormals[surface],
		/*0*/	rockVertices[0], surfaceNormals[surface],
		/*11*/	rockVertices[11], surfaceNormals[surface++],

		/*6*/	rockVertices[6], surfaceNormals[surface],
		/*1*/	rockVertices[1], surfaceNormals[surface],
		/*0*/	rockVertices[0], surfaceNormals[surface++],

		/*10*/	rockVertices[10], surfaceNormals[surface],
		/*1*/	rockVertices[1], surfaceNormals[surface],
		/*6*/	rockVertices[6], surfaceNormals[surface++],

		/*11*/	rockVertices[11], surfaceNormals[surface],
		/*0*/	rockVertices[0], surfaceNormals[surface],
		/*9*/	rockVertices[9], surfaceNormals[surface++],

		/*2*/	rockVertices[2], surfaceNormals[surface],
		/*11*/	rockVertices[11], surfaceNormals[surface],
		/*9*/	rockVertices[9], surfaceNormals[surface++],

		/*5*/	rockVertices[5], surfaceNormals[surface],
		/*2*/	rockVertices[2], surfaceNormals[surface],
		/*9*/	rockVertices[9], surfaceNormals[surface++],

		/*11*/	rockVertices[11], surfaceNormals[surface],
		/*2*/	rockVertices[2], surfaceNormals[surface],
		/*7*/	rockVertices[7], surfaceNormals[surface]
	};
    
    // Generate buffers
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    
    // Buffer object data
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * vertices.size(), vertices.data(), GL_STATIC_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * indices.size(), indices.data(), GL_STATIC_DRAW);
    
	// Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    
	// Normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    
    // Apply translation to model matrix
    model = glm::translate(model, -position);
    
	// Apply scale to model matrix
	if (rand() % 100 == 99)
	{
		model = glm::scale(model, glm::vec3(6.0f, 6.0f, 6.0f));
	}
	else
	{
		float scaler = rand() % 4;
		model = glm::scale(model, glm::vec3(scaler, scaler, scaler));
	}
    
    // Apply rotations to model matrix
    glm::vec3 eulerXYZ(dis(gen) * PI, dis(gen) * PI, dis(gen) * PI);
    
    model = glm::rotate(model, eulerXYZ.x, glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::rotate(model, eulerXYZ.y, glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::rotate(model, eulerXYZ.z, glm::vec3(0.0f, 0.0f, 1.0f));
    
    
    // Compile and load shaders
    if(rockShader == nullptr)
        rockShader = new Shader("res/shaders/rock.vs", "res/shaders/rock.fs"); 
    
    shader = rockShader;
}

void Rock::render(glm::mat4 view, glm::mat4 projection)
{
    shader->use();
    
    // Broadcast the uniform values to the shaders
    GLuint modelLoc = glGetUniformLocation(shader->program, "model");
    GLuint viewLoc = glGetUniformLocation(shader->program, "view");
    GLuint projectionLoc = glGetUniformLocation(shader->program, "projection");
    
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
    
    // Draw object
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 60);
    glBindVertexArray(0);
}
