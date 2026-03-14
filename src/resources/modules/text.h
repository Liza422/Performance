#pragma once
#include <GLFW/glfw3.h>

void loadFont(const char* path, float size);
void drawText(float x, float y, const char* text, float r, float g, float b, float o);
float getTextWidth(const char* text);
float getTextHeight(const char* text);