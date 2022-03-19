#ifndef _ASTRO_H

#define _ASTRO_H

#include "texture.h"

// metros
#define UA 149597870700

class Camera;

enum ASTROS_OPTS_SHADERS
{
    AOS_NONE,
    PLANET
};

enum COSMOTIPOELEMENTO
{
    ASTRO = 0x8,
    ASTROCONORBITA = ASTRO | 0x100,
    NONE = 0,
    GALAXIA = 1,
    ESTRELLA = 2 | ASTRO,
    PLANETA = 3 | ASTROCONORBITA,
    SATELITE = 4 | ASTROCONORBITA
};

typedef struct
{
    // eName - string - the name of the object
    std::string eName;
    // isPlanet - boolean - is the object a planet (this includes the five dwarf planets)
    std::string isPlanet;
    // semimajorAxis - integer - mean orbital distance in km
    long long int semimajorAxis;
    // perihelion - integer - closest distance from the Sun during its orbit
    long long int perihelion;
    // aphelion - integer - furthest distance from the Sun during its orbit
    long long int aphelion;
    // eccentricity - double - ratio of perihelion to aphelion
    double eccentricity;
    // inclination - double - difference in angle between body's orbit and the ecliptic
    double inclination;
    // density - double - average density of the body
    double density;
    // gravity - double - surface gravity, measures in m/s^2
    double gravity;
    // escape - integer - escape velocity at surface level
    double escape;
    // meanRadius - double - average total radius
    double meanRadius;
    // equaRadius - double - average equatorial radius
    double equaRadius;
    // polarRadius - double - average polar radius
    double polarRadius;
    // flattening - double - ratio of equatorial radius to polar radius
    double flattening;
    // dimension - string - approximate spatial dimensions of irregular shaped objects
    std::string dimension;
    // sideralOrbit - double - orbital period in Earth days
    double sideralOrbit;
    // sideralRotation - double - rotational period in hours
    double sideralRotation;
    // discoveryDate - date - date of discovery, this is left blank for any objects that were known since antiquity
    std::string discoveryDate;
    // mass_kg - integer - total estimated mass of object in kg
    double mass_kg;
    // volume - integer - approximate volume in km^3
    std::string volume;
    // orbit_type - class - either primary; orbites the Sun, or secondary; orbits a planet
    std::string orbit_type;
    // orbits - class - the planet that the body orbits. If it does not orbit a planet then it is NA
    std::string orbits;
    // bondAlbido - double - Bond albedo, portion of light/energy that get reflected by the surface
    std::string bondAlbido;
    // geomAlbido - double - Geometric albedo, modified reflective metric for spherical objects which, because of opposition effects, can be greater than 1
    std::string geomAlbido;
    // RV_abs - double - radial velocity of Sun due to object's gravitational pull
    std::string RV_abs;
    // p_transit - double - probability that a transit will be observable
    std::string p_transit;
    // transit_visibility - double - angle from the ecliptic that a transit will be visible
    std::string transit_visibility;
    // transit_depth - double - portion of Sun's energy that is blocked during transit
    double transit_depth;
    // massj - integer - mass compared to Jupiter
    double massj;
    // semimajorAxis_AU - integer - orbital radius in Astronomical Units
    double semimajorAxis_AU;
    // grav_int - gravitational interaction with the Sun
    std::string grav_int;
} SolDataItem;

typedef struct
{
    // eName - string - the name of the object
    std::string eName;
    // texture - string
    std::string texture;
    // tshader - integer (0 = none, 1 = planet)
    int tshader;
} SSolarDataItem;

typedef struct
{
    double numerator, denominator;

    inline double calc(double valor) const
    {
        return valor * numerator / denominator;
    }
} CosmoEscalaItem;

typedef struct
{
    CosmoEscalaItem radio_planeta;
    CosmoEscalaItem distancia;
    CosmoEscalaItem radio_estrella;
} CosmoEscala;

class CosmoElemento
{
public:
    inline CosmoElemento() {}
    inline CosmoElemento(COSMOTIPOELEMENTO tipo, const SolDataItem *data = NULL) : tipo(tipo), data(data) {}
    inline virtual ~CosmoElemento()
    {
        if (data)
            delete data;
    }

    inline COSMOTIPOELEMENTO getTipo() const
    {
        return tipo;
    }

