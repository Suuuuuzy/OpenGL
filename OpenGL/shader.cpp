//
//  shader.cpp
//  OpenGL
//
//  Created by 余蒹葭 on 2018/6/19.
//  Copyright © 2018年 余蒹葭. All rights reserved.
//

#include "shader.hpp"
#pragma  once


 // 看如何凭借两个文件，来构造出一个shader program，编译链接什么的都做好了嘻嘻
//   这个看懂了就问题不大了   后面色set value大概是传递参数的吧，到后面再说
Shader::Shader(const GLchar *vertexPath, const GLchar *fragmentPath, const GLchar *tcsPath, const GLchar *tesPath, const GLchar *geometryPath)
{
    // Step 1
    // from the path, get the vertex/fragment shader code
    std::string vertexCode;
    std::string fragmentCode;
    std::string tcsCode;
    std::string tesCode;
    std::string geometryCode;
    std::ifstream vShaderFile; // 一般自己定义的也就是 vs和fs   了
    std::ifstream fShaderFile;
    std::ifstream tcShaderFile; // 这几个shader几乎不会用到的
    std::ifstream teShaderFile;
    std::ifstream gShaderFile;
    
    // ensure ifstream object can catch exceptions
    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    tcShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    teShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    gShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        // open files
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);
        std::stringstream vShaderStream, fShaderStream;
        
        // read file content to data stream
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();
        
        // close files
        vShaderFile.close();
        fShaderFile.close();
        
        // convert data stream to string
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
        
        // try to open TESS  Control  Shader
        if (tcsPath != nullptr)
        {
            tcShaderFile.open(tcsPath);
            std::stringstream tcShaderStream;
            tcShaderStream << tcShaderFile.rdbuf();
            tcShaderFile.close();
            tcsCode = tcShaderStream.str();
        }
        // try to open TESS Evaluation Shader
        if (tesPath != nullptr)
        {
            teShaderFile.open(tesPath);
            std::stringstream teShaderStream;
            teShaderStream << teShaderFile.rdbuf();
            teShaderFile.close();
            tesCode = teShaderStream.str();
        }
        // try to open geometry shader
        if (geometryPath != nullptr)
        {
            gShaderFile.open(geometryPath);
            std::stringstream gShaderStream;
            gShaderStream << gShaderFile.rdbuf();
            gShaderFile.close();
            geometryCode = gShaderStream.str();
        }
    }
    catch (std::ifstream::failure &e)
    {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ" << std::endl;
    }
    const char *vShaderCode = vertexCode.c_str();
    const char *fShaderCode = fragmentCode.c_str();
    
    // Step 2
    // compile shaders, 对两个shader进行编译连接
    GLuint vertex, fragment;
    
    // vertex shader
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);
    checkCompileErrors(vertex, "VERTEX");
    
    // fragment shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    checkCompileErrors(fragment, "FRAGMENT");
    
    // if TESS  Control  Shader is given, also compile it
    GLuint tcs;
    if (tcsPath != nullptr)
    {
        const char *tcsShaderCode = tcsCode.c_str();
        tcs = glCreateShader(GL_TESS_CONTROL_SHADER);
        glShaderSource(tcs, 1, &tcsShaderCode, NULL);
        glCompileShader(tcs);
        checkCompileErrors(tcs, "TESS_CONTROL");
    }
    // if TESS Evaluation Shader is given, also compile it
    GLuint tes;
    if (tesPath != nullptr)
    {
        const char *tesShaderCode = tesCode.c_str();
        tes = glCreateShader(GL_TESS_EVALUATION_SHADER);
        glShaderSource(tes, 1, &tesShaderCode, NULL);
        glCompileShader(tes);
        checkCompileErrors(tes, "TESS_EVALUATION");
    }
    // if geometry shader is given, also compile it
    GLuint geometry;
    if (geometryPath != nullptr)
    {
        const char *gShaderCode = geometryCode.c_str();
        geometry = glCreateShader(GL_GEOMETRY_SHADER);
        glShaderSource(geometry, 1, &gShaderCode, NULL);
        glCompileShader(geometry);
        checkCompileErrors(geometry, "GEOMETRY");
    }
    
    // shader program
    // build a shader program
    ID = glCreateProgram();
    //link
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    if (tcsPath != nullptr)
    {
        glAttachShader(ID, tcs);
    }
    if (tesPath != nullptr)
    {
        glAttachShader(ID, tes);
    }
    if (geometryPath != nullptr)
    {
        glAttachShader(ID, geometry);
    }
    glLinkProgram(ID);
    checkCompileErrors(ID, "PROGRAM");
    
    // delete shader after they're linked into the shader program.
    // 在编译完成之后删除这两个对象
    glDeleteShader(vertex);
    glDeleteShader(fragment);
    if (tcsPath != nullptr)
    {
        glDeleteShader(tcs);
    }
    if (tesPath != nullptr)
    {
        glDeleteShader(tes);
    }
    if (geometryPath != nullptr)
    {
        glDeleteShader(geometry);
    }
}

