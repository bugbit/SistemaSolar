#ifndef _SHADERS_H

#define _SHADERS_H

class ShaderProgram
{
public:
    inline ShaderProgram() : shaders(), program(0) {}

    inline ~ShaderProgram()
    {
        Delete();
    }

    inline void Delete()
    {
        for (auto shader : shaders)
            glDeleteShader(shader);
        shaders.clear();
        if (program)
        {
            glDeleteProgram(program);
            program = 0;
        }
    }

    inline GLuint loadShader(GLenum type, const char *shaderSrc)
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

    inline GLuint loadShaderFromFile(GLenum type, const char *shaderFile)
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

        return loadShader(type, source.c_str());
    }

    inline GLboolean createProgram()
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

    inline GLboolean createProgram(const char *shaderVSSrc, const char *shaderFSSrc)
    {
        if (!loadShader(GL_VERTEX_SHADER, shaderVSSrc) || !loadShader(GL_FRAGMENT_SHADER, shaderFSSrc))
            return GL_FALSE;

        return createProgram();
    }

    inline GLboolean createProgramFromFile(const char *shaderVSFile, const char *shaderFSFile)
    {
        if (!loadShaderFromFile(GL_VERTEX_SHADER, shaderVSFile) || !loadShaderFromFile(GL_FRAGMENT_SHADER, shaderFSFile))
            return GL_FALSE;

        if (!createProgram())
            return GL_FALSE;

        initData();

        return GL_TRUE;
    }

    inline void Use() const
    {
        glUseProgram(program);
    }

protected:
    GLuint program;
    std::vector<GLuint> shaders;

    inline virtual void initData()
    {
    }
};

class SimpleShaderProgram : public ShaderProgram
{
public:
    inline SimpleShaderProgram() : ShaderProgram(), vPositionAttribLocation(0) {}

    inline GLuint GetIndexVPosition() const
    {
        return vPositionAttribLocation;
    }

protected:
    GLuint vPositionAttribLocation;

    inline virtual void initData()
    {
        ShaderProgram::initData();
        vPositionAttribLocation = glGetAttribLocation(program, "vPosition");
    }
};

#endif