    inline const SolDataItem &getData() const
    {
        return *data;
    }

    inline void setData(const SolDataItem *_data)
    {
        data = _data;
    }

    inline virtual void calcDatas(const CosmoEscala *escalas = NULL) {}

    inline virtual GLboolean initGL() { return GL_TRUE; }

    inline virtual void Orbita(double ellapse) {}

protected:
    COSMOTIPOELEMENTO tipo;
    const SolDataItem *data;
};

class Astro : public CosmoElemento
{
public:
    inline Astro(COSMOTIPOELEMENTO tipo, const SolDataItem *data = NULL)
        : CosmoElemento(tipo, data), shader(ASTROS_OPTS_SHADERS::AOS_NONE), filetex(), radius(0), position()
    {
    }
    inline Astro(COSMOTIPOELEMENTO tipo, ASTROS_OPTS_SHADERS shader, const char *name, const char *filetex, glm::float32 radius, glm::vec3 position)
        : CosmoElemento(tipo), shader(shader), name2(name), filetex(filetex), radius(radius), position(position)
    {
    }
    inline Astro(COSMOTIPOELEMENTO tipo, ASTROS_OPTS_SHADERS shader, const char *name, const char *filetex, glm::float32 radius)
        : CosmoElemento(tipo), shader(shader), name2(name), filetex(filetex), radius(radius), position()
    {
    }

    inline ASTROS_OPTS_SHADERS getShader() const
    {
        return shader;
    }

    virtual void calcDatas(const CosmoEscala *escalas = NULL);
    virtual GLboolean initGL();

    inline void setFileTex(std::string _filetex)
    {
        filetex = _filetex;
    }

    inline void setShader(ASTROS_OPTS_SHADERS _shader)
    {
        shader = _shader;
    }

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

    virtual double getDataRadius(const SolDataItem &data) const
    {
        return data.equaRadius;
    }
    inline virtual const CosmoEscalaItem &getEscalaItemRadius(const CosmoEscala &escalas) const
    {
        return escalas.radio_planeta;
    }
};

class Estrella;

class AstroConOrbita : public Astro
{
public:
    inline AstroConOrbita(COSMOTIPOELEMENTO tipo, const SolDataItem *data = NULL)
        : Astro(tipo, data), astroCentro(NULL), ejeMayor(0), excentricidad(0), periodoOrbital(0), periodoRotacion(0), velAngOrbital(0), velAngRotacion(0), angOrbital(0), angRotacion(0) {}
    AstroConOrbita(COSMOTIPOELEMENTO tipo, ASTROS_OPTS_SHADERS shader, const char *name, const char *filetex, glm::float32 radius, double ejeMayor, double excentricidad, double periodoOrbital)
        : Astro(tipo, shader, name, filetex, radius), astroCentro(NULL), ejeMayor(ejeMayor), excentricidad(excentricidad), periodoOrbital(periodoOrbital), periodoRotacion(0), velAngOrbital(0), velAngRotacion(0), angOrbital(0), angRotacion(0)
    {
    }

    inline const glm::vec3 &getCenter() const
    {
        return (astroCentro) ? astroCentro->getPosition() : centerZero;
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

    // virtual GLboolean initGL();
    virtual void calcDatas(const CosmoEscala *escalas = NULL);

    // ellapse in secs
    inline virtual void Orbita(double ellapse)
    {
        angOrbital = fmod(angOrbital + velAngOrbital * ellapse, 2 * M_PI);
        angRotacion = fmod(angRotacion + velAngRotacion * ellapse, 2 * M_PI);
        CalcMVP();
    }

protected:
    static glm::vec3 centerZero;
    Astro *astroCentro;
    double ejeMayor;
    double excentricidad;
    double a; // ejeMayor/2
    double b; // ejeMenor/2
    double c; // distancia focaL
    float angOrbital;
    float angRotacion;
    double periodoOrbital;  // days
    double velAngOrbital;   // rad/s
    double periodoRotacion; // horas
    double velAngRotacion;  // rad/s

    void CalcMVP();
};

class Satelite : public AstroConOrbita
{
public:
    inline Satelite(const SolDataItem *data = NULL) : AstroConOrbita(SATELITE, data) {}
    Satelite(ASTROS_OPTS_SHADERS shader, const char *name, const char *filetex, glm::float32 radius, double ejeMayor, double excentricidad, double peridoOrbital)
        : AstroConOrbita(SATELITE, shader, name, filetex, radius, ejeMayor, excentricidad, peridoOrbital)
    {
    }
};

class Planeta : public AstroConOrbita
{
public:
    inline Planeta(const SolDataItem *data = NULL) : AstroConOrbita(PLANETA, data), satelites() {}
    Planeta(ASTROS_OPTS_SHADERS shader, const char *name, const char *filetex, glm::float32 radius, double ejeMayor, double excentricidad, double peridoOrbital)
        : AstroConOrbita(PLANETA, shader, name, filetex, radius, ejeMayor, excentricidad, peridoOrbital), satelites()
    {
    }

