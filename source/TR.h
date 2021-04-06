#ifndef _TR_H_
#define _TR_H_

#include "main.h"
class TR
{
public:
    TR() {};
    TR(float x, float y, float size, color_t color);
    glm::vec3 position;
    color_t color;
    float x, y, size;
    void renderString(float x, float y, float size,int len, char* str);
    void renderChar(float x, float y, float size, char str);
};

#endif