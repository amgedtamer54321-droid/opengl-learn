//
// Created by mostafa on 7/4/2026.
//

#include "Shader.h"

#include "GL/glew.h"
#include <fstream>
#include <iostream>

#include <sstream>
#include <glfw3.h>

using namespace std;


    void  Shader::createProgram(){
        programID = glCreateProgram();
        vertexShader = glCreateShader(GL_VERTEX_SHADER);
        fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    }

std::string Shader::readShader(const std::string& path)
{
    std::ifstream file(path);

    if (!file.is_open())
        throw std::runtime_error("Cannot open " + path);

    std::stringstream ss;
    ss << file.rdbuf();

    return ss.str();
}

    bool  Shader::checkVertexShader(){
        int Sucess;
        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &Sucess);
        if (!Sucess) {
            glGetShaderInfoLog(vertexShader, 512, 0, info);
            cout << "vertex Shader info Log : \n" << info << "\n";
        }
        if (Sucess) return true; else return false;
    }

    bool Shader::checkFragmentShader(){
        int Sucess = 0;
        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &Sucess);
        if (!Sucess) {
            glGetShaderInfoLog(fragmentShader, 512, 0, info);
            cout << "fragment Shader info Log : \n" << info << "\n";
        }
        if (Sucess) return true; else return false;
    }


    bool Shader::checkProgramAttachment(){
        int Sucess = 0;
        glGetProgramiv(programID, GL_LINK_STATUS, &Sucess);
        if (!Sucess) {
            glGetProgramInfoLog(programID, 512, NULL, info);
            std::cout << "Program link error:\n" << info << '\n';
        }
        if (Sucess) return true; else return false;
    }






    bool Shader::readShaders( string vertexShaderPath,
                      string fragmentShaderPath
                     ){

        bool isSucess;
        try {
            fragmentShaderSource = readShader(fragmentShaderPath).c_str();

            vertexShaderSource = readShader(vertexShaderPath).c_str();

            isSucess = true;
        }catch (exception e){
            isSucess = false;
        }
        return isSucess;
    }

    void Shader::run() {

        createProgram();
        const char* vertexSrc = vertexShaderSource.c_str();
        const char* fragmentSrc = fragmentShaderSource.c_str();
        glShaderSource(vertexShader, 1, &vertexSrc, nullptr);
        glShaderSource(fragmentShader, 1, &fragmentSrc, NULL);

        glCompileShader(vertexShader);
        glCompileShader(fragmentShader);
        checkFragmentShader();
        checkVertexShader();

        glAttachShader(programID, vertexShader);
        glAttachShader(programID, fragmentShader);
        glLinkProgram(programID);
        checkProgramAttachment();

        glUseProgram(programID);

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
    };

    GLuint Shader::getUniformLocation(const GLchar *name){
        return glGetUniformLocation(programID,name);
    }

