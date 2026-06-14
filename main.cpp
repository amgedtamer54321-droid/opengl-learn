
#include <cstdlib>
#include "GL/glew.h"
#include  "glm/vec3.hpp"
#include "GLFW/glfw3.h"
#include "iostream"
using namespace std;
using namespace glm;


const GLchar * vertexShader = "#version 460 core \n\r"
                            "layout (location = 0) in  vec3 v_pos; \n"
                            "layout (location = 1) in vec3 v_color; \n"
                            "out vec3 f_color; \n"
                            "void main()\n"
                            "{\n"
                            "gl_Position  = vec4(v_pos,1.0);\n"
                            "f_color = v_color;"
                            "}";
const GLchar* fragmentShader = "#version 460 core\n"
                               "in vec3 f_color;\n"
                               "out vec4 FragColor;\n"
                               "\n"
                               "void main()\n"
                               "{\n"
                               "    FragColor = vec4(f_color, 1.0);\n"
                               "}";

GLuint vbo;
GLuint vao;
GLuint programId;
void display()
{
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(programId);

    glBindVertexArray(vao);

    glDrawArrays(GL_TRIANGLES, 0, 3);
}
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
    cout<<width<<"  "<<height<<endl;
}

void createProgram()
{
    GLint success;
    char infoLog[512];

    GLuint vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &vertexShader, nullptr);
    glCompileShader(vs);

    glGetShaderiv(vs, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        glGetShaderInfoLog(vs, 512, nullptr, infoLog);
        std::cout << infoLog << std::endl;
    }

    GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &fragmentShader, nullptr);
    glCompileShader(fs);

    glGetShaderiv(fs, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        glGetShaderInfoLog(fs, 512, nullptr, infoLog);
        std::cout << infoLog << std::endl;
    }

     programId = glCreateProgram();

    glAttachShader(programId, vs);
    glAttachShader(programId, fs);

    glLinkProgram(programId);

    glGetProgramiv(programId, GL_LINK_STATUS, &success);

    if (!success)
    {
        glGetProgramInfoLog(programId, 512, nullptr, infoLog);
        std::cout << infoLog << std::endl;
    }

    glDeleteShader(vs);
    glDeleteShader(fs);
}
void createVertix() {
    GLfloat data[] =
            {

                    -1.0f,-1.0f,0.0f,     0.0f,1.0f,0.0f,
                    1.0f, 1.0f,0.0f,     0.0f,1.0f,0.0f,
                    0.0f, 1.0f,0.0f,     0.0f,1.0f,0.0f
            };



    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    glBufferData(
            GL_ARRAY_BUFFER,
                 sizeof(data),
                 data,
                 GL_STATIC_DRAW
                 );
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glVertexAttribPointer(
            0,
            3,
            GL_FLOAT,
            GL_FALSE,
            6 * sizeof(float),
            (void*)nullptr);

    glEnableVertexAttribArray(0);

    glVertexAttribPointer(
            1,
            3,
            GL_FLOAT,
            GL_FALSE,
            6 * sizeof(float),
            (void*)(3 * sizeof(float)));

    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);


}


int main(int argc, char **argv) {

    if (!glfwInit()) {
        exit(EXIT_FAILURE);
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    GLFWwindow *window = glfwCreateWindow(600, 600, "Chapter2 - program1", nullptr, nullptr);
    glfwMakeContextCurrent(window);
    if (glewInit() != GLEW_OK) { exit(EXIT_FAILURE); }
    glfwSwapInterval(1);
    glfwSetFramebufferSizeCallback(window,framebuffer_size_callback);
    glClearColor(1.0, 0.0, 0.0, 1.0);
    glViewport(0,0,600,600);
    createProgram();
    createVertix();
    while (!glfwWindowShouldClose(window)) {
        display();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}