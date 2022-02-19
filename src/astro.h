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
    inline Astro(ASTROS_OPTS_SHADERS shader, const char *name, const char *filetex, glm::float32 radius)
        : shader(shader), name(name), filetex(filetex), radius(radius), position()
    {
    }

    virtual bool initGL();

    inline const glm::vec3 &getPosition() const
    {
        return position;
    }

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

class Estrella;

class AstroConOrbita : public Astro
{
public:
    AstroConOrbita(ASTROS_OPTS_SHADERS shader, const char *name, const char *filetex, glm::float32 radius, double ejeMayor, double excentricidad)
        : Astro(shader, name, filetex, radius), astroCentro(NULL), ejeMayor(ejeMayor), excentricidad(excentricidad), center(), angOrbital(0)
    {
    }

    inline void setAstroCentro(Astro *astro)
    {
        this->astroCentro = astro;
    }

    virtual bool initGL();

    inline void Orbita()
    {
        angOrbital = fmod(angOrbital + M_PI / 180, 2 * M_PI);
        CalcMVP();
    }

protected:
    Astro *astroCentro;
    double ejeMayor;
    double excentricidad;
    double a; // ejeMayor/2
    double b; // ejeMenor/2
    double c; // distancia focaL
    glm::vec3 center;
    float angOrbital;

    void CalcMVP();
};

class Planeta : public AstroConOrbita
{
public:
    Planeta(ASTROS_OPTS_SHADERS shader, const char *name, const char *filetex, glm::float32 radius, double aphelion, double perihelion)
        : AstroConOrbita(shader, name, filetex, radius, aphelion, perihelion)
    {
    }

protected:
};

class Estrella : public Astro
{
public:
    Estrella(ASTROS_OPTS_SHADERS shader, const char *name, const char *filetex, glm::float32 radius, glm::vec3 position)
        : Astro(shader, name, filetex, radius, position), planetas()
    {
    }

    inline void add(Planeta *planeta)
    {
        planeta->setAstroCentro(this);
        planetas.push_back(planeta);
    }

    virtual bool initGL();

protected:
    std::vector<Planeta *> planetas;
};

#endif