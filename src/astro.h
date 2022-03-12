#ifndef _ASTRO_H

#define _ASTRO_H

#include "texture.h"

// metros
#define UA 149597870700

class Camera;

enum ASTROS_OPTS_SHADERS
{
    Planet = 0
};

enum COSMOTIPOELEMENTO
{
    GALAXIA,
    ESTRELLA,
    PLANETA,
    SATELITE
};

class CosmoElemento
{
public:
    inline CosmoElemento() {}
    inline CosmoElemento(COSMOTIPOELEMENTO tipo) : tipo(tipo) {}

    inline COSMOTIPOELEMENTO getTipo() const
    {
        return tipo;
    }

    inline void setName(std::string _name)
    {
        name = _name;
    }

    inline const std::string &getName() const
    {
        return name;
    }

protected:
    COSMOTIPOELEMENTO tipo;
    std::string name;
};

class Astro : public CosmoElemento
{
public:
    inline Astro(COSMOTIPOELEMENTO tipo) : CosmoElemento(tipo) {}
    inline Astro(ASTROS_OPTS_SHADERS shader, const char *name, const char *filetex, glm::float32 radius, glm::vec3 position)
        : shader(shader), name2(name), filetex(filetex), radius(radius), position(position)
    {
    }
    inline Astro(ASTROS_OPTS_SHADERS shader, const char *name, const char *filetex, glm::float32 radius)
        : shader(shader), name2(name), filetex(filetex), radius(radius), position()
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
    const char *name2;
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
    inline AstroConOrbita(COSMOTIPOELEMENTO tipo) : Astro(tipo) {}
    AstroConOrbita(ASTROS_OPTS_SHADERS shader, const char *name, const char *filetex, glm::float32 radius, double ejeMayor, double excentricidad, double peridoOrbital)
        : Astro(shader, name, filetex, radius), astroCentro(NULL), ejeMayor(ejeMayor), excentricidad(excentricidad), periodoOrbital(peridoOrbital), center(), angOrbital(0)
    {
        velAngOrbital = 2 * M_PI / (peridoOrbital * 24 * 60 * 60);
    }

    inline const glm::vec3 &getCenter() const
    {
        return center;
    }

    inline float getA() const
    {
        return a;
    }

    inline float getB() const
    {
        return b;
    }

    inline void setAstroCentro(Astro *astro)
    {
        this->astroCentro = astro;
    }

    virtual bool initGL();

    // ellapse in secs
    inline void Orbita(double ellapse)
    {
        angOrbital = fmod(angOrbital + velAngOrbital * ellapse, 2 * M_PI);
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
    double periodoOrbital; // days
    double velAngOrbital;  // rad/s

    void CalcMVP();
};

class Planeta : public AstroConOrbita
{
public:
    inline Planeta() : AstroConOrbita(PLANETA) {}
    Planeta(ASTROS_OPTS_SHADERS shader, const char *name, const char *filetex, glm::float32 radius, double ejeMayor, double excentricidad, double peridoOrbital)
        : AstroConOrbita(shader, name, filetex, radius, ejeMayor, excentricidad, peridoOrbital)
    {
    }

protected:
};

class Estrella : public Astro
{
public:
    inline Estrella() : Astro(ESTRELLA) {}
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

private:
    std::vector<Planeta *> planetas;
};

class Galaxia : public CosmoElemento
{
public:
    inline Galaxia() : CosmoElemento(ESTRELLA) {}

    inline void add(Estrella *estrella)
    {
        estrellas.push_back(estrella);
    }

private:
    std::vector<Estrella *> estrellas;
};

#endif