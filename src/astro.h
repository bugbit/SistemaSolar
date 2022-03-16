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

typedef struct
{
//eName - string - the name of the object
std::string eName;
//isPlanet - boolean - is the object a planet (this includes the five dwarf planets)
std::string isPlanet;
//semimajorAxis - integer - mean orbital distance in km
long long int semimajorAxis;
//perihelion - integer - closest distance from the Sun during its orbit
long long int perihelion;
//aphelion - integer - furthest distance from the Sun during its orbit
long long int aphelion;
//eccentricity - double - ratio of perihelion to aphelion
double eccentricity;
//inclination - double - difference in angle between body's orbit and the ecliptic
double inclination;
//density - double - average density of the body
double density;
//gravity - double - surface gravity, measures in m/s^2
double gravity;
//escape - integer - escape velocity at surface level
long long escape;
//meanRadius - double - average total radius
double meanRadius;
//equaRadius - double - average equatorial radius
double equaRadius;
//polarRadius - double - average polar radius
double polarRadius;
//flattening - double - ratio of equatorial radius to polar radius
double flattening;
//dimension - string - approximate spatial dimensions of irregular shaped objects
std::string dimension;
//sideralOrbit - double - orbital period in Earth days
double sideralOrbit;
//sideralRotation - double - rotational period in hours
double sideralRotation;
//discoveryDate - date - date of discovery, this is left blank for any objects that were known since antiquity
std::string discoveryDate;
//mass_kg - integer - total estimated mass of object in kg
double mass_kg;
//volume - integer - approximate volume in km^3
long long volume;
//orbit_type - class - either primary; orbites the Sun, or secondary; orbits a planet
std::string orbit_type;
//orbits - class - the planet that the body orbits. If it does not orbit a planet then it is NA
std::string orbits;
//bondAlbido - double - Bond albedo, portion of light/energy that get reflected by the surface
std::string bondAlbido;
//geomAlbido - double - Geometric albedo, modified reflective metric for spherical objects which, because of opposition effects, can be greater than 1
std::string geomAlbido;
//RV_abs - double - radial velocity of Sun due to object's gravitational pull
std::string RV_abs;
//p_transit - double - probability that a transit will be observable
double p_transit;
//transit_visibility - double - angle from the ecliptic that a transit will be visible
double transit_visibility;
//transit_depth - double - portion of Sun's energy that is blocked during transit
double transit_depth;
//massj - integer - mass compared to Jupiter
double massj;
//semimajorAxis_AU - integer - orbital radius in Astronomical Units
double semimajorAxis_AU;
//grav_int - gravitational interaction with the Sun
double grav_int;
} SolDataItem;

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
    std::string filetex;
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

class SSolar : public Galaxia
{
public:
    inline SSolar() : Galaxia() {}
};

#endif