#ifdef __APPLE__
#define GLFW_INCLUDE_GLCOREARB
#elif _WIN32
#include <GL/glew.h>
#endif

#include <GLFW/glfw3.h>

#pragma once

GLFWwindow* window;

void initGlfw();
