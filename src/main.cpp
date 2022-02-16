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

static const GLfloat stars_pos[] =
    {
        -1,
        -1,
        -1,
        1,
        1,
        1,
        1,
        -1,
};

static const GLfloat stars_uv[] =
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
static ShaderProgram starGLSL;
static PlanetShaderProgram planetGLSL;
static Texture2d stars_tex /*, texture*/;
static GLuint starsVAO;
static VAO sphereVAO;
static GLsizei sphereNumIdxs;
static Estrella sol(ASTROS_OPTS_SHADERS::Planet, "sol", "2k_sun.jpg", 109 / 20, glm::vec3(0, 0, 0));
static Planeta tierra(ASTROS_OPTS_SHADERS::Planet, "tierra", "Tierra2k.jpg", 1);

static void resizeGL();
static GLboolean initGL();
static GLboolean init();
static GLboolean initAstros();
static void terminate();
static void terminateGL();
void main_loop();
static void displayGL();
static void displayAstro(Astro &astro);

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
    if (!stars_tex.load("assets/textures/2k_stars+milky_way.jpg"))
        return GL_FALSE;

    if (!starGLSL.createProgramFromFile("assets/shaders/stars.vs", "assets/shaders/stars.fs"))
        return GL_FALSE;

    if (!planetGLSL.createProgramFromFile("assets/shaders/planet.vs", "assets/shaders/planet.fs"))
        return GL_FALSE;

    if (!sphereVAO.MakeSolidSphere(1, 32, 32, sphereNumIdxs))
        return GL_FALSE;

    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    GLuint vbos[2];
    glGenBuffers(2, vbos);

    glBindBuffer(GL_ARRAY_BUFFER, vbos[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(stars_pos), stars_pos, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, vbos[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(stars_uv), stars_uv, GL_STATIC_DRAW);
    glVertexAttribPointer(8, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
    glEnableVertexAttribArray(8);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    starsVAO = vao;

    if (!sol.initGL())
        return GL_FALSE;

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
    // sol=new Estrella();
    return GL_TRUE;
}
static void terminateGL()
{
    planetGLSL.Delete();
    // texture.Delete();
}

static void terminate()
{
    // if (sol)
    // {
    //     delete sol;

    //     sol = NULL;
    // }
    glfwTerminate();
}

void main_loop()
{
    displayGL();

    /*

    // glClearColor(1.0, 0.0, 1.0, 1.0);
    glClearColor(0, 0, 0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    camera.lookUp(obsPos, obsCenter, obsUp);
    planetGLSL.setMVP(camera.getProjectionMatrix() * camera.getviewMatrix());

    // glBindVertexArray(rectangleVAO);
    glBindVertexArray(sphereVAO.getVAO());
    texture.BindTexture();
    planetGLSL.Use();
    // glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    glDrawElements(GL_TRIANGLES, sphereNumIdxs, GL_UNSIGNED_INT, BUFFER_OFFSET(0));

    */

    glBindVertexArray(0);
    glUseProgram(0);
    glBindTexture(GL_TEXTURE_2D, 0);

    glfwSwapBuffers(window);

    /* Poll for and process events */
    glfwPollEvents();
}

static void displayGL()
{
    /* Choose background color */
    // glClearColor(1.0, 0.0, 1.0, 1.0);
    glClearColor(0, 0, 0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    camera.lookUp(obsPos, obsCenter, obsUp);

    glBindVertexArray(starsVAO);
    stars_tex.BindTexture();
    starGLSL.Use();
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

    displayAstro(sol);
}

static void displayAstro(Astro &astro)
{
    glBindVertexArray(sphereVAO.getVAO());
    astro.getTexture().BindTexture();
    planetGLSL.Use();
    planetGLSL.setMVP(camera.getProjectionMatrix() * camera.getviewMatrix() * astro.getModelMatrix());
    glDrawElements(GL_TRIANGLES, sphereNumIdxs, GL_UNSIGNED_INT, BUFFER_OFFSET(0));
}