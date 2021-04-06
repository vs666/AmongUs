#include "person.h"
#include "main.h"
Person::Person(float x, float y, color_t color, int state)
{
    this->face = 1;
    this->state = state;
    this->x = x;
    this->y = y;
    this->color = color;
}

void Person::drawPerson(glm::mat4 VP)
{
    float HEAD_RADIUS = 0.1f, BODY_SIZE = 0.1f;
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate(glm::vec3(0.0f, 0.0f, 0.0f)); // glTranslatef
    glm::mat4 rotate = glm::rotate((float)(0 * M_PI / 180.0f), glm::vec3(1, 0, 0));
    // No need as coords centered at 0, 0, 0 of cube arouund which we waant to rotate
    // rotate          = rotate * glm::translate(glm::vec3(0, -0.6, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);

    // draw head
    for (int i = 0; i < 180; i++)
    {
        GLfloat ar1[] = {
            this->x,
            this->y + BODY_SIZE,
            0.0f,
            (this->x + HEAD_RADIUS * cos((i * M_PI / 180.0f))),
            (this->y + BODY_SIZE + HEAD_RADIUS * sin((i * M_PI / 180.0f))),
            0.0f,
            (this->x + HEAD_RADIUS * cos(((i + 1) * M_PI / 180.0f))),
            (this->y + BODY_SIZE + HEAD_RADIUS * sin(((i + 1) * M_PI / 180.0f))),
            0.0f,
        };
        VAO *vo1 = create3DObject(GL_TRIANGLES, 3, ar1, this->color, GL_FILL);
        draw3DObject(vo1);
        deleteEverything(vo1);
    }

    // leg
    float y1 = this->y - BODY_SIZE;
    float x1 = this->x - 0.5 * BODY_SIZE;
    float y2 = this->y - 2 * BODY_SIZE;
    float x2 = x1 - 0.5 * BODY_SIZE;
    GLfloat ar2[] = {
        x1,
        y1,
        0.0f,
        x2,
        y1,
        0.0f,
        x1,
        y2,
        0.0f,
        x2,
        y1,
        0.0f,
        x1,
        y2,
        0.0f,
        x2,
        y2,
        0.0f,
    };
    VAO *vo1 = create3DObject(GL_TRIANGLES, 3 * 2, ar2, this->color, GL_FILL);
    draw3DObject(vo1);
    deleteEverything(vo1);

    y1 = this->y - BODY_SIZE;
    x1 = this->x + 0.5 * BODY_SIZE;
    y2 = this->y - 2 * BODY_SIZE;
    x2 = x1 + 0.5 * BODY_SIZE;
    GLfloat ar21[] = {
        x1,
        y1,
        0.0f,
        x2,
        y1,
        0.0f,
        x1,
        y2,
        0.0f,
        x2,
        y1,
        0.0f,
        x1,
        y2,
        0.0f,
        x2,
        y2,
        0.0f,
    };
    VAO *vo2 = create3DObject(GL_TRIANGLES, 3 * 2, ar21, this->color, GL_FILL);
    draw3DObject(vo2);
    deleteEverything(vo2);

    // bag
    y1 = this->y - BODY_SIZE;
    y2 = this->y + BODY_SIZE / 2.0;
    x1 = this->x - this->face * BODY_SIZE;
    x2 = this->x - 1.5 * this->face * BODY_SIZE;
    GLfloat ar_bag[] = {
        x1,
        y1,
        0.0f,
        x2,
        y1,
        0.0f,
        x1,
        y2,
        0.0f,
        x2,
        y1,
        0.0f,
        x1,
        y2,
        0.0f,
        x2,
        y2,
        0.0f,
    };
    VAO *vo3 = create3DObject(GL_TRIANGLES, 3 * 2, ar_bag, this->color, GL_FILL);
    draw3DObject(vo3);
    deleteEverything(vo3);
    // helmet

    y1 = this->y + BODY_SIZE / 2.0;
    y2 = this->y + 1.5 * BODY_SIZE;
    x1 = this->x;
    x2 = this->x + this->face * BODY_SIZE;
    GLfloat ar_helmet[] = {
        x1,
        y1,
        0.0f,
        x2,
        y1,
        0.0f,
        x1,
        y2,
        0.0f,
        x2,
        y1,
        0.0f,
        x1,
        y2,
        0.0f,
        x2,
        y2,
        0.0f,
    };
    VAO *vo4 = create3DObject(GL_TRIANGLES, 3 * 2, ar_helmet, COLOR_HELMET, GL_FILL);
    draw3DObject(vo4);
    deleteEverything(vo4);

    // body
    GLfloat ar[] = {
        this->x + BODY_SIZE,
        this->y + BODY_SIZE,
        0.0f,
        this->x + BODY_SIZE,
        this->y - BODY_SIZE,
        0.0f,
        this->x - BODY_SIZE,
        this->y - BODY_SIZE,
        0.0f,

        this->x + BODY_SIZE,
        this->y + BODY_SIZE,
        0.0f,
        this->x - BODY_SIZE,
        this->y - BODY_SIZE,
        0.0f,
        this->x - BODY_SIZE,
        this->y + BODY_SIZE,
        0.0f,
    };
    VAO *vo5 = create3DObject(GL_TRIANGLES, 3 * 2, ar, this->color, GL_FILL);
    draw3DObject(vo5);
    deleteEverything(vo5);
}

void Person::renderPerson(glm::mat4 vp)
{
    this->drawPerson(vp);
}