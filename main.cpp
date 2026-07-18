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
#include <iostream>
#include <fstream>
#include <sstream>
#include "stb_image.h"
#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"
#include <glfw3.h>
#include "Shader.h"
#include "mySystemPath.h"

using namespace std;
using namespace glm;
unsigned int VBO;
unsigned int EBO;

float incr = 0.0;
GLuint vao;
GLuint refColor;
unsigned char *imageData;
unsigned char *imageData2;
int width, height, nrChannels;
GLuint texture1;
GLuint texture2;
GLuint matLocation;
float pitch1 =0 , yaw1 = -90;
GLuint lightVao;

const float speed = .5f;
const vec3 upVector = vec3(0,1,0);
vec3 frontCam = vec3(0,0,-1);
vec3 cameraPos = vec3(0,0,3);
float radus =0;

Shader objectShader;
Shader lightShader;

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, 10, 10);
}

float lastX = 400, lastY = 300;
bool firstMouse = true;
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }
    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;
    float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;
    yaw1 += xoffset;
    
    pitch1 += yoffset;
    if(pitch1 > 89.0f)
        pitch1 = 89.0f;
    if(pitch1 < -89.0f)
        pitch1 = -89.0f;
    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw1)) * cos(glm::radians(pitch1));
    direction.y = sin(glm::radians(pitch1));
    direction.z = sin(glm::radians(yaw1)) * cos(glm::radians(pitch1));
    frontCam = glm::normalize(direction);
    
    auto look = glm::lookAt(cameraPos, cameraPos + frontCam, upVector);
    objectShader.use();
    objectShader.setModel(look, "view");
    lightShader.use();
    lightShader.setModel(look, "view");

}
void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_A)==GLFW_PRESS) {
        cameraPos -= normalize(cross(frontCam,upVector))* speed;
    }
    if (glfwGetKey(window, GLFW_KEY_D)==GLFW_PRESS) {
        cameraPos += normalize(cross(frontCam,upVector))* speed;
    }
    if (glfwGetKey(window, GLFW_KEY_W)==GLFW_PRESS) {
        cameraPos += upVector * speed;
    }
    if (glfwGetKey(window, GLFW_KEY_S)==GLFW_PRESS) {
        cameraPos -= upVector * speed;
    }
    auto look = glm::lookAt(cameraPos, cameraPos + frontCam, upVector);
    objectShader.Active();
    objectShader.setModel(look, "view");
    lightShader.Active();
    lightShader.setModel(look, "view");
}

void draw() {
    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
   // glActiveTexture(GL_TEXTURE0);
   // glBindTexture(GL_TEXTURE_2D, texture1);
    //    glActiveTexture(GL_TEXTURE1);
    //    glBindTexture(GL_TEXTURE_2D, texture2);
    
    vector<vec3> postions = {
        glm::vec3( 0.0f,  0.0f,  -5.0f ),
        glm::vec3( 2.0f,  5.0f, -13.0f + sin(glfwGetTime()) * 20 ),
//        glm::vec3(-1.5f, -2.2f, -2.5f),
//        glm::vec3(-3.8f, -2.0f, -12.3f),
//        glm::vec3( 2.4f, -0.4f, -3.5f),
//        glm::vec3(-1.7f,  3.0f, -7.5f),
//        glm::vec3( 1.3f, -2.0f, -2.5f),
//        glm::vec3( 1.5f,  2.0f, -2.5f),
//        glm::vec3( 1.5f,  0.2f, -1.5f),
//        glm::vec3(-1.3f,  1.0f, -1.5f)
    };
        objectShader.Active();
    
        objectShader.setUniform(objectShader.getUniformLocation("objectColor") , 1.0f, 0.5f, 0.31f);
        objectShader.setUniform(objectShader.getUniformLocation("lightColor") , 1.0f, 1.0f, 1.0f);
        objectShader.setUniform(objectShader.materialPos.ambient, 1.0f, 0.5f, 0.31f);
        objectShader.setUniform(objectShader.materialPos.diffuse, 1.0f, 0.5f, 0.31f);
        objectShader.setUniform(objectShader.materialPos.specular, 0.5f, 0.5f, 0.5f);
        objectShader.setUniformf(objectShader.materialPos.shininess, 32.0f);
        objectShader.setUniform(objectShader.lightPos.position, postions[1].x, postions[1].y, postions[1].z);
        objectShader.setUniform(objectShader.lightPos.ambient,  glm::vec3(0.2f, 0.2f, 0.2f)); // خافت جداً ليعطي ظلالاً عميقة
        objectShader.setUniform(objectShader.lightPos.diffuse,  glm::vec3(0.5f, 0.5f, 0.5f)); // شدة متوسطة لكي لا يحترق لون الصندوق
        objectShader.setUniform(objectShader.lightPos.specular, glm::vec3(1.0f, 1.0f, 1.0f));
        auto look = lookAt(cameraPos, cameraPos + frontCam, upVector);
        objectShader.setModel(look, "view");
        objectShader.setUniform(objectShader.CampPosLoc, cameraPos.x,  cameraPos.y,  cameraPos.z);
        auto model = mat4 (1);
        mat4 moved = translate(model,postions[0]);
        mat3 normalMat = glm::transpose(glm::inverse(mat3(model)));
    
    objectShader.setUniformm(objectShader.NmatLoc,normalMat);
        glUniform3f(objectShader.getUniformLocation("lightPos"),postions[1].x,postions[1].y,postions[1].z);
        objectShader.setModel(moved,"model");
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(lightVao);

        lightShader.Active();

         model = mat4 (1);
         moved = translate(model,postions[1]);
        lightShader.setModel(moved,"model");
        lightShader.setModel(look,"view");

        glDrawArrays(GL_TRIANGLES, 0, 36);
}



