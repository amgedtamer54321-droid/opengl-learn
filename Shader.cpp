//
// Created by mostafa on 7/4/2026.
//

#include "Shader.h"

#include <fstream>
#include <sstream>
#include <iostream>
#include <stdexcept>
#include <glm.hpp>

using namespace std;
using namespace glm;
void Shader::createProgram()
{
    programID = glCreateProgram();
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
}

string Shader::readShader(const string& path)
{
    ifstream file(path);

    if (!file.is_open())
        throw runtime_error("Cannot open shader: " + path);

    stringstream ss;
    ss << file.rdbuf();

    return ss.str();
}

bool Shader::readShaders(const string& vertexPath,
                         const string& fragmentPath)
{
    try
    {
        vertexShaderSource = readShader(vertexPath);
        fragmentShaderSource = readShader(fragmentPath);

        return true;
    }
    catch (const exception& e)
    {
        cout << e.what() << endl;
        return false;
    }
}

bool Shader::checkShader(GLuint shader,
                         const string& name)
{
    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        char infoLog[1024];
        glGetShaderInfoLog(shader, 1024, nullptr, infoLog);

        cout << "==========================" << endl;
        cout << name << " compile error" << endl;
        cout << infoLog << endl;
        cout << "==========================" << endl;

        return false;
    }

    return true;
}

bool Shader::checkProgram()
{
    GLint success;
    glGetProgramiv(programID, GL_LINK_STATUS, &success);

    if (!success)
    {
        char infoLog[1024];
        glGetProgramInfoLog(programID, 1024, nullptr, infoLog);

        cout << "==========================" << endl;
        cout << "Program Link Error" << endl;
        cout << infoLog << endl;
        cout << "==========================" << endl;

        return false;
    }

    return true;
}

void Shader::run()
{
    createProgram();

//    cout << "=========== Vertex Shader ===========" << endl;
//    cout << vertexShaderSource << endl;
//
//    cout << "========== Fragment Shader ==========" << endl;
//    cout << fragmentShaderSource << endl;

    const char* vertexSrc = vertexShaderSource.c_str();
    const char* fragmentSrc = fragmentShaderSource.c_str();

    glShaderSource(vertexShader, 1, &vertexSrc, nullptr);
    glShaderSource(fragmentShader, 1, &fragmentSrc, nullptr);

    glCompileShader(vertexShader);

    if (!checkShader(vertexShader, "Vertex Shader"))
        throw runtime_error("Vertex shader compilation failed.");

    glCompileShader(fragmentShader);

    if (!checkShader(fragmentShader, "Fragment Shader"))
        throw runtime_error("Fragment shader compilation failed.");

    glAttachShader(programID, vertexShader);
    glAttachShader(programID, fragmentShader);

    glLinkProgram(programID);

    if (!checkProgram())
        throw runtime_error("Program link failed.");

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

void Shader::use()
{

    Active();

    viewLoc =getUniformLocation("view");
    modelLoc = getUniformLocation("model");
    preLoc = getUniformLocation("pre");
    NmatLoc = getUniformLocation("Nmat");
    CampPosLoc = getUniformLocation("camPos");
    materialPos.ambient = getUniformLocation("material.ambient");
    materialPos.diffuse = getUniformLocation("material.diffuse");
    materialPos.specular = getUniformLocation("material.specular");
    materialPos.shininess = getUniformLocation("material.shininess");
    
    lightPos.ambient = getUniformLocation("light.ambient");
    lightPos.diffuse = getUniformLocation("light.diffuse");
    lightPos.specular = getUniformLocation("light.specular");
    lightPos.position = getUniformLocation("light.position");



}

GLuint Shader::getUniformLocation(const GLchar* name)
{
    return glGetUniformLocation(programID, name);
}

void Shader::setUniform(GLuint location,
                        float x,
                        float y,
                        float z)
{
    glUniform3f(location, x, y, z);
}
void Shader::setUniform(GLuint location,vec3 v)
{
    glUniform3f(location, v.x, v.y, v.z);
}
void Shader::setUniformf(GLuint location,
                        float x)
{
    glUniform1f(location, x);
}

void Shader::setUniformm(GLuint location,mat3 mat)
{
    glUniformMatrix3fv(location,1,GL_FALSE,value_ptr(mat));
}

Shader::~Shader()
{
    if (programID != 0)
        glDeleteProgram(programID);
}
void Shader::setModel(mat4 model, string type){
    GLuint refl;
    if(type == "model")
        refl= modelLoc;
    else if(type == "view")
        refl = viewLoc;
    else
        refl = preLoc;
    
    glUniformMatrix4fv( refl ,
                       1,
                       GL_FALSE,
                       glm::value_ptr(model));
}
void Shader::initMatrixs(){
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
    projection = glm::perspective(glm::radians(Zoom), 800.0f / 600.0f, 0.1f,
                                  100.0f);
    
    glUniformMatrix4fv(preLoc, 1, GL_FALSE, glm::value_ptr(projection));
    
    
}
void Shader::Active(){
    glUseProgram(programID);
}
//void initImge(const string  path, unsigned &usedtexture){
//    int imwidth, imheight, imnrChannels;
//    stbi_set_flip_vertically_on_load(true);
//    auto imdata = stbi_load(path.c_str(), &imwidth, &imheight, &imnrChannels, 0);
//    
//    if (!imdata) {
//        std::cout << "Failed to load texture: " << path << std::endl;
//        return;
//    }
//    
//    // Determine format dynamically
//    GLenum format = GL_RGB;
//    if (imnrChannels == 1) format = GL_RED;
//    else if (imnrChannels == 3) format = GL_RGB;
//    else if (imnrChannels == 4) format = GL_RGBA;
//    
//    glGenTextures(1, &usedtexture);
//    glBindTexture(GL_TEXTURE_2D, usedtexture);
//    
//    // Fix: Prevent artifacts/crashes due to non-4-byte aligned widths
//    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
//    
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
//    
//    // Use the dynamic format variable here
//    glTexImage2D(GL_TEXTURE_2D, 0, format, imwidth, imheight, 0, format, GL_UNSIGNED_BYTE, imdata);
//    glGenerateMipmap(GL_TEXTURE_2D);
//    
//    stbi_image_free(imdata);
//}
