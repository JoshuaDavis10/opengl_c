#include "../include/glad/glad.h"
#include "../include/glfw3.h"

#include "logger.h"

#include <string.h>
#include <stdlib.h>

//compiles the shader given by src_str.
//@param 'type' specifies the type of shader to compile
unsigned int compile_shader(unsigned int type, const char* src_str) {
    
//compile shader
    //glCreateShader returns an ID or 'handle' to a shader of type 'type'
    unsigned int shader = glCreateShader(type);

    /* assings a set of string as the source code of 'shader'
     * in this case we'll just be using one string: src_str
     * passing NULL for 'length' param (see docs.gl) tells OpenGL to assume
     * that the string in src_str is null terminated, which hopefully it is
     * unless you screwed up bad lol
     */
    glShaderSource(shader, 1, &src_str, NULL);

    //compiles the shader with the given ID or 'handle'
    glCompileShader(shader);

//error handling 
    //get compile status (either GL_TRUE or GL_FALSE; true if previous compile operation was successfull
    //false if not)
    int compile_status;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compile_status);

    //if GL_FALSE, previous compile operation was unsuccessful
    //respond accordingly
    if(compile_status == GL_FALSE) {

        //get length of info log message
        int length;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length); 

        //get info log message
        char* error_msg = alloca(sizeof(char) * length);
        glGetShaderInfoLog(shader, length, &length, error_msg);

        //print the error message
        LOGERROR("Failed to compile shader. Info log message on line below:");
        LOGERROR(error_msg);

        //delete the shader once compilation is complete
        glDeleteShader(shader);

        //return 0 (i.e. no shader)
        return 0;
    }

    //return handle to shader ('ID')
    return shader;

}

unsigned int create_shader(const char* vert, const char* frag) {
    unsigned int program = glCreateProgram();

    //compile frag and vert shaders
    unsigned int vert_shader = compile_shader(GL_VERTEX_SHADER  , vert);
    unsigned int frag_shader = compile_shader(GL_FRAGMENT_SHADER, frag);

    //attach frag and vert shaders
    glAttachShader(program, vert_shader);
    glAttachShader(program, frag_shader);

    //link and validate program
    glLinkProgram(program);
    glValidateProgram(program);

    //delete frag and vert shaders
    glDeleteShader(vert_shader);
    glDeleteShader(frag_shader);

    //return the shader program
    return program;
}

int main() {

//GLFW/GLAD initialization stuff starts here

	GLFWwindow* window;

	/* Initialize the library */
	if(!glfwInit())
		return -1;

    //let glfw know the version of opengl we are using
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	/* Create a windowed mode windw and its OpenGL context */
	window = glfwCreateWindow(640, 480, "OpenGL C Test", NULL, NULL);
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

//GLFW/GLAD initialization stuff ends here


    //TODO: glfw can process window resizing and keyboard/mouse input. Look at glfw3.h to see how to do this (or google)


//vertex data
    float positions[] = {0.5f, 0.5f,
                        -0.5f, 0.5f,
                        -0.5f,-0.5f};

//vertex buffer; create and bind
    unsigned int vbuf;
    glGenBuffers(1, &vbuf);
    glBindBuffer(GL_ARRAY_BUFFER, vbuf);
    glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), positions, GL_STATIC_DRAW);

//vertex attributes
    //enable attrib that has index '0' (i.e. 'position' attribute)
    //this should be called before glVertexAttribPointer for the corresponding attribute
    //(I think? hehehe).
    glEnableVertexAttribArray(0); 
    //for 'position' attribute; again, note that GL is a big 'state machine' so it knows we are specifying
    //attributes for the currently bound buffer
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);

//this is needed in modern OpenGL and using core profile. Basically, nothing will draw if you don't have a VAO bound
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

//shaders; write, compile, and bind 
    //shaders
    const char* vertex_shader   = "#version 330 core\n\nlayout(location = 0) in vec4 position;\n\nvoid main()\n{\n    gl_Position = position;\n}\n";
    const char* fragment_shader = "#version 330 core\n\nlayout(location = 0) out vec4 color;\n\nvoid main()\n{\n    color = vec4(0.3, 1.0, 0.8, 1.0);\n}\n";

    //create the shader program
    unsigned int shader = create_shader(vertex_shader, fragment_shader);
    glUseProgram(shader);

//draw loop
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

//end fw context
	glfwTerminate();
	return 0;
}
