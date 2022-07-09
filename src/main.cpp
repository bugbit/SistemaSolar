#include "error.h"
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

static int wwidth = 800, wheight = 600;
static int wwwidth, wwheight, wwrefreshrate;
static double scaleTime = 1000000.0, lastTime;
static GLFWwindow *window;
static Camera *camera;
// static const GLfloat fov=30.0f;
static GLfloat fov = 30.0f;
// static glm::vec3 obsPos(10, 5, 10), obsCenter(0, 0, 0), obsUp(0, 1, 0);
// antes
// static glm::vec3 obsPos(100, 25, 25), obsCenter(-100, -25, -35), obsUp(0, 1, 0);
// static glm::vec3 obsPos(700, 25, 25), obsCenter(-700, -25, -35), obsUp(0, 1, 0);
glm::vec3 obsPos(0, 250, 250), obsCenter(0, 0, 0), obsUp(0, 1, 0), obsFront = glm::vec3(0.0f, 0.0f, -1.0f);
GLfloat obsSpeed = 10;
// static ShaderProgram starGLSL;
static ShyboxShaderProgram skyboxGLSL;
static PlanetShaderProgram planetGLSL;
static OrbitShaderProgram orbitGLSL;
// static Texture2d stars_tex;  /*, texture*/
static Texture2d skybox_tex; /*, texture*/
// static GLuint starsVAO;
//  static GLuint orbitVAO;
//  GLfloat orbitBuffer[32 * 3];
static VAO skyboxVAO;
static VAO sphereVAO;
static GLsizei sphereNumIdxs;
/*
Diameters is: 10’000 km (reality) = 1m
positions is : 1 million km (reality) = 1m (in Unity)*
*/
static double escala1 = 30.0 / (11.2 * 12756.78e3 / 2.0);
static double escala2 = 350.0 / (590010000e3);
static double escala3 = 90.0 / (14950300e3);
const CosmoEscala escalaVistaPlanetaria =
    {
        //{30.0, 11.2 * 12756.78 / 2.0},
        {1, 10000},
        //{350.0, 590010000},
        {1, 1000000},
        //{90.0, 14950300},
        {1, 100000},
};
static double radi_terra = (12756.78) / 2.0;
SSolar *ssolar;
/*
static Estrella sol(ASTROS_OPTS_SHADERS::PLANET, "sol", "2k_sun.jpg", 696000e3 * escala3, glm::vec3(0, 0, 0));
static Planeta tierra(ASTROS_OPTS_SHADERS::PLANET, "tierra", "Tierra2k.jpg", 6378.1e3 * escala1, 149503000e3 * escala2, 0.016, 365.2);
static Planeta venus(ASTROS_OPTS_SHADERS::PLANET, "venus", "2k_venus.jpg", 6051.8e3 * escala1, 108200000e3 * escala2, 0.001, 224.701);
*/

static void window_size_callback(GLFWwindow *window, int width, int height);
static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);
static void resizeGL();
static void resizeGL(int width, int height);
static GLboolean initGL();
static GLboolean init();
static GLboolean loadAstros();
static GLboolean initAstros();
static void terminate();
static void terminateGL();
void main_loop();
// static void displayGL();
// static void displayOrbit(AstroConOrbita &orbit);
// static void displayAstro(Astro &astro);

int main(int, char **)
{
    std::cout << "Hello, world!\n";
#ifndef __EMSCRIPTEN__
    const char *description;
#endif

    camera = new CameraPolar();

    /* Initialize the library */
    if (!glfwInit())
    {
#ifndef __EMSCRIPTEN__
        glfwGetError(&description);
        printf("glfwInit error : %s,", description);
#else
        printf("glfwInit error,");
#endif
        if (camera)
        {
            delete camera;
            camera = NULL;
        }

        return EXIT_FAILURE;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(wwidth, wheight, "System solar", NULL, NULL);
    if (!window)
    {
#ifndef __EMSCRIPTEN__
        glfwGetError(&description);
        printf("glfwInit error : %s,", description);
#else
        printf("glfwInit error,");
#endif

        if (camera)
        {
            delete camera;
            camera = NULL;
        }
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

        if (camera)
        {
            delete camera;
            camera = NULL;
        }
        glfwTerminate();

        return EXIT_FAILURE;
    }
#endif

    resizeGL();
    glfwSetKeyCallback(window, key_callback);
    glfwSetWindowSizeCallback(window, window_size_callback);

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
        {
            aerr.print();
            terminate();
        }
    }
    else
    {
        aerr.print();
        terminateGL();
    }

    return EXIT_SUCCESS;
}

