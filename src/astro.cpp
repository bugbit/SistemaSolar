#include "astro.h"

glm::vec3 AstroConOrbita::centerZero;

void Astro::calcDatas(const CosmoEscala *escalas)
{
    CosmoElemento::calcDatas(escalas);
    if (data)
    {
        if (escalas == NULL)
            radius = getDataRadius(*data);
        else
            radius = getEscalaItemRadius(*escalas).calc(getDataRadius(*data));
    }
}

GLboolean Astro::initGL()
{
    if (!CosmoElemento::initGL())
        return GL_FALSE;

    if (filetex.empty())
        return GL_TRUE;

    std::string file = "assets/textures/";

    file += filetex;

    return texture.load(file.c_str());
}

/*
GLboolean AstroConOrbita::initGL()
{
    if (!Astro::initGL())
        return GL_FALSE;

    a = ejeMayor / 2.0;
    b = ejeMayor * sqrt(1 - 2 * excentricidad) / 2.0;
    c = sqrt(a * a - b * b);
    center = glm::vec3(astroCentro->getPosition().x + c, astroCentro->getPosition().y, astroCentro->getPosition().z);
    CalcMVP();

    return GL_TRUE;
}
*/

void AstroConOrbita::calcDatas(const CosmoEscala *escalas)
{
    Astro::calcDatas(escalas);
    if (data)
    {
        ejeMayor = (escalas) ? escalas->distancia.calc(data->semimajorAxis) : data->semimajorAxis;
        excentricidad = data->eccentricity;
        periodoOrbital = data->sideralOrbit;
        periodoRotacion = data->sideralRotation;
    }
    a = ejeMayor / 2.0;
    b = ejeMayor * sqrt(1 - 2 * excentricidad) / 2.0;
    c = sqrt(a * a - b * b);
    if (periodoOrbital != 0)
        velAngOrbital = 2 * M_PI / (periodoOrbital * 24 * 60 * 60);
    if (periodoRotacion != 0)
        velAngRotacion = 2 * M_PI / (periodoRotacion * 60 * 60);
    CalcMVP();
}

void AstroConOrbita::CalcMVP()
{
    position = getCenter() + glm::vec3(a * sin(angOrbital), 0, b * cos(angOrbital));

    modelMatrix = glm::translate(position) * glm::scale(glm::vec3(radius)) * glm::rotate(angRotacion, glm::vec3(0, 1, 0));
}

GLboolean Planeta::initGL()
{
    if (!AstroConOrbita::initGL())
        return GL_FALSE;

    for (auto satelite = satelites.begin(); satelite != satelites.end(); satelite++)
    {
        if (!(*satelite)->initGL())
            return GL_FALSE;
    }

    return GL_TRUE;
}

void Planeta::calcDatas(const CosmoEscala *escalas)
{
    AstroConOrbita::calcDatas(escalas);
    for (auto satelite = satelites.begin(); satelite != satelites.end(); satelite++)
    {
        (*satelite)->calcDatas(escalas);
    }
}

GLboolean Estrella::initGL()
{
    if (!Astro::initGL())
        return GL_FALSE;

    // modelMatrix = glm::translate(position) * glm::scale(glm::vec3(radius));

    // std::for_each(planetas.begin(), planetas.end(), [](Planeta *p)
    //               { p->initGL(); });
    for (auto planeta = planetas.begin(); planeta != planetas.end(); planeta++)
    {
        if (!(*planeta)->initGL())
            return GL_FALSE;
    }

    return GL_TRUE;
}

void Estrella::calcDatas(const CosmoEscala *escalas)
{
    Astro::calcDatas(escalas);
    for (auto planeta = planetas.begin(); planeta != planetas.end(); planeta++)
    {
        (*planeta)->calcDatas(escalas);
    }
    modelMatrix = glm::translate(position) * glm::scale(glm::vec3(radius));
}

GLboolean Galaxia::initGL()
{
    if (!CosmoElemento::initGL())
        return GL_FALSE;

    // std::for_each(estrellas.begin(), estrellas.end(), [](Estrella *e)
    //               { e->initGL(); });
    for (auto estrella = estrellas.begin(); estrella != estrellas.end(); estrella++)
    {
        if (!(*estrella)->initGL())
            return GL_FALSE;
    }

    return GL_TRUE;
}

void Galaxia::calcDatas(const CosmoEscala *escalas)
{
    CosmoElemento::calcDatas(escalas);

    for (auto estrella = estrellas.begin(); estrella != estrellas.end(); estrella++)
    {
        (*estrella)->calcDatas(escalas);
    }
}

void SSolar::fixSatelites()
{
    for (auto msatelite = mapSateliteNames.begin(); msatelite != mapSateliteNames.end(); msatelite++)
    {
        Satelite *satelite = msatelite->second;
        Planeta *planeta = getPlaneta(satelite->getData().orbits);

        if (planeta != NULL)
            planeta->add(satelite);
    }
}