void createShader(string Vshader , string FShader , Shader &shader) {
    
    shader.readShaders(currentPath +Vshader,currentPath+FShader);
    
    shader.run();
}
void SendDatatToGL() {
    const float vertices[] ={
        // positions // normals // texture coords
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,  0.0f, 0.0f,
        0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,   1.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f,    1.0f, 1.0f,
        0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f,    1.0f, 1.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f,   0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,   0.0f, 0.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,     0.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,       1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,     1.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,     1.0f, 1.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,    0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,    0.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f,      1.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f   , 1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f
        };

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // glGenBuffers(1,&EBO);
    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    // glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indecies), indecies, GL_STATIC_DRAW);
    
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
     glEnableVertexAttribArray(1);
   // glEnableVertexAttribArray(2);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, (8 * sizeof(float)),(void *)0);
    
     glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, (8 * sizeof(float)), (void *)(3 * sizeof(float)));
    
   // glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, (5 * sizeof(float)), (void *)(3 * sizeof(float)));
    
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glGenVertexArrays(1, &lightVao);
    glBindVertexArray(lightVao);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, (8 * sizeof(float)),(void *)0);
    glBindVertexArray(0);

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
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    cout<<xoffset << "  "<<yoffset<<endl;
objectShader.Zoom -= (float)yoffset;
    if (objectShader.Zoom  < -5.0f)
    objectShader.Zoom = -5.0f;
if (objectShader.Zoom > 45.0f)
    objectShader.Zoom = 45.0f;
    
    glm::mat4 projection;
    projection = glm::perspective(glm::radians(objectShader.Zoom), 800.0f / 600.0f, 0.1f,
                                  100.0f);
    glUniformMatrix4fv(objectShader.preLoc, 1, GL_FALSE, glm::value_ptr(projection));
}

int main(int argc, const char *argv[]) {
    
    
    std::filesystem::path current_path = std::filesystem::current_path();
    
    // Print the path
    std::cout << "Current path: " << current_path << std::endl;
    
    GLFWwindow *window;
    InitGLFW(window);
    glewInit();
    
     createShader("VertexShader.glsl","FragmentShader.glsl",objectShader);
     createShader("VlightShader.glsl","LightShader.glsl",lightShader);

    //string imagePath = currentPath + "download.jpeg";
    //initImge(imagePath,texture1);
    // initImge("download2.jpg",texture2);
    SendDatatToGL();
    
    objectShader.use();

    objectShader.initMatrixs();
    lightShader.use();
    lightShader.initMatrixs();
    glEnable(GL_DEPTH_TEST);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.0, 0.0, 0, 1);
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
