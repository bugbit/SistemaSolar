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

class PlanetShaderProgram : public ShaderProgram
{
public:
    PlanetShaderProgram() : ShaderProgram(), MVPUniform(-1)
    {
    }

    inline void setMVP(glm::mat4 mvp)
    {
        glUniformMatrix4fv(MVPUniform, 1, GL_FALSE, glm::value_ptr(mvp));
    }

protected:
    // Model, View, Projection matrix uniform variable in shader program.
    GLint MVPUniform;

    inline virtual void initData()
    {
        MVPUniform = glGetUniformLocation(program, "MVP");
    }
};

class OrbitShaderProgram : public ShaderProgram
{
public:
    OrbitShaderProgram() : ShaderProgram(), centerUniform(-1), aUniform(-1), bUniform(-1), numVertUniform(-1)
    {
    }

    inline void setMVP(glm::mat4 mvp)
    {
        glUniformMatrix4fv(MVPUniform, 1, GL_FALSE, glm::value_ptr(mvp));
    }

    inline void setCenter(glm::vec3 &center)
    {
        glUniform3fv(centerUniform, 1, glm::value_ptr(center));
    }

    inline void setA(GLfloat a)
    {
        glUniform1f(aUniform, a);
    }

    inline void setB(GLfloat b)
    {
        glUniform1f(bUniform, b);
    }

    inline void setNumVert(GLint n)
    {
        glUniform1i(numVertUniform, n);
    }

protected:
    // Model, View, Projection matrix uniform variable in shader program.
    GLint MVPUniform;
    GLint centerUniform;
    GLint aUniform, bUniform;
    GLint numVertUniform;

    inline virtual void initData()
    {
        MVPUniform = glGetUniformLocation(program, "MVP");
        centerUniform = glGetUniformLocation(program, "vCenter");
        aUniform = glGetUniformLocation(program, "fA");
        bUniform = glGetUniformLocation(program, "fB");
        numVertUniform = glGetUniformLocation(program, "iNumVert");
    }
};

#endif