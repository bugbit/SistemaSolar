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

    CalcMVP();

    return GL_TRUE;
}

void AstroConOrbita::CalcMVP()
{
    modelMatrix = glm::rotate(angOrbital, glm::vec3(0, 1, 0)) * glm::translate(glm::vec3(0.05 * (aphelion + perihelion), 0, 0.05 * (aphelion + perihelion))) * glm::scale(glm::vec3(radius));
}

bool Estrella::initGL()
{
    if (!Astro::initGL())
        return GL_FALSE;

    modelMatrix = glm::translate(position) * glm::scale(glm::vec3(radius));

    return GL_TRUE;
}