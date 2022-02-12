#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#define GL_GLEXT_PROTOTYPES
#define EGL_EGLEXT_PROTOTYPES
#else
//#include <glad/glad.h>
#include <GL/glew.h>
#endif
#include <GLFW/glfw3.h>