Shader::~Shader()
{
}

// use the program
GLvoid Shader::use()
{
    glUseProgram(ID);
}

// uniform tool function
// 是用来向shader里面的uniform参数传递变量的函数
// glGetUniformLocation 用来找到这个变量的位置，value是要赋给这个变量的值
// 但是，这里好像没有对layout进行传参的函数？？？
// 难道glArrtibutePointer是在外面用的？？？
// ------------------------------------------------------------------------
GLvoid Shader::setBool(const std::string &name, GLboolean value) const
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), (GLint)value);
}
// ------------------------------------------------------------------------
GLvoid Shader::setInt(const std::string &name, GLint value) const
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}
// ------------------------------------------------------------------------
GLvoid Shader::setFloat(const std::string &name, GLfloat value) const
{
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}
// ------------------------------------------------------------------------
GLvoid Shader::setVec2(const std::string &name, const glm::vec2 &value) const
{
    glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
}
GLvoid Shader::setVec2(const std::string &name, GLfloat x, GLfloat y) const
{
    glUniform2f(glGetUniformLocation(ID, name.c_str()), x, y);
}
// ------------------------------------------------------------------------
GLvoid Shader::setVec3(const std::string &name, const glm::vec3 &value) const
{
    glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
}
GLvoid Shader::setVec3(const std::string &name, GLfloat x, GLfloat y, GLfloat z) const
{
    glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
}
// ------------------------------------------------------------------------
GLvoid Shader::setVec4(const std::string &name, const glm::vec4 &value) const
{
    glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
}
GLvoid Shader::setVec4(const std::string &name, GLfloat x, GLfloat y, GLfloat z, GLfloat w) const
{
    glUniform4f(glGetUniformLocation(ID, name.c_str()), x, y, z, w);
}
// ------------------------------------------------------------------------
GLvoid Shader::setMat2(const std::string &name, const glm::mat2 &value) const
{
    glUniformMatrix2fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &value[0][0]);
}
// ------------------------------------------------------------------------
GLvoid Shader::setMat3(const std::string &name, const glm::mat3 &value) const
{
    glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &value[0][0]);
}
// ------------------------------------------------------------------------
GLvoid Shader::setMat4(const std::string &name, const glm::mat4 &value) const
{
    glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &value[0][0]);
}

// 检查是否有编译错误的函数
GLvoid Shader::checkCompileErrors(GLuint shader, const std::string &type)
{
    GLint success;
    GLchar infoLog[1024];
    if (type != "PROGRAM")
    {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << std::endl;
            std::cout << infoLog << std::endl;
            std::cout << "-------------------------------------------------------" << std::endl;
        }
    }
    else
    {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << std::endl;
            std::cout << infoLog << std::endl;
            std::cout << "-------------------------------------------------------" << std::endl;
        }
    }
}
