#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <shader_m.h>
#include <glm/glm.hpp>
#include <glm/gtx/spline.hpp>
#include <vector>
#include <math.h>

#include <iostream>


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void cursor_position_callback(GLFWwindow* window, double xPos, double yPos);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);

//window settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
int currentWidth = 800;
int currentHeight = 600;
int currentPointNumber = 0;
int MAXPOINT = 4;
bool enterResponse = true;

std::vector<glm::vec3> controlPoints = { glm::vec3(-1.0f, 1.0f, 0.0f) };
std::vector<glm::vec3> interpolatedPoints = { glm::vec3(0.0f, 0.0f, 0.0f) };

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
	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	//use Shader class
	Shader ourShader("E:\\Solutions\\Experments_of_Computer_Animation_Technology_Foundation\\test4\\shaders\\test4.vs",
		"E:\\Solutions\\Experments_of_Computer_Animation_Technology_Foundation\\test4\\shaders\\test4.fs");


	unsigned int VBO[2], VAO[2];
	glGenVertexArrays(2, VAO);
	glGenBuffers(2, VBO);
	
	glBindVertexArray(VAO[0]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, controlPoints.size() * sizeof(glm::vec3), controlPoints.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(VAO[1]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, interpolatedPoints.size() * sizeof(glm::vec3), interpolatedPoints.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);


	// render loop
	// -----------
	while (!glfwWindowShouldClose(window))
	{

		processInput(window);
		glfwSetCursorPosCallback(window, cursor_position_callback);
		glfwSetMouseButtonCallback(window, mouse_button_callback);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		ourShader.use();
		glLineWidth(1.0f);
		glPointSize(3.0f);
		glBindVertexArray(VAO[0]);
		ourShader.setVec4("ourColor", glm::vec4(1.0f, 0.0f, 1.0f, 1.0f));
		glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
		glBufferData(GL_ARRAY_BUFFER, controlPoints.size() * sizeof(glm::vec3), controlPoints.data(), GL_STATIC_DRAW);
		glDrawArrays(GL_POINTS, 1, controlPoints.size() - 2);
		glDrawArrays(GL_LINE_STRIP, 1, controlPoints.size()- 1);
		 
		glLineWidth(2.0f);
		ourShader.setVec4("ourColor", glm::vec4(1.0f, 1.0f, 0.0f, 1.0f));
		glBindVertexArray(VAO[1]);
		glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
		glBufferData(GL_ARRAY_BUFFER, interpolatedPoints.size() * sizeof(glm::vec3), interpolatedPoints.data(), GL_STATIC_DRAW);
		glDrawArrays(GL_LINE_STRIP, 1, interpolatedPoints.size() - 1);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(2, VAO);
	glDeleteBuffers(2, VBO);

	glfwTerminate();
	return 0;
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS && currentPointNumber == MAXPOINT && enterResponse) {
		std::cout << "Add interpolated points." << std::endl;

		for (int i = 0; i <= 100; i++)
		{//添加插值点 1到2
			interpolatedPoints.push_back(glm::catmullRom(controlPoints[0], controlPoints[1], controlPoints[2], controlPoints[3], (float)i / 100));
		}


		for (int i = 0; i <= 100; i++)
		{//添加插值点 2到3
			interpolatedPoints.push_back(glm::catmullRom(controlPoints[1], controlPoints[2], controlPoints[3], controlPoints[4], (float)i / 100));
		}

		for (int i = 0; i <= 100; i++)
		{//添加插值点 3到4
			interpolatedPoints.push_back(glm::catmullRom(controlPoints[2], controlPoints[3], controlPoints[4], glm::vec3(1.0f,1.0f,0), (float)i / 100));
		}

		enterResponse = false;
	}
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{

	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS && currentPointNumber < MAXPOINT)
	{
		double x, xPos;
		double y, yPos;
		//获取并转换坐标
		glfwGetCursorPos(window, &xPos, &yPos);
		x = double((xPos - currentWidth / 2) / currentWidth) * 2;
		y = double(0 - (yPos - currentHeight / 2) / currentHeight) * 2;
		//添加控制点
		controlPoints.push_back(glm::vec3(x, y, 0));
		currentPointNumber++;

		std::cout << "Add controlPoint " << currentPointNumber << " :x= " << x << ", y= " << y << std::endl;

	}
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
	currentHeight = height;
	currentWidth = width;
}

void cursor_position_callback(GLFWwindow* window, double xPos, double yPos)
{
	//什么也不做
}