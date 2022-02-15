#include "astro.h"

bool Astro::initGL()
{
    if (filetex == NULL)
        return GL_TRUE;

    std::string file = "assets/textures/";

    file += filetex;

    return texture.load(file.c_str());
}

bool Estrella::initGL()
{
    if (!Astro::initGL())
        return GL_FALSE;

    modelMatrix = glm::translate(position) * glm::scale(glm::vec3(radius, radius, radius));

    return GL_TRUE;
}