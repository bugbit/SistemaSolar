#include "astro.h"

bool Astro::initGL()
{
    if (filetex == NULL)
        return GL_TRUE;

    std::string file = "assets/textures/";

    file += filetex;

    return texture.load(file.c_str());
}

bool AstroConOrbita::initGL()
{
    if (!Astro::initGL())
        return GL_FALSE;

    a = ejeMayor / 2.0d;
    b = ejeMayor * sqrt(1 - 2 * excentricidad) / 2.0d;
    c = sqrt(a * a - b * b);
    center = glm::vec3(astroCentro->getPosition().x + c, astroCentro->getPosition().y, astroCentro->getPosition().z);
    CalcMVP();

    return GL_TRUE;
}

void AstroConOrbita::CalcMVP()
{
    modelMatrix = glm::translate(center) * glm::translate(glm::vec3(a * sin(angOrbital), 0, b * cos(angOrbital))) * glm::scale(glm::vec3(radius));
}

bool Estrella::initGL()
{
    if (!Astro::initGL())
        return GL_FALSE;

    modelMatrix = glm::translate(position) * glm::scale(glm::vec3(radius));

    return GL_TRUE;
}