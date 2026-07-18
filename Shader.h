#ifndef SHADER_H
#define SHADER_H

#include <GL/glew.h>
#include <string>
#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"
#include "glm.hpp"

class Shader
{
private:
    GLuint vertexShader = 0;
    GLuint fragmentShader = 0;
    GLuint programID = 0;
    GLuint aNormalLoc =0;
    



    std::string vertexShaderSource;
    std::string fragmentShaderSource;

    std::string readShader(const std::string& path);
    void createProgram();

    bool checkShader(GLuint shader, const std::string& name);
    bool checkProgram();

public:
    struct Material {
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float shininess;
    };

    struct Light {
     glm::vec3 position;
     glm::vec3 ambient;
     glm::vec3 diffuse;
     glm::vec3 specular;
    };
    
    struct LightPos {
     GLuint position;
     GLuint ambient;
     GLuint diffuse;
     GLuint specular;
    };
    
    struct MaterialPos {
     GLuint position;
     GLuint ambient;
     GLuint diffuse;
     GLuint specular;
     GLuint shininess;
    };
    
    MaterialPos materialPos;
    LightPos lightPos;

    Material material;
    Light light;
    GLint preLoc;
    GLint viewLoc;
    GLuint NmatLoc =0;
    GLuint CampPosLoc=0;
    float Zoom = 45.0f;
    GLint modelLoc;

    bool readShaders(const std::string& vertexPath,
                     const std::string& fragmentPath);

    void run();

    void use();

    GLuint getUniformLocation(const GLchar* name);

    void setUniform(GLuint location,
                    float x,
                    float y,
                    float z);

    GLuint getProgram() const
    {
        return programID;
    }
    
    void setModel(glm::mat4 model, std::string type);
    void initMatrixs();
    void Active();
    void setUniformm(GLuint location,glm::mat3 mat);
    void setUniformf(GLuint location,float x);
    void setUniform(GLuint location,glm::vec3 v);

    ~Shader();
};

#endif