static void window_size_callback(GLFWwindow *window, int width, int height)
{
    wwidth = width;
    wheight = height;
    resizeGL(width, height);
}

static void resizeGL()
{
    int width, height;

    glfwGetWindowSize(window, &width, &height);

    resizeGL(width, height);
}

static void resizeGL(int width, int height)
{
    camera->viewport(0, 0, width, height);
    camera->setProjectionRH(fov, width / (float)height, 0.1f, 10000.0f);
}

static GLboolean initGL()
{
    // if (!stars_tex.load("assets/textures/2k_stars+milky_way.jpg"))
    //     return GL_FALSE;
    if (!skybox_tex.loadCubeMap(
            "assets/textures/skybox_Right.png", "assets/textures/skybox_Left.png",
            "assets/textures/skybox_Top.png", "assets/textures/skybox_Bottom.png",
            "assets/textures/skybox_Back.png", "assets/textures/skybox_Front.png"))
        return GL_FALSE;
    // if (!stars_tex[1].load("assets/textures/skybox_Left.png"))
    //     return GL_FALSE;
    // if (!stars_tex[2].load("assets/textures/skybox_Back.png"))
    //     return GL_FALSE;
    // if (!stars_tex[3].load("assets/textures/skybox_Right.png"))
    //     return GL_FALSE;
    // if (!stars_tex[4].load("assets/textures/skybox_Top.png"))
    //     return GL_FALSE;
    // if (!stars_tex[5].load("assets/textures/skybox_Bottom.png"))
    //     return GL_FALSE;

    // if (!starGLSL.createProgramFromFile("assets/shaders/stars.vs", "assets/shaders/stars.fs"))
    //     return GL_FALSE;

    if (!skyboxGLSL.createProgramFromFile("assets/shaders/skybox.vs", "assets/shaders/skybox.fs"))
        return GL_FALSE;

    if (!planetGLSL.createProgramFromFile("assets/shaders/planet.vs", "assets/shaders/planet.fs"))
        return GL_FALSE;

    if (!orbitGLSL.createProgramFromFile("assets/shaders/orbit.vs", "assets/shaders/orbit.fs"))
        return GL_FALSE;

    // if (!orbitGLSL.createProgramFromFile("assets/simple3.vs", "assets/simple3.fs"))
    //     return GL_FALSE;

    if (!skyboxVAO.MakeSkybox())
        return GL_FALSE;

    if (!sphereVAO.MakeSolidSphere(1, 32, 32, sphereNumIdxs))
        return GL_FALSE;

    // GLuint vao;
    // glGenVertexArrays(1, &vao);
    // glBindVertexArray(vao);

    // GLuint vbos[3];
    // glGenBuffers(3, vbos);

    // glBindBuffer(GL_ARRAY_BUFFER, vbos[0]);
    // glBufferData(GL_ARRAY_BUFFER, sizeof(stars_pos), stars_pos, GL_STATIC_DRAW);
    // glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
    // glEnableVertexAttribArray(0);

    // glBindBuffer(GL_ARRAY_BUFFER, vbos[1]);
    // glBufferData(GL_ARRAY_BUFFER, sizeof(stars_uv), stars_uv, GL_STATIC_DRAW);
    // glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
    // glEnableVertexAttribArray(1);

    // starsVAO = vao;

    // glBindVertexArray(0);
    // glBindBuffer(GL_ARRAY_BUFFER, 0);
    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    /*
        sol.add(&tierra);
        sol.add(&venus);

        tierra.calcDatas();
        venus.calcDatas();
        sol.calcDatas();

        if (!sol.initGL())
            return GL_FALSE;

        if (!tierra.initGL())
            return GL_FALSE;

        if (!venus.initGL())
            return GL_FALSE;
            */

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
    ssolar = new SSolar();
    if (!loadAstros())
        return GL_FALSE;

    if (!ssolar->initGL())
        return GL_FALSE;

    // sol=new Estrella();
    return GL_TRUE;
}

