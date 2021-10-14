/* Hello Triangle - c�digo adaptado de https://learnopengl.com/#!Getting-started/Hello-Triangle
 *
 * Adaptado por Rossana Baptista Queiroz
 * para a disciplina de Processamento Gr�fico - Jogos Digitais - Unisinos
 * Vers�o inicial: 7/4/2017
 * �ltima atualiza��o em 09/08/2021
 *
 */

#include <iostream>
#include <string>
#include <assert.h>
#include <cmath>


using namespace std;

// GLAD
#include <glad/glad.h>

// GLFW
#include <GLFW/glfw3.h>

#include <glm/glm.hpp> 
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>

#include "stb_image.h"

#include "Shader.h"

#include "Object.h"
#include "Sprite.h"
#include "Timer.h"
#include "Jump.h"
#include <map>

using namespace std;

// Prot�tipo da fun��o de callback de teclado
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

// Prot�tipos das fun��es
int setupGeometry();
int loadTexture(string path);
GLuint createSprite();

// Dimens�es da janela (pode ser alterado em tempo de execu��o)
const GLuint WIDTH = 800, HEIGHT = 600;
const int nPoints = 100 + 1 + 1; //+centro +c�pia do primeiro
const float pi = 3.14159;

bool keys[1024];

// Fun��o MAIN
int main()
{
	// Inicializa��o da GLFW
	glfwInit();

	//Muita aten��o aqui: alguns ambientes n�o aceitam essas configura��es
	//Voc� deve adaptar para a vers�o do OpenGL suportada por sua placa
	//Sugest�o: comente essas linhas de c�digo para desobrir a vers�o e
	//depois atualize (por exemplo: 4.5 com 4 e 5)
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//Essencial para computadores da Apple
#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// Cria��o da janela GLFW
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Dino Yoshi!", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	// Fazendo o registro da fun��o de callback para a janela GLFW
	glfwSetKeyCallback(window, key_callback);

	// GLAD: carrega todos os ponteiros d fun��es da OpenGL
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;

	}

	// Obtendo as informa��es de vers�o
	const GLubyte* renderer = glGetString(GL_RENDERER); /* get renderer string */
	const GLubyte* version = glGetString(GL_VERSION); /* version as a string */
	cout << "Renderer: " << renderer << endl;
	cout << "OpenGL version supported " << version << endl;

	// Compilando e buildando o programa de shader
	Shader* shader = new Shader("./shaders/sprite.vs", "./shaders/sprite.fs");
	Shader* shaderEarth = new Shader("./shaders/sprite.vs", "./shaders/sprite.fs");
	Shader* animShader = new Shader("./shaders/animatedsprites.vs", "./shaders/animatedsprites.fs");
	Shader* animShaderCactusSmall = new Shader("./shaders/animatedsprites.vs", "./shaders/animatedsprites.fs");

	;
	GLuint backgroundTexID = loadTexture("./textures/background.png");
	GLuint earthTexID = loadTexture("./textures/earth.png");

	GLuint yoshiTexID = loadTexture("./textures/yoshi.png");
	GLuint cactusSmallTexID = loadTexture("./textures/cactusSmall.png");

	//Texturas das camadas

	vector<Sprite> birds;

	for (int i = 0; i < 3; i++)
	{
		GLuint birdID = loadTexture("./textures/bird.png");
		Shader* animatedShader = new Shader("./shaders/animatedsprites.vs", "./shaders/animatedsprites.fs");

		Sprite bird;
		bird.initialize();
		bird.setSpritesheet(birdID, 1, 2);
		bird.setShader(animatedShader);
		bird.setDimensions(glm::vec3(30.0 * 2, 39.0 * 2, 1.0));
		bird.setPosition(glm::vec3(900, 500.0, 0.0));
		birds.push_back(bird);
	}


	vector<Object> clouds;

	for (int i = 0; i < 10; i++)
	{
		GLuint cloudID = loadTexture("./textures/cloud.png");
		Shader* cloudShader = new Shader("./shaders/sprite.vs", "./shaders/sprite.fs");

		Object cloud;
		cloud.initialize();
		cloud.setTexture(cloudID);
		cloud.setShader(cloudShader);
		cloud.setDimensions(glm::vec3(30.0 * 2, 39.0 * 2, 1.0));
		cloud.setPosition(glm::vec3(900, 500.0, 0.0));
		clouds.push_back(cloud);
	}


	////Camadas			

	Object cactus;
	cactus.initialize();
	cactus.setTexture(cactusSmallTexID);
	cactus.setShader(animShaderCactusSmall);
	cactus.setDimensions(glm::vec3(30.0, 39.0, 1.0));
	cactus.setPosition(glm::vec3(800.0, 255.0, 0.0));

	Object background;
	background.initialize();
	background.setTexture(backgroundTexID);
	background.setShader(shader);
	background.setDimensions(glm::vec3(800.0, 600.0, 1.0));
	background.setPosition(glm::vec3(400.0, 300.0, 0.0));

	Object earth;
	earth.initialize();
	earth.setTexture(earthTexID);
	earth.setShader(shaderEarth);
	earth.setDimensions(glm::vec3(800.0, 38.0, 1.0));
	earth.setPosition(glm::vec3(400.0, 220.0, 0.0));


	Sprite yoshi;
	yoshi.initialize();
	yoshi.setSpritesheet(yoshiTexID, 2, 8);
	yoshi.setShader(animShader);
	yoshi.setDimensions(glm::vec3(30.0 * 2, 39.0 * 2, 1.0));
	yoshi.setPosition(glm::vec3(200.0, 270.0, 0.0));

	float xini = 100;
	float yini = 100;
	float spriteW = 100.0;
	float spriteH = 120.0;


	//Ativando o shader que ser� usado
	shader->Use();
	glUniform1i(glGetUniformLocation(shader->Program, "tex1"), 0);

	shaderEarth->Use();
	glUniform1i(glGetUniformLocation(shaderEarth->Program, "tex4"), 0);

	animShader->Use();
	glUniform1i(glGetUniformLocation(animShader->Program, "tex2"), 0);

	animShaderCactusSmall->Use();
	glUniform1i(glGetUniformLocation(animShaderCactusSmall->Program, "tex3"), 0);

	glm::mat4 ortho = glm::mat4(1); //inicializa com a matriz identidade
	double xmin = 0.0, xmax = 800.0, ymin = 0.0, ymax = 600.0;

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_ALWAYS);


	Timer timer;
	yoshi.setAnimation(1);
	Jump jump(&yoshi, &birds);
	int cactusLevel = 1;

	// Loop da aplica��o - "game loop"


	std::map<int, int> level = {	};

	int previous = 0;
	for (int i = 0; i < 100; i++)
	{
		if (i % 2 == 0)
			previous += 2;
		level.insert({ i, previous });
	}



	while (!glfwWindowShouldClose(window))
	{
		float initlevel = (float)glfwGetTime();

		timer.start();

		// Checa se houveram eventos de input (key pressed, mouse moved etc.) e chama as fun��es de callback correspondentes
		glfwPollEvents();

		// Definindo as dimens�es da viewport com as mesmas dimens�es da janela da aplica��o
		int width, height;
		glfwGetFramebufferSize(window, &width, &height);
		glViewport(0, 0, width, height);

		//Redefinindo a matriz de proje��o
		ortho = glm::ortho(xmin, xmax, ymin, ymax, -1.0, 1.0);

		// Limpa o buffer de cor
		glClearColor(0.3f, 0.3f, 0.3f, 1.0f); //cor de fundo
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		//Enviar a matriz de proje��o ortogr�fica para o shader
		shader->Use();
		shader->setMat4("projection", glm::value_ptr(ortho));

		shaderEarth->Use();
		shaderEarth->setMat4("projection", glm::value_ptr(ortho));

		//Enviar a matriz de proje��o ortogr�fica para o shader
		animShader->Use();
		animShader->setMat4("projection", glm::value_ptr(ortho));

		animShaderCactusSmall->Use();
		animShaderCactusSmall->setMat4("projection", glm::value_ptr(ortho));

		if (keys[GLFW_KEY_UP])
		{
			jump.StartJump();
		}

		if (keys[GLFW_KEY_DOWN])
		{
			jump.Down();
		}

		jump.Check();

		background.update();
		background.draw();

		earth.update();
		earth.draw();


		for (int i = 0; i < clouds.size() - 1; i++)
		{
			clouds[i].getShader()->Use();
			clouds[i].getShader()->setMat4("projection", glm::value_ptr(ortho));

			if (clouds[i].getXPos() > -2)
			{
				clouds[i].incrensePositionX(-1);
			}
			else
			{
				clouds[i].incrensePositionX(900.0);
			}
			int v1 = rand() % 300;
	
			if (!clouds[i].onScreen() && v1 == 1) {
				clouds[i].setPosition(glm::vec3(900, 500.0, 0.0));
				clouds[i].update();
				clouds[i].draw();
			}

			if (clouds[i].onScreen()) {
				clouds[i].update();
				clouds[i].draw();
			}

		}


		for (int i = 0; i < birds.size() - 1; i++)
		{
			birds[i].getShader()->Use();
			birds[i].getShader()->setMat4("projection", glm::value_ptr(ortho));

			if (birds[i].getXPos() > -4)
			{
				birds[i].incrensePositionX(-3);
			}
			else
			{
				birds[i].incrensePositionX(900.0);
				birds[i].removeScreen();
			}
			int v1 = rand() % 200;

			if (!birds[i].onScreen() && v1 == 1) {
				birds[i].setPosition(glm::vec3(900, 500.0, 0.0));
				birds[i].update();
				birds[i].draw();
			}

			if (birds[i].onScreen()) {
				birds[i].update();
				birds[i].draw();
			}

		}


		cactusLevel = static_cast<int>(initlevel);

		if (cactus.getXPos() > (6.0 + level.find(cactusLevel)->second) * -1)
		{
			cactus.incrensePositionX((8.0 + level.find(cactusLevel)->second) * -1);
		}
		else
		{
			cactus.incrensePositionX(900.0);
		}


		yoshi.update();
		yoshi.draw();

		cactus.update();
		cactus.draw();

		timer.finish();
		double waitingTime = timer.calcWaitingTime(30, timer.getElapsedTimeMs());
		if (waitingTime)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds((int)waitingTime));
		}

		if (yoshi.collision(cactus)) {
			break;
		}

		// Troca os buffers da tela
		glfwSwapBuffers(window);
	}

	std::cout << "Acabou\n";
	std::cout << "Level atingido: ";
	std::cout << cactusLevel;
	std::cout << "\n";

	// Finaliza a execu��o da GLFW, limpando os recursos alocados por ela
	glfwTerminate();
	return 0;
}


