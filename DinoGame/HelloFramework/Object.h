#pragma once

#include <string>
#include <iostream>
#include <vector>

// GLAD
#include <glad/glad.h>

// GLFW
#include <GLFW/glfw3.h>

#include <glm/glm.hpp> 
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"

using namespace std;



class Object
{
public:
	Object() {}
	~Object();

	void initialize();
	void update();
	void draw();

	inline void setTexture(GLuint texID) { this->texID = texID; }
	inline void setShader(Shader* shader) { this->shader = shader; }
	inline Shader* getShader() { return this->shader; }

	inline void setPosition(glm::vec3 position) { this->position = position; initalPositionY = position.y; initalPositionX = position.x; }
	inline void setDimensions(glm::vec3 scale) { this->scale = scale; }
	inline void setAngle(float angle) { this->angle = angle; }

	inline glm::vec3 getScale() { this->scale; }

	inline int getPositionY() { return this->position.y; }
	inline int getInitalPositionY() { return this->initalPositionY; }
	inline int getInitalPositionX() { return this->initalPositionX; }
	inline void setPositionX(float x) { this->position.x += x; }
	inline void IncrensePositionY(float y) { this->position.y += y; }

	inline void resetPositionY() { this->position.y = initalPositionY; }
	inline void resetPositionX() { this->position.x = initalPositionX; }

	bool Collision(Object sprite);

	float getXPos() { return position.x; }


protected:
	void setRotation(float angle, glm::vec3 axis, bool reset = false);
	void setTranslation(glm::vec3 displacements, bool reset = false);
	void setScale(glm::vec3 scaleFactors, bool reset = false);

	GLuint VAO;
	glm::mat4 transform;
	GLuint texID;
	Shader* shader;
	glm::vec3 position, scale;
	float angle;

	float initalPositionY;
	float initalPositionX;
};

