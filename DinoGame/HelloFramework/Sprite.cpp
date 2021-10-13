#include "Sprite.h"

void Sprite::setSpritesheet(GLuint texID, int nAnimations, int nFrames)
{
	this->texID = texID;
	this->nAnimations = nAnimations;
	this->nFrames = nFrames;

	iAnimation = 0;
	iFrame = 0;

	dx = 1.0 / (float)nFrames;
	dy = 1.0 / (float)nAnimations;

	float vertices[] = {
		// positions          // colors           // texture coords
		0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   dx, dy, // top right
		0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   dx, 0.0f, // bottom right
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
		-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, dy  // top left 
	};
	unsigned int indices[] = {
		0, 1, 3, // first triangle
		1, 2, 3  // second triangle
	};
	unsigned int VBO, EBO;
	
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// texture coord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
}

void Sprite::update()
{
	shader->Use();

	setTranslation(position, true);
	setRotation(angle, glm::vec3(0.0, 0.0, 1.0));
	setScale(scale);

	shader->setMat4("model", glm::value_ptr(transform));

	GLint offsetLoc = glGetUniformLocation(shader->Program, "offset");
	glUniform2f(offsetLoc, iFrame * dx, iAnimation * dy);
	iFrame = (iFrame + 1) % nFrames;
}

bool Sprite::onScreen()
{		
	return _onScreen;
}

void Sprite::Draw()
{
	Object::draw();
	_onScreen = true;
}

void Sprite::removeScreen()
{
	_onScreen = false;
}
