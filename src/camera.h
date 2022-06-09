#ifndef _CAMERA_H

#define _CAMERA_H

class Camera
{
public:
    inline Camera() : viewMatrix(1), projectionMatrix(1), viewMatrixMilkyway(1), position(), front()
    {
    }
    inline glm::mat4 getProjectionMatrix() const
    {
        return projectionMatrix;
    }
    inline glm::mat4 getviewMatrix() const
    {
        return viewMatrix;
    }
    inline glm::mat4 getviewMatrixMilkyway() const
    {
        return viewMatrixMilkyway;
    }
    inline void viewport(int x, int y, int width, int height)
    {
        glViewport(x, y, width, height);
    }
    inline void setProjectionRH(float fov, float aspectRatio, float zNear, float zFar)
    {
        projectionMatrix = glm::perspective(glm::radians(fov), aspectRatio, zNear, zFar);
    }
    inline void lookUp(glm::vec3 eye, glm::vec3 center, glm::vec3 up)
    {
        position = eye;
        front = center - eye;
        viewMatrix = glm::lookAt(eye, center, up);
    }

private:
    glm::mat4 projectionMatrix;
    glm::mat4 viewMatrix;
    glm::mat4 viewMatrixMilkyway;

    glm::vec3 position;
    glm::vec3 front;

    // falta angulos
    /*
        The azimuth describes the angle of rotation of the camera about the object in the x-y plane, measured from the positive y-axis.
        Similarly, elevation is the angle of rotation of the camera in the y-z plane, measured from the positive z-axis.
        Finally, twist represents the rotation of the viewing volume around its line of sight.
    */
};

#endif