static GLboolean loadAstros()
{
    io::CSVReader<31> sol_in("assets/sol_data.csv");

    sol_in.read_header(io::ignore_missing_column, "eName", "isPlanet", "semimajorAxis", "perihelion", "aphelion", "eccentricity", "inclination", "density", "gravity", "escape", "meanRadius", "equaRadius", "polarRadius", "flattening", "dimension", "sideralOrbit", "sideralRotation", "discoveryDate", "mass_kg", "volume", "orbit_type", "orbits", "bondAlbido", "geomAlbido", "RV_abs", "p_transit", "transit_visibility", "transit_depth", "massj", "semimajorAxis_AU", "grav_int");

    Estrella *sol = new Estrella();

    ssolar->addSol(sol);
    for (;;)
    {
        SolDataItem *data = new SolDataItem();

        if (!sol_in.read_row(data->eName, data->isPlanet, data->semimajorAxis, data->perihelion, data->aphelion, data->eccentricity, data->inclination, data->density, data->gravity, data->escape, data->meanRadius, data->equaRadius, data->polarRadius, data->flattening, data->dimension, data->sideralOrbit, data->sideralRotation, data->discoveryDate, data->mass_kg, data->volume, data->orbit_type, data->orbits, data->bondAlbido, data->geomAlbido, data->RV_abs, data->p_transit, data->transit_visibility, data->transit_depth, data->massj, data->semimajorAxis_AU, data->grav_int))
            break;
        if (data->isPlanet == "TRUE")
            ssolar->add(new Planeta(data));
        else if (data->orbit_type == "Secondary")
        {
            // es un satélite
            // std::cout << data->eName << " " << data->orbits << " " << std::endl;
            if (data->orbits == "NA")
            {
                // es un asteroide
                std::cout << data->eName << std::endl;
            }
            else if (data->eName == "Moon")
                ssolar->add(new Satelite(data));
        }
        else if (data->eName == "Sun")
        {
            sol->setData(data);
        }
        else
        {
            // es un asteroide
            // std::cout << data->eName << std::endl;
        }
    }

    ssolar->fixSatelites();

    // Planeta *p = ssolar->getPlaneta("Earth2");

    // SSolarData

    io::CSVReader<3> ssolar_in("assets/ssolar.csv");

    ssolar_in.read_header(io::ignore_missing_column, "eName", "texture", "tshader");

    SSolarDataItem ssolardata;

    while (ssolar_in.read_row(ssolardata.eName, ssolardata.texture, ssolardata.tshader))
    {
        Astro *astro = ssolar->getAstro(ssolardata.eName);

        if (astro != NULL)
        {
            astro->setFileTex(ssolardata.texture);
            astro->setShader((ASTROS_OPTS_SHADERS)ssolardata.tshader);
        }
    }

    std::cout << "Loaded astros data" << std::endl;

    ssolar->calcDatas(&escalaVistaPlanetaria);

    return GL_TRUE;
}

static void terminateGL()
{
    planetGLSL.Delete();
    // texture.Delete();
}

static void terminate()
{
    if (ssolar)
    {
        delete ssolar;
        ssolar = NULL;
    }
    glfwTerminate();
    if (camera)
    {
        delete camera;
        camera = NULL;
    }
}

static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_X && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    if (key == GLFW_KEY_W)
        obsPos += obsSpeed * obsFront;
    if (key == GLFW_KEY_S)
        obsPos -= obsSpeed * obsFront;
    if (key == GLFW_KEY_A)
        obsPos -= glm::normalize(glm::cross(obsFront, obsUp)) * obsSpeed;
    if (key == GLFW_KEY_D)
        obsPos += glm::normalize(glm::cross(obsFront, obsUp)) * obsSpeed;
#ifndef __EMSCRIPTEN__
    else if (key == GLFW_KEY_F && action == GLFW_PRESS)
    {
        GLFWmonitor *monitor = glfwGetPrimaryMonitor();
        const GLFWvidmode *mode = glfwGetVideoMode(monitor);
        bool isfullscreen = glfwGetWindowMonitor(window) != nullptr;

        if (isfullscreen)
            glfwSetWindowMonitor(window, nullptr, 0, 0, wwwidth, wwheight, wwrefreshrate);

        else
        {
            wwwidth = wwidth;
            wwheight = wheight;
            glfwSetWindowMonitor(window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
        }
    }
#endif
}

inline void displayOrbit(AstroConOrbita &orbit)
{
    // glBindVertexArray(orbitVAO);
    orbitGLSL.Use();
    orbitGLSL.setMVP(camera->getProjectionMatrix() * camera->getviewMatrix());
    orbitGLSL.setCenter((glm::vec3 &)orbit.getCenter());
    orbitGLSL.setA(orbit.getA());
    orbitGLSL.setB(orbit.getB());
    orbitGLSL.setNumVert(32);
    glDrawArrays(GL_LINE_LOOP, 0, 32);
}

