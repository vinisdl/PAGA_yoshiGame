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
#include "Sprite.h"
using namespace std;



class Jump
{
public:
	Jump(Sprite* sprite, vector<Sprite>* connectedObjects);
	~Jump() {};

	void StartJump();
	void Check();
	void Down();
	
protected:
	void checkConnectedObjects();
	void ResetJump();

	Sprite* sprite;
	vector<Sprite> *connectedObjects;
	bool jump;
	bool up;
	bool down;
};

