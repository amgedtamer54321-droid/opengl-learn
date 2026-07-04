#ifndef SHADER_H
#define SHADER_H

#include <GL/glew.h>
#include <string>

class Shader
{
private:
    GLuint vertexShader;
    GLuint fragmentShader;
    GLuint programID;

    const char* vertexShaderSource;
    const char* fragmentShaderSource;

    char info[512];

    void createProgram();
    std::string readShader(std::string path);
    bool checkVertexShader();
    bool checkFragmentShader();
    bool checkProgramAttachment();

public:
    bool readShaders(std::string vertexShaderPath,
                     std::string fragmentShaderPath);

    void run();

    GLuint getUniformLocation(const GLchar* name);
};

#endif