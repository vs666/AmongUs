#include "main.h"
#ifndef __PERSON_H
#define __PERSON_H

#define ALIVE 1
#define DEAD 0


class Person {
    public:
    glm::vec3 position;
    color_t color;
    int state,face;
    float x,y,x_span,y_span;
    Person(){}
    Person(float x,float y,color_t color,int state); // 0 for alive 1 for dead
    void drawPerson(glm::mat4 vp);
    void renderPerson(glm::mat4 vp);
};


#endif