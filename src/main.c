#include "../include/glad/glad.h"
#include "../include/glfw3.h"

#include "logger.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct shader_source {
    char* vertex_shader;
    char* fragment_shader;
} shader_source;

typedef enum shader_parse_mode {
    MODE_VERTEX = 0,
    MODE_FRAGMENT = 1
} shader_parse_mode;

//parses the source file into 2 shader sources (vert and frag)
//TODO: this function feels messsssyyyyy
shader_source parse_shader(const char* filepath) {
    FILE* fp = fopen(filepath, "r");
    char str[1024];
    shader_parse_mode p_mode = 0;

    char src[2][16384] = {"", ""};

    if(fp != NULL) {

    while(fgets(str,1024,fp)) {
        printf("Reading line: %s", str);
        if(strstr(str, "#shader") != NULL) {
            if(strstr(str, "vertex") != NULL)
                p_mode = MODE_VERTEX;
            if(strstr(str, "fragment") != NULL)
                p_mode = MODE_FRAGMENT;
        }
        else {
            strcat(src[p_mode], str);
        }
    }

    }
    else {
        printf("file failed to open.\n");
    }

    shader_source s_src;
    s_src.vertex_shader   = src[MODE_VERTEX];
    s_src.fragment_shader = src[MODE_FRAGMENT];
    return s_src;
}

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

//TODO: OpenGL error handling   

//TODO: abstract all of the stuff between here and the draw loop into functions or structs or whatever (can't do classes cuz it's not C++ lolllll)

//vertex data
    float positions[] = {0.5f, 0.5f,
                        -0.5f, 0.5f,
                        -0.5f,-0.5f};


//vertex buffer; create and bind
    unsigned int vbuf;
    glGenBuffers(1, &vbuf);
    glBindBuffer(GL_ARRAY_BUFFER, vbuf);
    glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), positions, GL_STATIC_DRAW);

//TODO: index buffers

//this is needed in modern OpenGL and using core profile. Basically, nothing will draw if you don't have a VAO bound
    //NOTE: this has to be specified before vertex attributes are specified (see next block of code)
    //TODO: there's more to this I guess? see cherno's 'vertex arrays in opengl' video
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

//vertex attributes
    //enable attrib that has index '0' (i.e. 'position' attribute)
    //this should be called before glVertexAttribPointer for the corresponding attribute
    //(I think? hehehe).
    glEnableVertexAttribArray(0); 
    //for 'position' attribute; again, note that GL is a big 'state machine' so it knows we are specifying
    //attributes for the currently bound buffer
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);

//shaders; write, compile, and bind 
    //TODO: uniforms

    
    //TODO: hardcoded string shaders... delete
    //const char* vertex_shader   = "#version 330 core\n\nlayout(location = 0) in vec4 position;\n\nvoid main()\n{\n    gl_Position = position;\n}\n";
    //const char* fragment_shader = "#version 330 core\n\nlayout(location = 0) out vec4 color;\n\nvoid main()\n{\n    color = vec4(1.0, 0.0, 1.0, 1.0);\n}\n";

    //create the shader program
    shader_source sources = parse_shader("C:/dev/opengl_c/res/shaders/basic.shader");
    printf("vertex:\n%s", sources.vertex_shader);
    printf("fragment:\n%s", sources.fragment_shader);

    unsigned int shader = create_shader(sources.vertex_shader, sources.fragment_shader);
    glUseProgram(shader);


//draw loop
	/* loop until the user closes the window */
	while(!glfwWindowShouldClose(window)) {

		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //draw triangle
        glDrawArrays(GL_TRIANGLES, 0, 3);

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

    glDeleteProgram(shader);

//end fw context
	glfwTerminate();
	return 0;
}
