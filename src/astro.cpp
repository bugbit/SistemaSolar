#include "astro.h"

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

void AstroConOrbita::CalcMVP()
{
    modelMatrix = glm::translate(center) * glm::translate(glm::vec3(a * sin(angOrbital), 0, b * cos(angOrbital))) * glm::scale(glm::vec3(radius));
}

GLboolean Estrella::initGL()
{
    if (!Astro::initGL())
        return GL_FALSE;

    modelMatrix = glm::translate(position) * glm::scale(glm::vec3(radius));

    std::for_each(planetas.begin(), planetas.end(), [](Planeta *p)
                  { p->initGL(); });

    return GL_TRUE;
}

GLboolean Galaxia::initGL()
{
    if (!CosmoElemento::initGL())
        return GL_FALSE;

    std::for_each(estrellas.begin(), estrellas.end(), [](Estrella *e)
                  { e->initGL(); });

    return GL_TRUE;
}