#include <GLFW/glfw3.h>
#include <windows.h>
#include <cmath>
#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#include "lib/stb_image.h"
#include "./resources/modules/text.h"
#include "./resources/modules/utils.h"
#include "./resources/modules/data.h"
#include <thread>

using namespace std;

void cursor(GLFWwindow* win) {
    int mx = getMouseX(win);
    int my = getMouseY(win);
}

void key_callback(GLFWwindow* win, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwDestroyWindow(win);
        glfwTerminate();
        exit(0);
    }
    if (key == GLFW_KEY_L && action == GLFW_PRESS) {
        if (enabled) {
            enabled = false;
        } else {
            enabled = true;
        }
    }
}
void button_callback(GLFWwindow* win, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        int mx = getMouseX(win);
        int my = getMouseY(win);
    }
}
void draw() {
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glColor3f(1.0f, 1.0f, 1.0f);

    glColor3f(0.28, 0.28, 0.28);
    glBegin(GL_QUADS);
        glVertex2f(0, 0);
        glVertex2f(width, 0);
        glVertex2f(width, height);
        glVertex2f(0, height);
    glEnd();

    if (enabled) {
        loadFont("./resources/fonts/arial.ttf", 24);
        drawText(20, 30 * 1, cpubuff, 1, 1, 1, 1);
        drawText(20, 30 * 2, rambuff, 1, 1, 1, 1);
        drawText(20, 30 * 3, freediskspace, 1, 1, 1, 1);
        drawText(20, 30 * 4, diskspace, 1, 1, 1, 1);
        if (wtOpacity > 0) {
            drawText(20, 30 * 5, "Feito por Liz :3", 1, 1, 1, wtOpacity);
            wtOpacity -= 0.005;
        }
    }

    glDisable(GL_BLEND);
    glDisable(GL_TEXTURE_2D);
}
int main() {
    if (!glfwInit()) { cout << "PANIC!" << endl; return -1; }

    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    GLFWwindow* win = glfwCreateWindow(300, 400, "Performance", NULL, NULL);
    if (!win) {
        cout << "Window err :(" << endl;
        glfwTerminate();
        return -1;
    }
    GLFWimage icon;
    int iconWidth, iconHeight, iconChannels;
    icon.pixels = stbi_load("./resources/icon.png", &iconWidth, &iconHeight, &iconChannels, 4);
    icon.width = iconWidth;
    icon.height = iconHeight;
    glfwSetWindowIcon(win, 1, &icon);
    stbi_image_free(icon.pixels);

    glfwMakeContextCurrent(win);
    glfwSetKeyCallback(win, key_callback);
    glfwSetMouseButtonCallback(win, button_callback);

    execUtils(win);

    int fbw, fbh;
    glfwGetFramebufferSize(win, &fbw, &fbh);
    glViewport(0, 0, fbw, fbh);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, width, height, 0, -1, 1);

    glfwSetWindowPos(win, GetSystemMetrics(SM_CXSCREEN)-width, GetSystemMetrics(SM_CYSCREEN)-height-50);

    thread update_values([] () {
        while (running && enabled) {
            snprintf(cpubuff, size(cpubuff), "CPU: %d%%", (int)roundf(GetCPUUsage()));
            snprintf(rambuff, size(cpubuff), "RAM: %d%%", (int)roundf(GetRAMUsage()));

            ULARGE_INTEGER free, total, freetotal;
            GetDiskFreeSpaceEx(NULL, &free, &total, &freetotal);

            snprintf(diskspace, size(diskspace), "Espaco no disco: %d mb", total.QuadPart / 1048576);

            Sleep(1000);
        }
    });
    thread freespacethread([] () {
        while (running && enabled) {
            ULARGE_INTEGER free, total, freetotal;
            GetDiskFreeSpaceEx(NULL, &free, &total, &freetotal);

            snprintf(freediskspace, size(freediskspace), "Espaco livre: %d mb", freetotal.QuadPart / 1048576);

            Sleep(1200000);
        }
    });
    freespacethread.detach();
    update_values.detach();

    while (!glfwWindowShouldClose(win)) {
        glClear(GL_COLOR_BUFFER_BIT);

        draw();
        cursor(win);

        glfwSwapBuffers(win);
        glfwPollEvents();
    }

    running = false;
    glfwDestroyWindow(win);
    glfwTerminate();
    return 0;
}