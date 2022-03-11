#include "shaders.h"
#include "texture.h"
#include "vao.h"
#include "camera.h"
#include "astro.h"
#include "csv.h"

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

static double scaleTime = 1000000.0, lastTime;
static GLFWwindow *window;
static Camera camera;
static glm::vec3 obsPos(25, 2, 0), obsCenter(0, 0, 0), obsUp(0, 1, 0);
static ShaderProgram starGLSL;
static PlanetShaderProgram planetGLSL;
static OrbitShaderProgram orbitGLSL;
static Texture2d stars_tex /*, texture*/;
static GLuint starsVAO;
// static GLuint orbitVAO;
// GLfloat orbitBuffer[32 * 3];
static VAO sphereVAO;
static GLsizei sphereNumIdxs;
static double radi_terra = (12756.78) / 2.0;
static Estrella sol(ASTROS_OPTS_SHADERS::Planet, "sol", "2k_sun.jpg", ((696000) / radi_terra) / 100.0, glm::vec3(0, 0, 0));
static Planeta tierra(ASTROS_OPTS_SHADERS::Planet, "tierra", "Tierra2k.jpg", 0.1, 149503 / 10000.0, 0.016, 365.2);
static Planeta venus(ASTROS_OPTS_SHADERS::Planet, "venus", "2k_venus.jpg", 0.1, 108200 / 10000.0, 0.001, 224.701);

static void resizeGL();
static GLboolean initGL();
static GLboolean init();
static GLboolean initAstros();
static void terminate();
static void terminateGL();
void main_loop();
static void displayGL();
static void displayOrbit(AstroConOrbita &orbit);
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
        if (init())
        {
            lastTime = glfwGetTime();
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

    if (!orbitGLSL.createProgramFromFile("assets/shaders/orbit.vs", "assets/shaders/orbit.fs"))
        return GL_FALSE;

    // if (!orbitGLSL.createProgramFromFile("assets/simple3.vs", "assets/simple3.fs"))
    //     return GL_FALSE;

    if (!sphereVAO.MakeSolidSphere(1, 32, 32, sphereNumIdxs))
        return GL_FALSE;

    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    GLuint vbos[3];
    glGenBuffers(3, vbos);

    glBindBuffer(GL_ARRAY_BUFFER, vbos[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(stars_pos), stars_pos, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, vbos[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(stars_uv), stars_uv, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
    glEnableVertexAttribArray(1);

    starsVAO = vao;

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    sol.add(&tierra);
    sol.add(&venus);

    if (!sol.initGL())
        return GL_FALSE;

    if (!tierra.initGL())
        return GL_FALSE;

    if (!venus.initGL())
        return GL_FALSE;

    // int i = 0;
    // for (GLfloat *b = orbitBuffer; i < 32; i++)
    // {
    //     float angle = (float(i) * 2.0f * M_PI) / float(32);
    //     glm::vec3 pos(tierra.getCenter() + glm::vec3(tierra.getA() * sin(angle), 0, tierra.getB() * cos(angle)));

    //     *b++ = pos.x;
    //     *b++ = pos.y;
    //     *b++ = pos.z;
    // }

    // glGenVertexArrays(1, &vao);
    // glBindVertexArray(vao);

    // glBindBuffer(GL_ARRAY_BUFFER, vbos[2]);
    // glBufferData(GL_ARRAY_BUFFER, sizeof(orbitBuffer), orbitBuffer, GL_STATIC_DRAW);
    // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
    // glEnableVertexAttribArray(0);

    // orbitVAO = vao;

    // glBindVertexArray(0);
    // glBindBuffer(GL_ARRAY_BUFFER, 0);
    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

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
    Csv csv;

    csv.load("assets/ssolar.csv");
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
    double _lastTime = glfwGetTime();
    double ellapse = _lastTime - lastTime;

    lastTime = _lastTime;
    tierra.Orbita(ellapse * scaleTime);
    venus.Orbita(ellapse * scaleTime);

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

    stars_tex.BindTexture();
    glBindVertexArray(starsVAO);
    starGLSL.Use();
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

    displayOrbit(tierra);
    displayOrbit(venus);
    displayAstro(sol);
    displayAstro(tierra);
    displayAstro(venus);
}

static void displayOrbit(AstroConOrbita &orbit)
{
    // glBindVertexArray(orbitVAO);
    orbitGLSL.Use();
    orbitGLSL.setMVP(camera.getProjectionMatrix() * camera.getviewMatrix());
    orbitGLSL.setCenter((glm::vec3 &)orbit.getCenter());
    orbitGLSL.setA(orbit.getA());
    orbitGLSL.setB(orbit.getB());
    orbitGLSL.setNumVert(32);
    glDrawArrays(GL_LINE_LOOP, 0, 32);
}

static void displayAstro(Astro &astro)
{
    astro.getTexture().BindTexture();
    glBindVertexArray(sphereVAO.getVAO());
    planetGLSL.Use();
    planetGLSL.setMVP(camera.getProjectionMatrix() * camera.getviewMatrix() * astro.getModelMatrix());
    glDrawElements(GL_TRIANGLES, sphereNumIdxs, GL_UNSIGNED_INT, BUFFER_OFFSET(0));
}