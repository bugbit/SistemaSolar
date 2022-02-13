#include "shaders.h"
#include "texture.h"
#include "vao.h"
#include "camera.h"
#include "astro.h"

// Un arreglo de 3 vectores que representan 3 vértices
// static const NodePoints g_vertex_buffer_data = {
// static const float g_vertex_buffer_data[] = {
static const GLfloat triangle[] =
    {
        -0.9f,
        -0.9f,
        0.0f,
        0.9f,
        0.9f,
        -0.9f,
};

/*
  -0.5, -0.5, 0.0, 1.0 ,  0.0, 0.0,
            -0.5,  0.5, 0.0, 1.0 ,  0.0, 1.0,
            0.5,  0.5, 0.0, 1.0 ,  1.0, 1.0,
            0.5, -0.5, 0.0, 1.0 ,  1.0, 0.0,
*/

static const GLfloat rectangle[] =
    {
        -0.5,
        -0.5,
        -0.5,
        0.5,
        0.5,
        0.5,
        0.5,
        -0.5,
};

static const GLfloat rectangle_uv[] =
    {
        0.0,
        0.0,
        0.0,
        1.0,
        1.0,
        1.0,
        1.0,
        0.0,
};

static GLFWwindow *window;
static Camera camera;
static glm::vec3 obsPos(100, 0, 0), obsCenter(0, 0, 0), obsUp(0, 1, 0);
static PlanetShaderProgram simpleGLSL;
static Texture2d texture;
static GLuint rectangleVAO;
static VAO sphereVAO;
static GLsizei sphereNumIdxs;
static Estrella *sol;

static void resizeGL();
static GLboolean initGL();
static GLboolean init();
static GLboolean initAstros();
static void terminate();
static void terminateGL();
void main_loop();

int main(int, char **)
{
    std::cout << "Hello, world!\n";
#ifndef __EMSCRIPTEN__
    const char *description;
#endif

    /* Initialize the library */
    if (!glfwInit())
    {
#ifndef __EMSCRIPTEN__
        glfwGetError(&description);
        printf("glfwInit error : %s,", description);
#else
        printf("glfwInit error,");
#endif

        return EXIT_FAILURE;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
#ifndef __EMSCRIPTEN__
        glfwGetError(&description);
        printf("glfwInit error : %s,", description);
#else
        printf("glfwInit error,");
#endif

        glfwTerminate();

        return EXIT_FAILURE;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

#ifndef __EMSCRIPTEN__
    GLenum err = glewInit();
    if (GLEW_OK != err)
    {
        /* Problem: glewInit failed, something is seriously wrong. */
        fprintf(stderr, "Error: %s\n", glewGetErrorString(err));

        glfwTerminate();

        return EXIT_FAILURE;
    }
#endif

    resizeGL();

    if (initGL())
    {
        if (init)
        {
#ifdef __EMSCRIPTEN__
            emscripten_set_main_loop(main_loop, 0, true);
#else
            while (!glfwWindowShouldClose(window))
                main_loop();

            terminateGL();
            terminate();
#endif
        }
        else
            terminate();
    }
    else
        terminateGL();

    return EXIT_SUCCESS;
}

static void resizeGL()
{
    int width, height;

    glfwGetWindowSize(window, &width, &height);

    camera.viewport(0, 0, width, height);
    camera.setProjectionRH(30.0f, width / (float)height, 0.1f, 200.0f);
}

static GLboolean initGL()
{
    if (!texture.load("assets/textures/2k_sun.jpg"))
        return GL_FALSE;

    if (!simpleGLSL.createProgramFromFile("assets/shaders/planet.vs", "assets/shaders/planet.fs"))
        return GL_FALSE;

    if (!sphereVAO.MakeSolidSphere(1, 32, 32, sphereNumIdxs))
        return GL_FALSE;

    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    GLuint vbos[2];
    glGenBuffers(2, vbos);

    glBindBuffer(GL_ARRAY_BUFFER, vbos[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(rectangle), rectangle, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, vbos[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(rectangle_uv), rectangle_uv, GL_STATIC_DRAW);
    glVertexAttribPointer(8, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
    glEnableVertexAttribArray(8);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    rectangleVAO = vao;

    return GL_TRUE;
}

static GLboolean init()
{
    if (!initAstros())
        return GL_FALSE;

    return GL_TRUE;
}

static GLboolean initAstros()
{
    //sol=new Estrella();
    return GL_TRUE;
}
static void terminateGL()
{
    simpleGLSL.Delete();
    texture.Delete();
}

static void terminate()
{
    if (sol)
    {
        delete sol;
        
        sol=NULL;
    }
    glfwTerminate();
}

void main_loop()
{
    /* Choose background color */
    // glClearColor(1.0, 0.0, 1.0, 1.0);
    glClearColor(0, 0, 0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    camera.lookUp(obsPos, obsCenter, obsUp);
    simpleGLSL.setMVP(camera.getProjectionMatrix() * camera.getviewMatrix());

    // glBindVertexArray(rectangleVAO);
    glBindVertexArray(sphereVAO.getVAO());
    texture.BindTexture();
    simpleGLSL.Use();
    // glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    glDrawElements(GL_TRIANGLES, sphereNumIdxs, GL_UNSIGNED_INT, BUFFER_OFFSET(0));

    glfwSwapBuffers(window);

    /* Poll for and process events */
    glfwPollEvents();
}