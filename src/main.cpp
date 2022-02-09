#include <iostream>

GLFWwindow *window;

void main_loop();
std::string readFile(const char *filePath);
GLuint LoadShader(GLenum type, const char *shaderSrc);
GLuint LoadShaderFromFile(GLenum type, const char *filePath);
GLuint LoadShaderFromFile(const char *vertex_path, const char *fragment_path);

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

    // init

    GLuint program = LoadShaderFromFile("assets/simple.fs", "assets/simple.vs");

    // destroy

    glDeleteProgram(program);

#ifdef __EMSCRIPTEN__
    emscripten_set_main_loop(main_loop, 0, true);
#else
    while (!glfwWindowShouldClose(window))
        main_loop();
#endif

    glfwTerminate();

    return EXIT_SUCCESS;
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

    glfwSwapBuffers(window);

    /* Poll for and process events */
    glfwPollEvents();
}

std::string readFile(const char *filePath)
{
    std::string content;
    std::ifstream fileStream(filePath, std::ios::in);

    if (!fileStream.is_open())
    {
        std::cerr << "Could not read file " << filePath << ". File does not exist." << std::endl;
        return "";
    }

    std::string line = "";
    while (!fileStream.eof())
    {
        std::getline(fileStream, line);
        content.append(line + "\n");
    }

    fileStream.close();

    return content;
}

GLuint LoadShader(GLenum type, const char *shaderSrc)
{
    GLuint shader;
    GLint compiled;

    shader = glCreateShader(type);
    if (shader == 0)
        return 0;

    glShaderSource(shader, 1, &shaderSrc, NULL);
    glCompileShader(shader);
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
    if (!compiled)
    {
        GLint infoLen = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
        if (infoLen > 1)
        {
            char *infoLog = static_cast<char *>(malloc(sizeof(char) * infoLen));
            glGetShaderInfoLog(shader, infoLen, NULL, infoLog);
            printf("Error compiling shader:\n%s\n", infoLog);
            free(infoLog);
        }
        glDeleteShader(shader);
        return 0;
    }
    return shader;
}

GLuint LoadShaderFromFile(GLenum type, const char *filePath)
{
    std::string src = readFile(filePath);

    return LoadShader(type, src.c_str());
}

GLuint LoadShaderFromFile(const char *vertex_path, const char *fragment_path)
{
    GLuint vertexShader = LoadShaderFromFile(GL_VERTEX_SHADER, vertex_path);

    if (vertexShader <= 0)
        return 0;

    // vertexShader = gldr::LoadShader(GL_VERTEX_SHADER, vShaderStr2);
    GLuint fragmentShader = LoadShaderFromFile(GL_FRAGMENT_SHADER, fragment_path);

    if (fragmentShader < 0)
        return 0;

    GLuint programObject = glCreateProgram();

    return programObject;
}