inline void displayOrbits(Planeta &planeta)
{
    displayOrbit(planeta);

    auto satelites = planeta.getSatelites();

    for (auto satelite = satelites.begin(); satelite != satelites.end(); satelite++)
    {
        if ((*satelite)->getShader() != ASTROS_OPTS_SHADERS::AOS_NONE)
            displayOrbit(**satelite);
    }
}

inline void displayOrbits(Estrella &estrella)
{
    auto planetas = estrella.getPlanetas();
    for (auto planeta = planetas.begin(); planeta != planetas.end(); planeta++)
    {
        if ((*planeta)->getShader() != ASTROS_OPTS_SHADERS::AOS_NONE)
            displayOrbits(**planeta);
    }
}

inline void displayShaderPlanet(Astro &astro)
{
    astro.getTexture().BindTexture();
    glBindVertexArray(sphereVAO.getVAO());
    planetGLSL.Use();
    planetGLSL.setMVP(camera->getProjectionMatrix() * camera->getviewMatrix() * astro.getModelMatrix());
    glDrawElements(GL_TRIANGLES, sphereNumIdxs, GL_UNSIGNED_INT, BUFFER_OFFSET(0));
}

inline void displayAstro(Astro &astro)
{
    switch (astro.getShader())
    {
    case ASTROS_OPTS_SHADERS::PLANET:
        displayShaderPlanet(astro);
        break;
    }
}

inline void displayPlaneta(Planeta &planeta)
{
    displayAstro(planeta);
    auto satelites = planeta.getSatelites();

    for (auto satelite = satelites.begin(); satelite != satelites.end(); satelite++)
    {
        if ((*satelite)->getShader() != ASTROS_OPTS_SHADERS::AOS_NONE)
            displayAstro(**satelite);
    }
}

inline void displayEstrella(Estrella &estrella)
{
    displayOrbits(estrella);
    displayAstro(estrella);

    auto planetas = estrella.getPlanetas();

    for (auto planeta = planetas.begin(); planeta != planetas.end(); planeta++)
    {
        displayPlaneta(**planeta);
    }
}

inline void displaySSolar()
{
    displayEstrella(*ssolar->getSol());
}

/*

// cuadrado
inline void displayMilkyway()
{
    stars_tex.BindTexture();
    glBindVertexArray(starsVAO);
    starGLSL.Use();
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
}

// esfera
inline void displayMilkyway()
{
    stars_tex.BindTexture();
    glBindVertexArray(sphereVAO.getVAO());
    planetGLSL.Use();
    planetGLSL.setMVP(camera.getProjectionMatrix() * camera.getviewMatrixMilkyway());
    glDrawElements(GL_TRIANGLES, sphereNumIdxs, GL_UNSIGNED_INT, BUFFER_OFFSET(0));
}

*/

inline void displayMilkyway()
{
    skybox_tex.BindTexture();
    glBindVertexArray(skyboxVAO.getVAO());
    skyboxGLSL.Use();
    // Tell the shader to use texture unit 0 for u_skybox
    skyboxGLSL.setSkyboxLocation(0);
    skyboxGLSL.setViewDirectionProjectionInverseLocation(glm::inverse(camera->getProjectionMatrix() * camera->getviewMatrixMilkywayInverse()));
    // let our quad pass the depth test at 1.0
    glDepthFunc(GL_LEQUAL);
    glDrawArrays(GL_TRIANGLES, 0, 1 * 6);
    // glDrawElements(GL_TRIANGLES, 1 * 6, GL_UNSIGNED_INT, BUFFER_OFFSET(0));
}

inline void displayGL()
{
    /* Choose background color */
    // glClearColor(1.0, 0.0, 1.0, 1.0);
    glClearColor(0, 0, 0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    camera->setProjectionRH(fov, wwidth / (float)wheight, 0.1f, 10000.0f);
    camera->lookUp(obsPos, obsCenter, obsUp);

    displayMilkyway();
    displaySSolar();
}

void main_loop()
{
    double _lastTime = glfwGetTime();
    double ellapse = _lastTime - lastTime;

    lastTime = _lastTime;
    // tierra.Orbita(ellapse * scaleTime);
    // venus.Orbita(ellapse * scaleTime);
    ssolar->Orbita(ellapse * scaleTime);

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