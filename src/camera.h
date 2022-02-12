#ifndef _CAMERA_H

#define _CAMERA_H

class Camera
{
public:
    inline Camera() : viewMatrix(1), projectionMatrix(1)
    {
    }
    inline void Viewport(int x, int y, int width, int height)
    {
        glViewport(x, y, width, height);
    }
    inline void SetProjectionRH(float fov, float aspectRatio, float zNear, float zFar)
    {
        projectionMatrix = glm::perspective(glm::radians(fov), aspectRatio, zNear, zFar);
    }
    inline glm::mat4 GetProjectionMatrix()
    {
        return projectionMatrix;
    }

private:
    glm::mat4 viewMatrix;
    glm::mat4 projectionMatrix;
};

#endif