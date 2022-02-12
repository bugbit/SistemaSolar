#include "shaders.h"

GLuint ShaderProgram::loadShader(GLenum type, const char *shaderSrc)
{
    GLuint shader;
    GLint compiled;

    shader = glCreateShader(type);
    if (!shader)
        return 0;

    glShaderSource(shader, 1, &shaderSrc, NULL);
    glCompileShader(shader);
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
    if (!compiled)
    {
        GLint infoLen = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
        if (infoLen > 1)
        {
            char *infoLog = static_cast<char *>(malloc(sizeof(char) * infoLen));

            glGetShaderInfoLog(shader, infoLen, NULL, infoLog);
            // printf("Error linking program:\n%s\n", infoLog);
            std::cerr << "Error linking program:" << std::endl
                      << infoLog << std::endl;
            free(infoLog);
        }
        glDeleteShader(shader);
        return 0;
    }

    shaders.push_back(shader);

    return shader;
}

GLuint ShaderProgram::loadShaderFromFile(GLenum type, const char *shaderFile)
{
    std::ifstream ifs;

    // Load the shader source file.
    ifs.open(shaderFile);

    if (!ifs)
    {
        std::cerr << "Can not open shader file: \"" << shaderFile << "\"" << std::endl;

        return 0;
    }

    std::string source(std::istreambuf_iterator<char>(ifs), (std::istreambuf_iterator<char>()));
    ifs.close();

    std::cout << "Loaded shader " << shaderFile << std::endl;

    return loadShader(type, source.c_str());
}

GLboolean ShaderProgram::createProgram()
    {
        GLint linked;

        program = glCreateProgram();
        if (!program)
            return GL_FALSE;

        for (auto shader : shaders)
            glAttachShader(program, shader);
        glLinkProgram(program);
        glGetProgramiv(program, GL_LINK_STATUS, &linked);
        if (!linked)
        {
            GLint infoLen = 0;

            glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLen);
            if (infoLen > 1)
            {
                char *infoLog = static_cast<char *>(malloc(sizeof(char) * infoLen));

                glGetProgramInfoLog(program, infoLen, NULL, infoLog);
                // printf("Error linking program:\n%s\n", infoLog);
                std::cerr << "Error linking program:" << std::endl
                          << infoLog << std::endl;
                free(infoLog);
            }
            glDeleteProgram(program);

            return GL_FALSE;
        }

        return GL_TRUE;
    }