//
//  main.cpp
//  tt
//
//  Created by Mostafa Gamal Abdel Moniem on 31/05/2026.
//

//
#define STB_IMAGE_IMPLEMENTATION

#include "GL/glew.h"
#include "vec3.hpp"
#include <fstream>
#include <glfw3.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <stb_image.h>
using namespace std;
unsigned int VBO;
float incr = 0.0;
GLuint vao;
GLuint refColor;
GLuint fragmentColorLocation;
unsigned char *imageData;
int width, height, nrChannels;
GLuint texture;
void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
}
void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}
void draw() {
    incr += 1;
    auto time = glfwGetTime() * incr;
    glUniform1f(fragmentColorLocation, sin(time));
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, 3);
}

string readShader(string path){
    ifstream file(path);
    
    
    if (!file.is_open()) {
        throw runtime_error("Failed to open file: " + path);
    }
    
    stringstream filestream;
    
    filestream<<file.rdbuf();
    file.close();
    return filestream.str();
    
}

void createShader() {
    unsigned int vertexShader;
    unsigned int fragmentShader;
    auto program = glCreateProgram();
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    
    const char* vertexShaderSource;
    const char* framgentShaderSource;
    string temp ="";
    temp = readShader("VertexShader.glsl");
    vertexShaderSource = temp.c_str();
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    temp = readShader("FragmentShader.glsl");
    framgentShaderSource = temp.c_str();



    glShaderSource(fragmentShader, 1, &framgentShaderSource, NULL);


    glCompileShader(vertexShader);
    glCompileShader(fragmentShader);

    char info[512];
    int Sucess;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &Sucess);
    if (!Sucess) {
        glGetShaderInfoLog(vertexShader, 512, 0, info);
        cout << "vertex Shader info Log : \n" << info << "\n";
    }
    Sucess = 0;
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &Sucess);
    if (!Sucess) {
        glGetShaderInfoLog(fragmentShader, 512, 0, info);
        cout << "fragment Shader info Log : \n" << info << "\n";
    }
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);

    glLinkProgram(program);
    Sucess = 0;
    glGetProgramiv(program, GL_LINK_STATUS, &Sucess);
    if (!Sucess) {
        glGetProgramInfoLog(program, 512, NULL, info);
        std::cout << "Program link error:\n" << info << '\n';
    }
    fragmentColorLocation = glGetUniformLocation(program, "color");

    glUseProgram(program);
}
void SendDatatToGL() {
    const float vertices[] = {
        0.0f, 0.0f, // lower-left corner
        1.0f, 0.0f, // lower-right corner
        0.5f, 1.0f // top-center corner
    };
    const unsigned int indices[] = {
        // note that we start from 0!
        0, 1, 2, // first triangle
    };
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    //glEnableVertexAttribArray(1);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, (2 * sizeof(float)),
                          (void *)0);
    //glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, (6 * sizeof(float)),
                       //   (void *)(3 * sizeof(float)));
    glBindVertexArray(0);
}
void initImge(){
    imageData = stbi_load("download.jpeg", &width, &height, &nrChannels, 0);
    glGenTextures(1, &texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_UNSIGNED_BYTE, GL_RGB, imageData);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(imageData);
}
void InitGLFW(GLFWwindow *&window) {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSwapInterval(1);
}

int main(int argc, const char *argv[]) {
    

   
    GLFWwindow *window;
    InitGLFW(window);
    glewInit();
    createShader();
    initImge();
    SendDatatToGL();
    while (!glfwWindowShouldClose(window)) {
        glClearColor(1.0, 0, 0, 1);
        glClear(GL_COLOR_BUFFER_BIT);
        draw();
        processInput(window);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return EXIT_SUCCESS;
}