// Fun��o de callback de teclado - s� pode ter uma inst�ncia (deve ser est�tica se
// estiver dentro de uma classe) - � chamada sempre que uma tecla for pressionada
// ou solta via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
			keys[key] = true;
		else if (action == GLFW_RELEASE)
			keys[key] = false;
	}
}

// Esta fun��o est� bastante harcoded - objetivo � criar os buffers que armazenam a 
// geometria de um tri�ngulo
// Apenas atributo coordenada nos v�rtices
// 1 VBO com as coordenadas, VAO com apenas 1 ponteiro para atributo
// A fun��o retorna o identificador do VAO
int setupGeometry()
{
	// Aqui setamos as coordenadas x, y e z do tri�ngulo e as armazenamos de forma
	// sequencial, j� visando mandar para o VBO (Vertex Buffer Objects)
	// Cada atributo do v�rtice (coordenada, cores, coordenadas de textura, normal, etc)
	// Pode ser arazenado em um VBO �nico ou em VBOs separados
	GLfloat* vertices;

	vertices = new GLfloat[nPoints * 3];

	float angle = 0.0;
	float deltaAngle = 2 * pi / (float)(nPoints - 2);
	float radius = 0.5;

	//Adicionar o centro
	vertices[0] = 0.0; // x
	vertices[1] = 0.0; // y
	vertices[2] = 0.0; // z sempre zero 

	for (int i = 3; i < nPoints * 3; i += 3)
	{
		vertices[i] = radius * cos(angle);
		vertices[i + 1] = radius * sin(angle);
		vertices[i + 2] = 0.0;

		angle += deltaAngle;
	}

	GLuint VBO, VAO;

	//Gera��o do identificador do VBO
	glGenBuffers(1, &VBO);
	//Faz a conex�o (vincula) do buffer como um buffer de array
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//Envia os dados do array de floats para o buffer da OpenGl
	glBufferData(GL_ARRAY_BUFFER, (nPoints * 3) * sizeof(GLfloat), vertices, GL_STATIC_DRAW);

	//Gera��o do identificador do VAO (Vertex Array Object)
	glGenVertexArrays(1, &VAO);
	// Vincula (bind) o VAO primeiro, e em seguida  conecta e seta o(s) buffer(s) de v�rtices
	// e os ponteiros para os atributos 
	glBindVertexArray(VAO);
	//Para cada atributo do vertice, criamos um "AttribPointer" (ponteiro para o atributo), indicando: 
	// Localiza��o no shader * (a localiza��o dos atributos devem ser correspondentes no layout especificado no vertex shader)
	// Numero de valores que o atributo tem (por ex, 3 coordenadas xyz) 
	// Tipo do dado
	// Se est� normalizado (entre zero e um)
	// Tamanho em bytes 
	// Deslocamento a partir do byte zero 
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	// Observe que isso � permitido, a chamada para glVertexAttribPointer registrou o VBO como o objeto de buffer de v�rtice 
	// atualmente vinculado - para que depois possamos desvincular com seguran�a
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Desvincula o VAO (� uma boa pr�tica desvincular qualquer buffer ou array para evitar bugs medonhos)
	glBindVertexArray(0);

	return VAO;
}

int loadTexture(string path)
{
	GLuint texID;

	// Gera o identificador da textura na mem�ria 
	glGenTextures(1, &texID);
	glBindTexture(GL_TEXTURE_2D, texID);

	//Ajusta os par�metros de wrapping e filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	//Carregamento da imagem
	int width, height, nrChannels;
	unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);

	if (data)
	{
		if (nrChannels == 3) //jpg, bmp
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		}
		else //png
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		}
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}

	stbi_image_free(data);

	glBindTexture(GL_TEXTURE_2D, 0);

	return texID;
}

GLuint createSprite()
{
	GLuint VAO;
	GLuint VBO, EBO;

	float vertices[] = {
		// positions          // colors          // texture coords
		0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0, // top right
		0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
		-0.5f, -0.5f, 0.0f,  0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
		-0.5f,  0.5f, 0.0f,  1.0f, 1.0f, 0.0f,   0.0f, 1.0  // top left 
	};
	unsigned int indices[] = {
	0, 1, 3, // first triangle
	1, 2, 3  // second triangle
	};

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

	return VAO;
}

