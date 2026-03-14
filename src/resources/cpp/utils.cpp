#include <GLFW/glfw3.h>
#include "../modules/utils.h"
#include "../../lib/stb_image.h"
#include <windows.h>

double width = 0;
double height = 0;
GLFWcursor* hand = nullptr;
GLFWcursor* arrow = nullptr;

void execUtils(GLFWwindow* window) {
    hand = glfwCreateStandardCursor(GLFW_HAND_CURSOR);
    arrow = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);

    int w, h;
    glfwGetWindowSize(window, &w, &h);
    width = (double)w;
    height = (double)h;
}

int getfps() {
    static double lastTime = glfwGetTime();
    static int frameCount = 0;
    static int fps = 0;

    frameCount++;
    double currentTime = glfwGetTime();

    if (currentTime - lastTime >= 1.0) {
        fps = frameCount;
        frameCount = 0;
        lastTime = currentTime;
    }

    return fps;
}

GLuint loadTexture(const char* path) {
    int w, h, channels;
    unsigned char* data = stbi_load(path, &w, &h, &channels, 4);

    GLuint tex;
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

    stbi_image_free(data);
    return tex;
}

void drawImage(GLuint tex, float x, float y, float w, float h) {
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, tex);
    glColor3f(1, 1, 1);

    glBegin(GL_QUADS);
        glTexCoord2f(0, 0); glVertex2f(x,     y);
        glTexCoord2f(1, 0); glVertex2f(x + w, y);
        glTexCoord2f(1, 1); glVertex2f(x + w, y + h);
        glTexCoord2f(0, 1); glVertex2f(x,     y + h);
    glEnd();

    glDisable(GL_TEXTURE_2D);
}
void drawImageVertex(GLuint tex, vec2 v1, vec2 v2, vec2 v3, vec2 v4) {
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, tex);
    glColor3f(1, 1, 1);

    glBegin(GL_QUADS);
        glTexCoord2f(0, 0); glVertex2f(v1.x, v1.y);
        glTexCoord2f(1, 0); glVertex2f(v2.x, v2.y);
        glTexCoord2f(1, 1); glVertex2f(v3.x, v3.y);
        glTexCoord2f(0, 1); glVertex2f(v4.x, v4.y);
    glEnd();

    glDisable(GL_TEXTURE_2D);
}
int getMouseX(GLFWwindow* window) {
    double mouseX, mouseY;
    glfwGetCursorPos(window, &mouseX, &mouseY);
    return (int)mouseX;
}

int getMouseY(GLFWwindow* window) {
    double mouseX, mouseY;
    glfwGetCursorPos(window, &mouseX, &mouseY);
    return (int)mouseY;
}
ULONGLONG FileTimeToULL(FILETIME ft) {
    return ((ULONGLONG)ft.dwHighDateTime << 32) | ft.dwLowDateTime;
}

float GetCPUUsage() {
    static FILETIME prevIdle, prevKernel, prevUser;

    FILETIME idle, kernel, user;
    GetSystemTimes(&idle, &kernel, &user);

    ULONGLONG idleDiff   = FileTimeToULL(idle)   - FileTimeToULL(prevIdle);
    ULONGLONG kernelDiff = FileTimeToULL(kernel)  - FileTimeToULL(prevKernel);
    ULONGLONG userDiff   = FileTimeToULL(user)    - FileTimeToULL(prevUser);

    prevIdle   = idle;
    prevKernel = kernel;
    prevUser   = user;

    ULONGLONG total = kernelDiff + userDiff;
    if (total == 0) return 0.0f;

    return (1.0f - (float)idleDiff / total) * 100.0f;
}
float GetRAMUsage() {
    MEMORYSTATUSEX mem;
    mem.dwLength = sizeof(mem);
    GlobalMemoryStatusEx(&mem);
    return (float)(mem.ullTotalPhys - mem.ullAvailPhys) / mem.ullTotalPhys * 100.0f;
}