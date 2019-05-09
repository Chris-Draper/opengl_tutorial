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

	// setup the vertex shader and fragment shader
	Shader textureShader("vShader.vs", "fShader.fs");

	// ----------------------------------------------------
	// Gen VAO, Gen VBO, Gen Attributes, Store VBO into VAO
	// VAO = Vertex Attribute Object
	// VBO = Vertex Buffer Object
	// ----------------------------------------------------

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float vertices[] = {
		// positions          // colors           // texture coords
		 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
		 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
		-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left 
	};
	unsigned int indices[] = {
		0, 1, 3, // first triangle
		1, 2, 3  // second triangle
	};

	// get id of the vertex buffer object
	unsigned int VBO, VAO, EBO;
	// generate the VAO, VBO, and EBO
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	// bind the VAO first, then bind VBO, then set vertex attributes
	glBindVertexArray(VAO);

	// set the type of the buffer - aka array buffer
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// fill the buffer with the vertices' data and say static draw is infrequent update
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// bind EBO and put index data into it
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// vertex position attribute
	// 0 is the location
	// 3 is the number of floats
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// vertex color attribute - 4th parameter is the offset to the color
	// first parameters are 1 here becuase that's the location set for color in the vertex shader
	// the last parameter is the data offset within the stride
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// texture attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

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
	textureShader.use();
	// manually set the uniform for texture
	glUniform1i(glGetUniformLocation(textureShader.ID, "texture1"), 0);
	// use the shader special method to set the texture
	textureShader.setInt("texture2", 1);

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

		// activate and bind the first texture unit
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);

		// activate and bind the second texture unit
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);

		// transform the texture container
		// create the identity matrix with diagonals of only 1's
		glm::mat4 transform = glm::mat4(1.0f);
		// translate the texture container right and down in the xy plane
		transform = glm::translate(transform, glm::vec3(0.5f, -0.5f, 0.0f));
		// rotation around the z-axis over time
		// glm::radians() takes input as degress and converts to radians - glm::radians(90.0f) is roate 90 degrees
		// must rotate around a unit vector so you always have to make sure to normalize first
		transform = glm::rotate(transform, (float)glfwGetTime(), glm::vec3(0.0, 0.0, 1.0));
		// multiply matrix by scalar to shrink the image on x, y, and z axis (even though only 2D)
		transform = glm::scale(transform, glm::vec3(0.5, 0.5, 0.5));

		// tell GL to use the new shader program whenever render is called
		textureShader.use();
		// now that the shader is active we pass the transformation matrix to it
		unsigned int transformLoc = glGetUniformLocation(textureShader.ID, "transform");
		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));

		// bind the vertex array again? Not sure why but see if that fixes the bug?
		glBindVertexArray(VAO);
		// render the triangle
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		// swap out colors in the buffer based on updated user input
		glfwSwapBuffers(window);
		// check for any user input
		glfwPollEvents();
	}

	// optional: de-allocate all resources after done rendering
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

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