#include "../include/glad/glad.h"
#include "../include/glfw3.h"

#include "logger.h"

int main() {
	GLFWwindow* window;

	/* Initialize the library */
	if(!glfwInit())
		return -1;

    //let glfw know the version of opengl we are using
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	/* Create a windowed mode windw and its OpenGL context */
	window = glfwCreateWindow(640, 480, "Hellow World", NULL, NULL);
	if(!window) {
		glfwTerminate();
        LOGERROR("GLFW failed to create window.");
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);
    glfwSwapInterval(1); //glfw will consistently swap buffers every 1 'tick' or whatever

    //load function pointers using glad
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        glfwTerminate();
        LOGERROR("GLAD loader failed to load function pointers.");
        return -1;
    }

    //TODO: glfw can process window resizing and keyboard/mouse input. Look at glfw3.h to see how to do this (or google)

    float positions[] = {0.5f, 0.5f,
                        -0.5f, 0.5f,
                        -0.5f,-0.5f};
          
    float indices[] = {0, 1, 2, 0, 3, 2};

//vertex buffer; create and bind
    unsigned int vbuf;
    glGenBuffers(1, &vbuf);
    glBindBuffer(GL_ARRAY_BUFFER, vbuf);
    glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), positions, GL_STATIC_DRAW);

//vertex array; create and bind
    //enable attrib that has index '0' (i.e. 'position' attribute)
    //this should be called before glVertexAttribPointer for the corresponding attribute
    //(I think? hehehe).
    glEnableVertexAttribArray(0); 
    //for 'position' attribute; again, note that GL is a big 'state machine' so it knows we are specifying
    //attributes for the currently bound buffer
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);

    //TODO: shader; write, compile, and bind

	/* loop until the user closes the window */
	while(!glfwWindowShouldClose(window)) {

		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //TODO: draw buffer/array using shader
        glDrawArrays(GL_TRIANGLES, 0, 3);

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

    //end glfw context
	glfwTerminate();
	return 0;
}
