#define STB_IMAGE_IMPLEMENTATION

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <shader_m.h>
#include <camera.h>

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

Camera camera(glm::vec3(0.0f, 0.5f, 5.0f));
double lastX = SCR_WIDTH / 2.0f;
double lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

int main()
{
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

	// glfw window creation
	// --------------------
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "test2", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// build and compile our shader zprogram
	// ------------------------------------
	Shader roadShader("E:\\Solutions\\Experments_of_Computer_Animation_Technology_Foundation\\test1\\shaders\\test1.vs",
		"E:\\Solutions\\Experments_of_Computer_Animation_Technology_Foundation\\test1\\shaders\\test1.fs");

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float road[] = {
		// positions             // texture coords
		 0.5f,  100.0f, 0.0f,    1.0f, 100.0f, // top right
		 0.5f,	-50.0f, 0.0f,    1.0f, -50.0f, // bottom right
		-0.5f,	-50.0f, 0.0f,    0.0f, -50.0f, // bottom left
		-0.5f,  100.0f, 0.0f,    0.0f, 100.0f  // top left 
	};

	unsigned int indices_road[] = {
		0, 1, 3, // first triangle
		1, 2, 3  // second triangle
	};

	float car[] = {
		//下半部
		-40.0f,   0.0f,  5.0f,
		 40.0f,   0.0f,  5.0f,
		 40.0f, -15.0f,  5.0f,
		-40.0f,   0.0f,  5.0f,
		 40.0f, -15.0f,  5.0f,
		-40.0f, -15.0f,  5.0f,//18

		40.0f,  0.0f,  -15.0f,
		40.0f,  0.0f,   15.0f,
		40.0f, -15.0f,  15.0f,
		40.0f,  0.0f,  -15.0f,
		40.0f, -15.0f,  15.0f,
		40.0f, -15.0f, -15.0f,//36

		-40.0f,  0.0f, -15.0f,
		-40.0f,  0.0f,  15.0f,
		-40.0f, -15.0f, 15.0f,
		-40.0f,  0.0f, -15.0f,
		-40.0f, -15.0f, 15.0f,
		-40.0f, -15.0f,-15.0f,//54

		-40.0f,  0.0f,  -15.0f,
		 40.0f,  0.0f,  -15.0f,
		 40.0f, -15.0f, -15.0f,
		-40.0f,  0.0f,  -15.0f,
		 40.0f, -15.0f, -15.0f,
		-40.0f, -15.0f, -15.0f,//72
		
		-40.0f,  0.0f,   15.0f,
		-40.0f,  0.0f,  -15.0f,
		 40.0f,  0.0f,  -15.0f,
		-40.0f,  0.0f,   15.0f,
		 40.0f,  0.0f,  -15.0f,
		 40.0f,  0.0f,   15.0f,

		 -40.0f, -15.0f,  15.0f,
		 -40.0f, -15.0f, -15.0f,
		  40.0f, -15.0f, -15.0f,
		 -40.0f, -15.0f,  15.0f,
		  40.0f, -15.0f, -15.0f,
		  40.0f, -15.0f,  15.0f,

		  //上半部
		 -20.0f, 0.0f,  15.0f,
		 -10.0f, 10.0f, 15.0f,
		  20.0f, 10.0f, 15.0f,
		 -20.0f, 0.0f,  15.0f,
		  20.0f, 10.0f, 15.0f,
		  25.0f, 0.0f,  15.0f,

		  -20.0f, 0.0f, -15.0f,
		  -10.0f, 10.0f, -15.0f,
		   20.0f, 10.0f, -15.0f,
		  -20.0f, 0.0f, -15.0f,
		   20.0f, 10.0f, -15.0f,
		   25.0f, 0.0f, -15.0f,

		 -10.0f, 10.0f, 15.0f,
		 -10.0f, 10.0f, -15.0f,
		  20.0f, 10.0f, -15.0f,
		 -10.0f, 10.0f, 15.0f,
		  20.0f, 10.0f, -15.0f,
		  20.0f, 10.0f, 15.0f,

		  10.0f, 10.0f, 15.0f,
		 -20.0f, 0.0f, 15.0f,
		 -20.0f, 0.0f, -15.0f,
		 -20.0f, 0.0f, -15.0f,
		  10.0f, 10.0f, 15.0f,
		 -10.0f, 10.0f, -15.0f,

		 20.0f, 10.0f, 15.0f,
		 20.0f, 10.0f, -15.0f,
	 	 25.0f, 0.0f, -15.0f,
		 20.0f, 10.0f, 15.0f,
	 	 25.0f, 0.0f, -15.0f,
		 25.0f, 0.0f, 15.0f,

		-30.0f, -15.0f, 15.0f,
		-30.0f, -15.0f, -15.0f,
		 30.0f, -15.0f, -15.0f,
		-30.0f, -15.0f, 15.0f,
		 30.0f, -15.0f, -15.0f,
		 30.0f, -15.0f, 15.0f,

	};


	unsigned int VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(road), road, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices_road), indices_road, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// texture coord attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);


	float ground[]=
	{
			0.5f,  0.0f, 100.0f,
			0.5f,  0.0f, -50.0f,
			50.0f, 0.0f,-50.0f,
			50.0f, 0.0f, 100.0f
	};

	unsigned int indices_ground[] = {
		0,1,2,
		2,3,0
	};

	unsigned int VAO_G, VBO_G, EBO_G;
	glGenVertexArrays(1, &VAO_G);
	glGenBuffers(1, &VBO_G);
	glGenBuffers(1, &EBO_G);

	glBindVertexArray(VAO_G);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_G);
	glBufferData(GL_ARRAY_BUFFER, sizeof(ground), ground, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_G);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices_ground), indices_ground, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	Shader groundShader("E:\\Solutions\\Experments_of_Computer_Animation_Technology_Foundation\\test2\\shaders\\g.vs",
		"E:\\Solutions\\Experments_of_Computer_Animation_Technology_Foundation\\test2\\shaders\\g.fs");


	// load and create a texture 
	// -------------------------
	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, create texture and generate mipmaps
	int width, height, nrChannels;
	// The FileSystem::getPath(...) is part of the GitHub repository so we can find files on any IDE/platform; replace it with your own image path.
	unsigned char* data = stbi_load("E:\\Solutions\\Experments_of_Computer_Animation_Technology_Foundation\\test1\\testures\\road.jpg", &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);


	unsigned int VBO_CAR,VAO_CAR;
	glGenVertexArrays(1, &VAO_CAR);
	glGenBuffers(1, &VBO_CAR);

	glBindVertexArray(VAO_CAR);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_CAR);
	glBufferData(GL_ARRAY_BUFFER, sizeof(car), car, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	Shader carShader("E:\\Solutions\\Experments_of_Computer_Animation_Technology_Foundation\\test2\\shaders\\car.vs",
		"E:\\Solutions\\Experments_of_Computer_Animation_Technology_Foundation\\test2\\shaders\\car.fs");



	//金字塔部分
	float Pyramid[] = {
		 11.5f, 0.0f,  11.5f,
		 11.5f, 0.0f, -11.5f,
		-11.5f, 0.0f, -11.5f,
		-11.5f, 0.0f,  11.5f,
		  0.0f, 13.6f, 0.0f
	};

	unsigned int indices_Pyramid[] = {
		0, 1, 4,
		1, 2, 4,
		2, 3, 4,
		3, 0, 4
	};

	glm::vec3 pPos[] = {
		glm::vec3(-50.0f, 0.0f, -40.0f),
		glm::vec3(40.0f, 0.0f, -50.0f),
		glm::vec3(30.0f, 0.0f, -0.0f),

	};

	unsigned VAO_PYRAMID, VBO_PYRAMID, EBO_PYRAMID;
	glGenVertexArrays(1,& VAO_PYRAMID);
	glGenBuffers(1, &VBO_PYRAMID);
	glGenBuffers(1, &EBO_PYRAMID);

	glBindVertexArray(VAO_PYRAMID);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_PYRAMID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Pyramid), Pyramid, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_PYRAMID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices_Pyramid), indices_Pyramid, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	Shader pShader("E:\\Solutions\\Experments_of_Computer_Animation_Technology_Foundation\\test1\\shaders\\p.vs",
		"E:\\Solutions\\Experments_of_Computer_Animation_Technology_Foundation\\test1\\shaders\\p.fs");

	
	glm::mat4 projection = glm::mat4(1.0f);
	projection = glm::perspective(glm::radians(30.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

	// render loop
	// -----------
	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// input
		// -----
		processInput(window);

		// render
		// ------
		//63, 142, 199
		glClearColor(0.247f, 0.557f, 0.78f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
		glBindTexture(GL_TEXTURE_2D, texture);
		glm::mat4 view = camera.GetViewMatrix();
		
		
		//绘制路
		roadShader.use();
		
		glm::mat4 model_road = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
		model_road = glm::rotate(model_road, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

		roadShader.setMat4("view", view);
		roadShader.setMat4("model", model_road);
		roadShader.setMat4("projection", projection);

		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		//绘制小车
		carShader.use();
		glm::mat4 model_car = glm::mat4(1.0f);
		model_car = glm::translate(model_car, glm::vec3(0.0f, 0.25f, 0.0f));
		
		model_car = glm::rotate(model_car, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));

		//小车颜色渐变
		float timeValue = glfwGetTime();
		//float Value = sin(timeValue) / 2.0f + 0.5f;
		//carShader.setVec4("ourColor", glm::vec4(Value, 1.0f, 1.0f, 1.0f ));
		float greenValue, BlueValue;
		if (timeValue < 2) {
			greenValue = 0;
			BlueValue = 1.0f;
		}else if(timeValue >=2 && timeValue <7){
			greenValue = (timeValue -2) / 5;
			BlueValue = 1 - (timeValue - 2) / 5;
		}else {

			greenValue = 1.0f;
			BlueValue = 0;
		}
		carShader.setVec4("ourColor", glm::vec4(BlueValue, greenValue, 0.0f, 1.0f));

		carShader.setMat4("view", view);
		model_car = glm::scale(model_car, glm::vec3(0.025, 0.025, 0.025));
		carShader.setMat4("model", model_car);
		carShader.setMat4("projection", projection);
		
		carShader.setFloat("dx", timeValue*50);



		glBindVertexArray(VAO_CAR);
		glDrawArrays(GL_TRIANGLES, 0, 72);

		//绘制金字塔
		pShader.use();
		for (int k = 0; k < 3; k++) {
			glm::mat4 model_p = glm::mat4(1.0f);
			model_p = glm::scale(model_p, glm::vec3(0.05f, 0.05f, 0.05f));
			model_p = glm::translate(model_p, pPos[k]);

			pShader.setMat4("view", view);
			pShader.setMat4("model", model_p);
			pShader.setMat4("projection", projection);
			glBindVertexArray(VAO_PYRAMID);
			glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);
		}
		

		//绘制地面
		groundShader.use();
		glm::mat4 model_g = glm::mat4(1.0f);
		groundShader.setMat4("view", view);
		groundShader.setMat4("model", model_g);
		groundShader.setMat4("projection", projection);
		glBindVertexArray(VAO_G);

		groundShader.setFloat("i", 1.0f);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		groundShader.setFloat("i", -1.0f);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// optional: de-allocate all resources once they've outlived their purpose:
	// ------------------------------------------------------------------------
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glfwTerminate();
	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
}





// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = (float)xpos - lastX;
	float yoffset = lastY - (float)ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;
	
	camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}