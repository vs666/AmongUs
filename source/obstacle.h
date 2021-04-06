#ifndef _OBSTACLE_H_
#define _OBSTACLE_H_

#include "main.h"

class Obstacle {
    public:
    Obstacle(){}
    Obstacle(float x,float y,float x_span,float y_span,color_t color);
    glm::vec3 position;
    color_t color;
    float x,y,x_span,y_span;
    int type;
    void drawPoly(float x1,float x2,float x3,float x4,float y1,float y2,float y3,float y4,glm::mat4 vp);
    void renderObstacle(glm::mat4 vp);
};

#endif