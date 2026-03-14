#include <GLFW/glfw3.h>
#include <cstring>
#include <cstdio>
#include <cmath>
#include "../modules/text.h"

#define STB_TRUETYPE_IMPLEMENTATION
#include "../../lib/stb_truetype.h"

static unsigned char fontBitmap[512 * 512];
static stbtt_bakedchar charData[96];
static GLuint fontTex = 0;
static float fontHeight = 32.0f;

static stbtt_fontinfo fontInfo;
static unsigned char* fontBuf = nullptr;

void loadFont(const char* path, float size) {
    fontHeight = size;

    FILE* f = fopen(path, "rb");
    fseek(f, 0, SEEK_END);
    long len = ftell(f);
    rewind(f);

    if (fontBuf) delete[] fontBuf;
    fontBuf = new unsigned char[len];
    fread(fontBuf, 1, len, f);
    fclose(f);

    stbtt_InitFont(&fontInfo, fontBuf, 0);
    stbtt_BakeFontBitmap(fontBuf, 0, size, fontBitmap, 512, 512, 32, 96, charData);

    glGenTextures(1, &fontTex);
    glBindTexture(GL_TEXTURE_2D, fontTex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, 512, 512, 0, GL_ALPHA, GL_UNSIGNED_BYTE, fontBitmap);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void drawText(float x, float y, const char* text, float r, float g, float b, float o) {
    glBindTexture(GL_TEXTURE_2D, fontTex);
    glColor4f(r, g, b, o);

    glBegin(GL_QUADS);
    for (int i = 0, len = (int)strlen(text); i < len; i++) {
        if (text[i] >= 32 && text[i] < 128) {
            stbtt_aligned_quad q;
            stbtt_GetBakedQuad(charData, 512, 512, text[i] - 32, &x, &y, &q, 1);
            glTexCoord2f(q.s0, q.t0); glVertex2f(q.x0, q.y0);
            glTexCoord2f(q.s1, q.t0); glVertex2f(q.x1, q.y0);
            glTexCoord2f(q.s1, q.t1); glVertex2f(q.x1, q.y1);
            glTexCoord2f(q.s0, q.t1); glVertex2f(q.x0, q.y1);
        }
    }
    glEnd();

    glBindTexture(GL_TEXTURE_2D, 0);
}

float getTextWidth(const char* text) {
    float x = 0.0f, y = 0.0f;
    for (int i = 0, len = (int)strlen(text); i < len; i++) {
        if (text[i] >= 32 && text[i] < 128) {
            stbtt_aligned_quad q;
            stbtt_GetBakedQuad(charData, 512, 512, text[i] - 32, &x, &y, &q, 1);
        }
    }
    return x;
}

float getTextHeight(const char* text) {
    float x = 0.0f, y = 0.0f;
    float minY = 1e9f, maxY = -1e9f;
    for (int i = 0, len = (int)strlen(text); i < len; i++) {
        if (text[i] >= 32 && text[i] < 128) {
            stbtt_aligned_quad q;
            stbtt_GetBakedQuad(charData, 512, 512, text[i] - 32, &x, &y, &q, 1);
            if (q.y0 < minY) minY = q.y0;
            if (q.y1 < minY) minY = q.y1;
            if (q.y0 > maxY) maxY = q.y0;
            if (q.y1 > maxY) maxY = q.y1;
        }
    }
    return maxY - minY;
}