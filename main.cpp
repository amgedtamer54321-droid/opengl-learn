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
unsigned int EBO;

float incr = 0.0;
GLuint vao;
GLuint refColor;
GLuint fragmentColorLocation;
unsigned char *imageData;
unsigned char *imageData2;
int width, height, nrChannels;
GLuint texture1;
GLuint texture2;

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
}
void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}
void draw() {
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindVertexArray(vao);
   // glActiveTexture(GL_TEXTURE0);
   // glBindTexture(GL_TEXTURE_2D, texture1);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture2);
    cout<<"-------2344   "<<glGetError()<<endl;
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
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
    
    glUseProgram(program);
    
    fragmentColorLocation = glGetUniformLocation(program, "color");
    auto textureLocation1 = glGetUniformLocation(program, "texture1");
    glUniform1i(textureLocation1, 0);
    auto textureLocation2 = glGetUniformLocation(program, "texture2");
    glUniform1i(textureLocation2, 1);
}
void SendDatatToGL() {
    const float vertices[] = {

     -0.5f, 0.5f, 0.0f,            1.0f, 0.0f, 0.0f,         1.0f, 1.0f, // top right
     0.5f, 0.5f, 0.0f,       0.0f, 1.0f, 0.0f,               0.0f, 1.0f, // bottom right
     0.5f, -0.5f, 0.0f,       0.0f, 0.0f, 1.0f,             0.0f, 0.0f, // bottom left
    -0.5f, -0.5f, 0.0f,          1.0f, 1.0f, 0.0f,           1.0f, 0.0f // top left
    };
    const unsigned int indecies[] ={
        0, 1, 2, // المثلث الأول (الأيمن السفلي)
        2, 3, 0  // المثلث الثاني (الأيسر العلوي)
    };
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glGenBuffers(1,&EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indecies), indecies, GL_STATIC_DRAW);

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, (8 * sizeof(float)),
                          (void *)0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, (8 * sizeof(float)), (void *)(3 * sizeof(float)));
    
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, (8 * sizeof(float)), (void *)(6 * sizeof(float)));

    glBindVertexArray(0);
}
void initImge(const char* path,unsigned &usedtexture){
    int imwidth,imheight,imnrChannels;
    stbi_set_flip_vertically_on_load(true);
    auto imdata = stbi_load(path, &imwidth, &imheight, &imnrChannels, 0);
    glGenTextures(1, &usedtexture);
    glBindTexture(GL_TEXTURE_2D, usedtexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imwidth, imheight, 0 , GL_RGB,GL_UNSIGNED_BYTE, imdata);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(imdata);
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
    initImge("download.jpeg",texture1);
    initImge("download2.jpg",texture2);

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
