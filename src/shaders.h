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

    GLuint loadShader(GLenum type, const char *shaderSrc);
    GLuint loadShaderFromFile(GLenum type, const char *shaderFile);
    GLboolean createProgram();

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

#endif