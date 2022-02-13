#ifndef _CAMERA_H

#define _CAMERA_H

class Camera
{
public:
    inline Camera() : viewMatrix(1), projectionMatrix(1)
    {
    }
    inline void viewport(int x, int y, int width, int height)
    {
        glViewport(x, y, width, height);
    }
    inline void setProjectionRH(float fov, float aspectRatio, float zNear, float zFar)
    {
        projectionMatrix = glm::perspective(glm::radians(fov), aspectRatio, zNear, zFar);
    }
    inline glm::mat4 getProjectionMatrix()
    {
        return projectionMatrix;
    }
    inline void lookUp(glm::vec3 eye, glm::vec3 center, glm::vec3 up)
    {
        viewMatrix = glm::lookAt(eye, center, up);
    }
    inline glm::mat4 getviewMatrix()
    {
        return viewMatrix;
    }

private:
    glm::mat4 viewMatrix;
    glm::mat4 projectionMatrix;
};

#endif