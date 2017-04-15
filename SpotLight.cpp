#include "SpotLight.h"



SpotLight::SpotLight(): Light()
{
	this->position = glm::vec3(0.0f, 0.0f, 0.0f);
	this->direction = glm::vec3(0.0f, 0.0f, 1.0f);
	this->cutOff = glm::cos(glm::radians(12.5f));
	this->outerCutOff = glm::cos(glm::radians(15.0f));
	this->constant = 1.0f;
	this->linear = 0.09f;
	this->quadratic = 0.032f;
}

SpotLight::SpotLight(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, glm::vec3 position, glm::vec3 direction, float cutOff, float outerCutOff, float constant, float linear, float quadratic):
Light(ambient, diffuse, specular)
{
	this->position = position;
	this->direction = direction;
	this->cutOff = cutOff;
	this->outerCutOff = outerCutOff;
	this->constant = constant;
	this->linear = linear;
	this->quadratic = quadratic;
}

SpotLight::SpotLight(glm::vec3 position, glm::vec3 direction): Light()
{
	this->position = position;
	this->direction = direction;
	this->cutOff = glm::cos(glm::radians(12.5f));
	this->outerCutOff = glm::cos(glm::radians(15.0f));
	this->constant = 1.0f;
	this->linear = 0.09f;
	this->quadratic = 0.032f;
}

void SpotLight::UpdatePosition(glm::vec3 position, glm::vec3 direction)
{
	this->position = position;
	this->direction = direction;
}


SpotLight::~SpotLight()
{
}