    inline const std::vector<Satelite *> &getSatelites() const
    {
        return satelites;
    }

    inline void add(Satelite *satelite)
    {
        satelite->setAstroCentro(this);
        satelites.push_back(satelite);
    }

    GLboolean initGL();
    virtual void calcDatas(const CosmoEscala *escalas = NULL);
    inline virtual void Orbita(double ellapse)
    {
        AstroConOrbita::Orbita(ellapse);
        for (auto satelite = satelites.begin(); satelite != satelites.end(); satelite++)
        {
            (*satelite)->Orbita(ellapse);
        }
    }

protected:
    std::vector<Satelite *> satelites;
};

class Estrella : public Astro
{
public:
    inline Estrella(const SolDataItem *data = NULL) : Astro(ESTRELLA, data) {}
    Estrella(ASTROS_OPTS_SHADERS shader, const char *name, const char *filetex, glm::float32 radius, glm::vec3 position)
        : Astro(ESTRELLA, shader, name, filetex, radius, position), planetas()
    {
    }

    inline const std::vector<Planeta *> &getPlanetas() const
    {
        return planetas;
    }

    inline void add(Planeta *planeta)
    {
        planeta->setAstroCentro(this);
        planetas.push_back(planeta);
    }

    virtual GLboolean initGL();
    virtual void calcDatas(const CosmoEscala *escalas = NULL);
    inline virtual void Orbita(double ellapse)
    {
        for (auto planeta = planetas.begin(); planeta != planetas.end(); planeta++)
        {
            (*planeta)->Orbita(ellapse);
        }
    }

protected:
    virtual double getDataRadius(const SolDataItem &data) const
    {
        return (data.equaRadius != 0) ? data.equaRadius : (data.meanRadius > 0) ? data.meanRadius
                                                                                : data.polarRadius;
    }
    inline virtual const CosmoEscalaItem &getEscalaItemRadius(const CosmoEscala &escalas) const
    {
        return escalas.radio_estrella;
    }

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

    virtual GLboolean initGL();
    virtual void calcDatas(const CosmoEscala *escalas = NULL);
    inline virtual void Orbita(double ellapse)
    {
        for (auto estrella = estrellas.begin(); estrella != estrellas.end(); estrella++)
        {
            (*estrella)->Orbita(ellapse);
        }
    }

private:
    std::vector<Estrella *> estrellas;
};

class SSolar : public Galaxia
{
public:
    inline SSolar() : Galaxia(), sol(NULL), mapPlanetaNames(), mapSateliteNames() {}
    inline virtual ~SSolar() {}

    inline Estrella *getSol() const
    {
        return sol;
    }

    inline void addSol(Estrella *_sol)
    {
        sol = _sol;
        Galaxia::add(_sol);
    }
    inline void add(Planeta *planeta)
    {
        mapPlanetaNames[planeta->getData().eName] = planeta;
        sol->add(planeta);
    }
    inline void add(Satelite *satelite)
    {
        mapSateliteNames[satelite->getData().eName] = satelite;
    }

    inline Planeta *getPlaneta(const std::string &name)
    {
        return mapPlanetaNames[name];
    }

    inline Satelite *getSatelite(const std::string &name)
    {
        return mapSateliteNames[name];
    }

    inline Astro *getAstro(const std::string &name)
    {
        if (sol->getData().eName == name)
            return sol;

        Astro *astro = getPlaneta(name);

        if (astro != NULL)
            return astro;

        return getSatelite(name);
    }

    void fixSatelites();

private:
    Estrella *sol;
    std::map<std::string, Planeta *> mapPlanetaNames;
    std::map<std::string, Satelite *> mapSateliteNames;
};

#endif