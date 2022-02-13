#ifndef _ASTRO_H

#define _ASTRO_H

class PlanetShaderProgram;
class VAO;
class Camera;

class Astro
{
public:
    inline Astro(ShaderProgram &shader, VAO &vao, glm::float32 radio) : shader(shader), vao(vao), radio(radio)
    {
    }
    inline virtual void DisplayGL(Camera &camera)
    {
    }

protected:
    ShaderProgram &shader;
    VAO &vao;
    glm::float32 radio;
    glm::mat4 modelMatrix;
};

class Estrella : public Astro
{
public:
    Estrella(ShaderProgram &shader, VAO &vao, glm::float32 radio, glm::vec3 position) : Astro(shader, vao, radio), position(position)
    {
    }

protected:
    glm::vec3 position;
};

#endif