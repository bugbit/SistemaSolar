#ifndef _ASTRO_H

#define _ASTRO_H

class PlanetShaderProgram;
class VAO;
class Camera;

class Astro
{
public:
    inline Astro(glm::float32 radio) : radio(radio)
    {
    }
    inline virtual void DisplayGL(Camera &camera)
    {
    }

protected:
    glm::float32 radio;
    glm::vec3 position;
    glm::mat4 modelMatrix;
};

class Estrella : public Astro
{
public:
    Estrella(glm::float32 radio, glm::vec3 position) : Astro(radio)
    {
        Astro::position = position;
    }

protected:
};

#endif