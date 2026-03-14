#pragma once
#include <GLFW/glfw3.h>

inline float circleKey = 3.1415926f;
extern double width;
extern double height;
extern GLFWcursor* hand;
extern GLFWcursor* arrow;

struct vec2 {
    float x = 0;
    float y = 0;
    vec2(float x, float y) : x(x), y(y) {}
    vec2() = default;
};
void execUtils(GLFWwindow* window);
int getfps();
GLuint loadTexture(const char* path);
void drawImage(GLuint tex, float x, float y, float w, float h);
void drawImageVertex(GLuint, vec2 vec1, vec2 vectwo, vec2 vec3, vec2 vec4);
int getMouseX(GLFWwindow* window);
int getMouseY(GLFWwindow* window);
float GetCPUUsage();
float GetRAMUsage();