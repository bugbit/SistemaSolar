#include "shaders.h"
#include "texture.h"

#define BUFFER_OFFSET(offset) ((void *)(offset))

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

static const GLfloat rectangle[] =
    {
        0.0,
        0.0,
        1.0,
        0.0,
        0.0,
        1.0,
        0.0,
        1.0,
        1.0,
        0.0,
        1.0,
        1.0,
};

static GLFWwindow *window;
static SimpleShaderProgram simpleGLSL;
static Texture2d texture;
static GLuint rectangleVAO;

static GLboolean initGL();
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

    if (initGL())
    {
#ifdef __EMSCRIPTEN__
        emscripten_set_main_loop(main_loop, 0, true);
#else
        while (!glfwWindowShouldClose(window))
            main_loop();

        terminate();

#endif
    }
    else
        terminate();

    return EXIT_SUCCESS;
}

static GLboolean initGL()
{
    if (!texture.load("assets/Tierra2k.jpg"))
        return GL_FALSE;

    if (!simpleGLSL.createProgramFromFile("assets/simple.vs", "assets/simple.fs"))
        return GL_FALSE;

    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    GLuint vbos;
    glGenBuffers(1, &vbos);

    glBindBuffer(GL_ARRAY_BUFFER, vbos);
    glBufferData(GL_ARRAY_BUFFER, sizeof(rectangle), rectangle, GL_STATIC_DRAW);
    glVertexAttribPointer(simpleGLSL.GetIndexVPosition(), 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
    glEnableVertexAttribArray(simpleGLSL.GetIndexVPosition());

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    rectangleVAO = vao;

    return GL_TRUE;
}

static void terminateGL()
{
    simpleGLSL.Delete();
    texture.Delete();
}

static void terminate()
{
    glfwTerminate();

    terminateGL();
}

void main_loop()
{
    int width, height;
    glfwGetWindowSize(window, &width, &height);

    glViewport(0, 0, width, height);

    /* Choose background color */
    // glClearColor(1.0, 0.0, 1.0, 1.0);
    glClearColor(0, 0, 0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    glBindVertexArray(rectangleVAO);
    simpleGLSL.Use();
    glDrawArrays(GL_TRIANGLES, 0, 3 + 3);

    glfwSwapBuffers(window);

    /* Poll for and process events */
    glfwPollEvents();
}