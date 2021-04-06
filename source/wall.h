#include "main.h"

#ifndef WALL_H
#define WALL_H


class Wall {
    public:
    Wall(){}
    Wall(float x,float y,float x_span,float y_span,color_t color);
    glm::vec3 position;
    color_t color;
    bool gate;
    float x,y,x_span,y_span;
    void drawPoly(float x1,float x2,float x3,float x4,float y1,float y2,float y3,float y4,glm::mat4 vp);
    void renderWall(glm::mat4 vp);
};

#endif