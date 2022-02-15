#ifndef _ASTRO_H

#define _ASTRO_H

#include "texture.h"

class Camera;

enum ASTROS_OPTS_SHADERS
{
    Planet = 0
};

class Astro
{
public:
    inline Astro(ASTROS_OPTS_SHADERS shader, const char *name, const char *filetex, glm::float32 radius, glm::vec3 position)
        : shader(shader), name(name), filetex(filetex), radius(radius), position(position)
    {
    }
    inline virtual void displayGL(Camera &camera)
    {
    }

    virtual bool initGL();

    inline const glm::mat4 &getModelMatrix() const
    {
        return modelMatrix;
    }

    inline const Texture2d &getTexture() const
    {
        return texture;
    }

protected:
    ASTROS_OPTS_SHADERS shader;
    const char *name;
    const char *filetex;
    glm::float32 radius;
    glm::vec3 position;
    glm::mat4 modelMatrix;
    Texture2d texture;
};

class Estrella : public Astro
{
public:
    Estrella(ASTROS_OPTS_SHADERS shader, const char *name, const char *filetex, glm::float32 radius, glm::vec3 position)
        : Astro(shader, name, filetex, radius, position)
    {
        Astro::position = position;
    }

    virtual bool initGL();

protected:
};

#endif