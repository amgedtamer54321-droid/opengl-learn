//
//  main.cpp
//  tt
//
//  Created by Mostafa Gamal Abdel Moniem on 31/05/2026.
//

//
#define STB_IMAGE_IMPLEMENTATION

#include "GL/glew.h"
#include "glm/vec3.hpp"
#include <fstream>
#include <iostream>
#include <fstream>
#include <sstream>
#include "stb_image.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <GLFW/glfw3.h>
#include "Shader.h"

using namespace std;
using namespace glm;

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
GLuint matLocation;
GLint preLoc;
GLint viewLoc;
GLint modelLoc;

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
}
void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}
void makeTransformation(){
    glm::mat4 trans = glm::mat4(1.0f);
    trans = glm::translate(trans, glm::vec3(0.5f, -0.5f, 0.0f));
    trans = glm::rotate(trans, (float)glfwGetTime(),
    glm::vec3(0.0f, 0.0f, 1.0f));
    glUniformMatrix4fv(matLocation,1, GL_FALSE, glm::value_ptr(trans));

}
void setModel(mat4 model){
    glUniformMatrix4fv(modelLoc,
                       1,
                       GL_FALSE,
                       glm::value_ptr(model));
}
void animaterotate(){
    glm::mat4 model = glm::mat4(1.0f);
    float e = abs(sin(glfwGetTime()));
    model = glm::rotate(model, radians(90.0f * e ) , vec3(1.0f,1.0f,1.0f));
    glUniformMatrix4fv(modelLoc,
                       1,
                       GL_FALSE,
                       glm::value_ptr(model));

}
void trasltetoz(){
    glm::mat4 view = glm::mat4(1.0f);
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
    glm::mat4 model(1.0f);

    model = glm::rotate(model,
                        glm::radians(60.0f),
                        glm::vec3(1.0f,1.0f,1.0f));

    glUniformMatrix4fv(modelLoc,
                       1,
                       GL_FALSE,
                       glm::value_ptr(model));
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

    glm::mat4 projection;
    projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f,
    100.0f);
    glUniformMatrix4fv(preLoc, 1, GL_FALSE, glm::value_ptr(projection));


}
void draw() {
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindVertexArray(vao);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture1);
//    glActiveTexture(GL_TEXTURE1);
//    glBindTexture(GL_TEXTURE_2D, texture2);

    vector<vec3> postions = {
            glm::vec3( 0.0f,  0.0f,  0.0f),
            glm::vec3( 2.0f,  5.0f, -15.0f),
            glm::vec3(-1.5f, -2.2f, -2.5f),
            glm::vec3(-3.8f, -2.0f, -12.3f),
            glm::vec3( 2.4f, -0.4f, -3.5f),
            glm::vec3(-1.7f,  3.0f, -7.5f),
            glm::vec3( 1.3f, -2.0f, -2.5f),
            glm::vec3( 1.5f,  2.0f, -2.5f),
            glm::vec3( 1.5f,  0.2f, -1.5f),
            glm::vec3(-1.3f,  1.0f, -1.5f)
    };
    for (int i = 0; i < postions.size(); ++i) {
        auto model = mat4 (1);
        auto moved = translate(model,postions[i]);
        setModel(moved);
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }

}



void createShader() {

    Shader shader;
    shader.readShaders(
            currentPath +"VertexShader.glsl",
            currentPath+"FragmentShader.glsl"
            );

    shader.run();

   // matLocation = glGetUniformLocation(program,"trans");
   // fragmentColorLocation = glGetUniformLocation(program, "color");
    auto textureLocation1 = shader.getUniformLocation(
    "texture1"
            );
    glUniform1i(textureLocation1, 0);
//    auto textureLocation2 = glGetUniformLocation(program, "texture2");
//    glUniform1i(textureLocation2, 1);
    viewLoc = shader.getUniformLocation("view");
    modelLoc =shader.getUniformLocation("model");
    preLoc = shader.getUniformLocation("pre");



}
void SendDatatToGL() {
    const float vertices[] = {
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f

    };
//    const unsigned int indecies[] ={
//        0, 1, 2, // المثلث الأول (الأيمن السفلي)
//        2, 3, 0  // المثلث الثاني (الأيسر العلوي)
//    };
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // glGenBuffers(1,&EBO);
   // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
   // glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indecies), indecies, GL_STATIC_DRAW);

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
   // glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, (5 * sizeof(float)),
                          (void *)0);
   // glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, (8 * sizeof(float)), (void *)(3 * sizeof(float)));
    
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, (5 * sizeof(float)), (void *)(3 * sizeof(float)));

    glBindVertexArray(0);
}
void initImge(const string  path, unsigned &usedtexture){
    int imwidth, imheight, imnrChannels;
    stbi_set_flip_vertically_on_load(true);
    auto imdata = stbi_load(path.c_str(), &imwidth, &imheight, &imnrChannels, 0);
    
    if (!imdata) {
        std::cout << "Failed to load texture: " << path << std::endl;
        return;
    }

    // Determine format dynamically
    GLenum format = GL_RGB;
    if (imnrChannels == 1) format = GL_RED;
    else if (imnrChannels == 3) format = GL_RGB;
    else if (imnrChannels == 4) format = GL_RGBA;

    glGenTextures(1, &usedtexture);
    glBindTexture(GL_TEXTURE_2D, usedtexture);
    
    // Fix: Prevent artifacts/crashes due to non-4-byte aligned widths
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    
    // Use the dynamic format variable here
    glTexImage2D(GL_TEXTURE_2D, 0, format, imwidth, imheight, 0, format, GL_UNSIGNED_BYTE, imdata);
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
    string imagePath = currentPath + "download.jpeg";
    initImge(imagePath,texture1);
   // initImge("download2.jpg",texture2);
    SendDatatToGL();
    trasltetoz();
    glEnable(GL_DEPTH_TEST);
    while (!glfwWindowShouldClose(window)) {
        glClearColor(1.0, 0, 0, 1);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
        //animaterotate();

        draw();
        processInput(window);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return EXIT_SUCCESS;
}
