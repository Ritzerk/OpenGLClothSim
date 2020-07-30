#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <cmath>
#include <filesystem>

#include "ErrorManager.h"

#include "Shader.h"
#include "Camera.h"

#include "Particle.h"
#include "triangle.h"
#include "Cloth.h"

#include "Constraint.h"

void adjustViewportToWindowSize(GLFWwindow* window, int width, int height);
void setVSync(bool enabled);

void checkEsc(GLFWwindow* window);
void processInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = 800 / 2.0f;
float lastY = 600 / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;


int main(void)
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "Tab name", NULL, NULL);
	if (window == NULL) 
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1; 
	}

	setVSync(true);	//disable VSync. 
	glEnable(GL_DEPTH_TEST);

	glViewport(0, 0, 800, 600);	//size of GL rendering window. 
	glfwSetFramebufferSizeCallback(window, adjustViewportToWindowSize);		//Viewport needs to be resized everytime user resizes window, this is done using a callback. 
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	ILboolean success;
	unsigned int imageID;

	ilInit();				//init DevIL for textures; Once per application

	Shader firstShader("shaders/firstShader.vs", "shaders/firstShader.fs");


	glm::mat4 model = glm::mat4(1.0f);
	glm::mat4 projection = glm::mat4(1.0f);
	glm::mat4 view = glm::mat4(1.0f);


	Cloth cloth = Cloth(0.8, 0.4, 0.1);	


	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = (float)glfwGetTime();
		deltaTime = currentFrame - lastFrame;            //the time it took to render frame
		lastFrame = currentFrame;

		//std::cout << "Delta time: " << deltaTime << std::endl;

		processInput(window);
		glfwSetScrollCallback(window, scroll_callback);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);    //calling glClear sets the background to color values set by glClearColor function.


		firstShader.use();

		projection = glm::perspective(glm::radians(camera.Fov), 800.0f / 600.0f, 0.1f, 100.0f); 
		view = camera.GetViewMatrix();
		

		firstShader.setMatrix4fv("model", glm::value_ptr(model));
		firstShader.setMatrix4fv("projection", glm::value_ptr(projection));
		firstShader.setMatrix4fv("view", glm::value_ptr(view));

		cloth.drawCloth(deltaTime);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	//Clear up
	//glDeleteVertexArrays(1, &VAO);
	//glDeleteBuffers(1, &VBO);

	glfwTerminate();
	return 0; 

}

void adjustViewportToWindowSize(GLFWwindow* window, int width, int height) 
{
	glViewport(0, 0, width, height);
}

void checkEsc(GLFWwindow *window) 
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

void processInput(GLFWwindow *window) 
{
	checkEsc(window);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);

}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}

void setVSync(bool enabled)
{
	if (enabled)
	{
		glfwSwapInterval(1);
	}
	else 
	{
		glfwSwapInterval(0);
	}
}