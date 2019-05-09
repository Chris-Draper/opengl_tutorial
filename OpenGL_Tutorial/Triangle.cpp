// this makes it so that a console doesn't show up behind the render window
// there are other ways 
// #pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "MyShader.h"

#include <iostream>

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// register callback function for resizing window
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

int main()
{
	std::cout << "Running Shaders.cpp file" << std::endl;

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//need to do this on MAC OS ONLY
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	// create the opengl window pointer
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	// set opengl's focus to the current window
	glfwMakeContextCurrent(window);

	// register the callback function for resizing the opengl window
	// this gets called when the window is first open with default width and height
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// setup glad to manage function pointers in opengl
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// setup the vertex shader and fragment shader
	Shader triShader("vShader.vs", "fShader.fs");

	// ----------------------------------------------------
	// Gen VAO, Gen VBO, Gen Attributes, Store VBO into VAO
	// VAO = Vertex Attribute Object
	// VBO = Vertex Buffer Object
	// ----------------------------------------------------

	// setup the data for the triangle's vertices
	float vertices[] = {
		// positions         // colors in RGB format
		 0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // bottom right - red
		-0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // bottom left - green
		 0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // top - blue
	};

	// get id of the vertex buffer object
	unsigned int VBO, VAO;
	// generate the VAO and VBO
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	// bind the VAO first, then bind VBO, then set vertex attributes
	glBindVertexArray(VAO);
	// set the type of the buffer - aka array buffer
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// fill the buffer with the vertices' data and say static draw is infrequent update
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// vertex position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// vertex color attribute - 4th parameter is the offset to the color
	// first parameters are 1 here becuase that's the location set for color in the vertex shader
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*) (3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// -----------
	// RENDER LOOP
	// -----------

	// the render loop that tells opengl to stay open unless told to exit
	while (!glfwWindowShouldClose(window))
	{
		// check for user input every render loop
		processInput(window);

		// window rendering commands will go here
		// set the color you want to clear the entire window with
		// this is a state setting function
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		// clear the window with the previously set color
		// this is a state using function
		glClear(GL_COLOR_BUFFER_BIT);

		// tell GL to use the new shader program whenever render is called
		triShader.use();
		// bind the vertex array again? Not sure why but see if that fixes the bug?
		glBindVertexArray(VAO);
		// render the triangle
		glDrawArrays(GL_TRIANGLES, 0, 3);

		// swap out colors in the buffer based on updated user input
		glfwSwapBuffers(window);
		// check for any user input
		glfwPollEvents();
	}

	// optional: de-allocate all resources after done rendering
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	// delete all of glfw's resources used to render after done rendering
	glfwTerminate();
	return 0;
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

// check if the user pressed ESC and set close window if they did
void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
}