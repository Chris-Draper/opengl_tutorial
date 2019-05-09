// this makes it so that a console doesn't show up behind the render window
// there are other ways 
// #pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "MyShader.h"

#include <iostream>
#include <experimental/filesystem>

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

	// configure global OpenGL state - DEPTH TESTING
	glEnable(GL_DEPTH_TEST);

	// setup the vertex shader and fragment shader
	Shader threeDShader("vShader.vs", "fShader.fs");

	// ----------------------------------------------------
	// Gen VAO, Gen VBO, Gen Attributes, Store VBO into VAO
	// VAO = Vertex Attribute Object
	// VBO = Vertex Buffer Object
	// ----------------------------------------------------

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float vertices[] = {
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};

	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
	};

	// get id of the vertex buffer object
	// unsigned int EBO;
	unsigned int VBO, VAO;
	// generate the VAO, VBO, and EBO
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	// bind the VAO first, then bind VBO, then set vertex attributes
	glBindVertexArray(VAO);

	// set the type of the buffer - aka array buffer
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// fill the buffer with the vertices' data and say static draw is infrequent update
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//// bind EBO and put index data into it
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// vertex position attribute
	// 0 is the location
	// 3 is the number of floats
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// vertex color attribute - 4th parameter is the offset to the color
	// first parameters are 1 here becuase that's the location set for color in the vertex shader
	// the last parameter is the data offset within the stride
	/*glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);*/

	// texture attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// --------------
	// LOAD TEXTURE 1
	// --------------

	// load, gen, and bind texture
	unsigned int texture1;
	glGenTextures(1, &texture1);
	// now all calls to 2D texture will only effect this texture
	glBindTexture(GL_TEXTURE_2D, texture1);
	// set the texture wrap parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set the texture filtering parameters - scaling texture
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image
	int width, height, nrChannels;
	unsigned char* data = stbi_load("container.jpg", &width, &height, &nrChannels, 0);
	// check that data loaded correctly, set the texture to the jpg, and generate mipmaps for texture
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "ERROR::TEXTURE - Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	// --------------
	// LOAD TEXTURE 2
	// --------------

	// load, gen, and bind texture
	unsigned int texture2;
	glGenTextures(1, &texture2);
	// now all calls to 2D texture will only effect this texture
	glBindTexture(GL_TEXTURE_2D, texture2);
	// set the texture wrap parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set the texture filtering parameters - scaling texture
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// flip y coordinates on load - some images have (0,0) in the top left corner but opengl is (0,0) in top right corner
	stbi_set_flip_vertically_on_load(true);
	data = stbi_load("awesomeface.png", &width, &height, &nrChannels, 0);
	// check that data loaded correctly, set the texture to the jpg, and generate mipmaps for texture
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "ERROR::TEXTURE - Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	// activate the shader before setting any uniforms
	threeDShader.use();
	// manually set the uniform for texture
	glUniform1i(glGetUniformLocation(threeDShader.ID, "texture1"), 0);
	// use the shader special method to set the texture
	threeDShader.setInt("texture2", 1);

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
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// activate and bind the first texture unit
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);

		// activate and bind the second texture unit
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);

		// tell GL to use the new shader program whenever render is called
		threeDShader.use();

		// create the transformation matrices as identity matrices first
		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 view = glm::mat4(1.0f);
		glm::mat4 projection = glm::mat4(1.0f);
		// rotate on the x-axis to tilt image forward so you can see the top of it instead of a straight on view
		model = glm::rotate(model, (float)glfwGetTime() * glm::radians(-55.0f), glm::vec3(0.5f, 1.0f, 0.0f));
		// move the view backwards on z-axis so you can see more around the image
		view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
		// set the "camera" projection to have view angle of 45 degrees, set screen perspective, and set view distance
		projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		
		// get the matrix uniform locations for the vertex shader
		unsigned int modelLoc = glGetUniformLocation(threeDShader.ID, "model");
		unsigned int viewLoc = glGetUniformLocation(threeDShader.ID, "view");
		unsigned int projectionLoc = glGetUniformLocation(threeDShader.ID, "projection");

		// pass the matrix views to the shaders (three different ways)
		// using glm pointers to a matrix
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		// using c++ memory location pointers and array de-referencing
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);
		// using the built in shader function - hella useful need to go and add this into my shader.h file
		// threeDShader.setMat4("projection", projection);
		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

		// bind the vertex array again? Not sure why but see if that fixes the bug?
		glBindVertexArray(VAO);

		// loop to render multiple cubes in the space
		for (unsigned int i = 0; i < 10; i++) {
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, cubePositions[i]);
			float angle = 20.0f * i;
			model = glm::rotate(model, (float)glfwGetTime() * glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			// this overrides the model changes made on line 267 
			unsigned int modelLoc = glGetUniformLocation(threeDShader.ID, "model");
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			// render the triangles that make up the cube
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}		

		// swap out colors in the buffer based on updated user input
		glfwSwapBuffers(window);
		// check for any user input
		glfwPollEvents();
	}

	// optional: de-allocate all resources after done rendering
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	// glDeleteBuffers(1, &EBO);

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