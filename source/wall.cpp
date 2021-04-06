#include "wall.h"
#include "main.h"

Wall::Wall(float x, float y, float x_span, float y_span, color_t color)
{
    this->position = glm::vec3(x, y, 0); // set bottom left of the wall
    this->x = x;
    this->y = y;
    this->x_span = x_span;
    this->y_span = y_span;
    this->color = color;
    this->gate = false;
}

void Wall::drawPoly(float x1, float x2, float x3, float x4, float y1, float y2, float y3, float y4, glm::mat4 VP)
{

    GLfloat ar1[] = {
        x1,
        y1,
        0.0f,
        x2,
        y2,
        0.0f,
        x3,
        y3,
        0.0f,
        x4,
        y4,
        0.0f,
        x3,
        y3,
        0.0f,
        x2,
        y2,
        0.0f,
        x1,
        y1,
        0.0f,
        x2,
        y2,
        0.0f,
        x3,
        y3,
        0.0f,
        x4,
        y4,
        0.0f,
        x3,
        y3,
        0.0f,
        x2,
        y2,
        0.0f,
    };
    if (this->color.r == 100)
    {

        // make lever
        ar1[0] = (this->x + this->x_span / 2.0) + 0.3;
        ar1[1] = (this->y + this->y_span / 2.0) + 0.3;
        ar1[2] = 0.0;
        ar1[3] = (this->x + this->x_span / 2.0) - 0.3;
        ar1[4] = (this->y + this->y_span / 2.0) + 0.3;
        ar1[5] = 0.0;
        ar1[6] = (this->x + this->x_span / 2.0) - 0.3;
        ar1[7] = (this->y + this->y_span / 2.0) - 0.3;
        ar1[8] = 0.0;
        ar1[9] = (this->x + this->x_span / 2.0) - 0.3;
        ar1[10] = (this->y + this->y_span / 2.0) - 0.3;
        ar1[11] = 0.0;
        ar1[12] = (this->x + this->x_span / 2.0) + 0.3;
        ar1[13] = (this->y + this->y_span / 2.0) - 0.3;
        ar1[14] = 0.0;
        ar1[15] = (this->x + this->x_span / 2.0) + 0.3;
        ar1[16] = (this->y + this->y_span / 2.0) + 0.3;
        ar1[17] = 0.0;
        ar1[18] = (this->x + this->x_span / 2.0) + 0.2;
        ar1[19] = (this->y + this->y_span / 2.0) + 0.2;
        ar1[20] = 0.2;
        ar1[21] = (this->x + this->x_span / 2.0) - 0.2;
        ar1[22] = (this->y + this->y_span / 2.0) + 0.2;
        ar1[23] = 0.2;
        ar1[24] = (this->x + this->x_span / 2.0) - 0.2;
        ar1[25] = (this->y + this->y_span / 2.0) - 0.2;
        ar1[26] = 0.2;
        ar1[27] = (this->x + this->x_span / 2.0) - 0.2;
        ar1[28] = (this->y + this->y_span / 2.0) - 0.2;
        ar1[29] = 0.2;
        ar1[30] = (this->x + this->x_span / 2.0) + 0.2;
        ar1[31] = (this->y + this->y_span / 2.0) - 0.2;
        ar1[32] = 0.2;
        ar1[33] = (this->x + this->x_span / 2.0) + 0.2;
        ar1[34] = (this->y + this->y_span / 2.0) + 0.2;
        ar1[35] = 0.2;
    }

    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate(glm::vec3(0.0f, 0.0f, 0.0f)); // glTranslatef
    glm::mat4 rotate = glm::rotate((float)(0 * M_PI / 180.0f), glm::vec3(1, 0, 0));
    // No need as coords centered at 0, 0, 0 of cube arouund which we waant to rotate
    // rotate          = rotate * glm::translate(glm::vec3(0, -0.6, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    VAO *vo = create3DObject(GL_TRIANGLES, (this->color.r == 100) ? (12) : (3 * 2), ar1, (this->color.r == 100) ? COLOR_METAL : (this->color), GL_FILL);
    draw3DObject(vo);
    deleteEverything(vo);
    if (this->gate)
    {
        GLfloat ar2[] = {
            this->x + this->x_span / 2.0 - 0.2,
            this->y + this->y_span / 2.0 + 0.2,
            0.0,
            this->x + this->x_span / 2.0 - 0.1,
            this->y + this->y_span / 2.0 - 0.2,
            0.0,
            this->x + this->x_span / 2.0 + 0.3,
            this->y + this->y_span / 2.0 + 0.2,
            0.0,
        };
        VAO *vo1 = create3DObject(GL_TRIANGLES, 3, ar2, COLOR_METAL, GL_FILL);
        draw3DObject(vo1);
        deleteEverything(vo1);

        GLfloat ar3[] = {
            this->x + this->x_span / 2.0 - 0.1,
            this->y + this->y_span / 2.0 + 0.2,
            0.0,
            this->x + this->x_span / 2.0,
            this->y + this->y_span / 2.0 - 0.1,
            0.0,
            this->x + this->x_span / 2.0 + 0.2,
            this->y + this->y_span / 2.0 + 0.2,
            0.0,
        };
        VAO *vo2 = create3DObject(GL_TRIANGLES, 3, ar3, COLOR_GREEN, GL_FILL);
        draw3DObject(vo2);
        deleteEverything(vo2);
    }
}

void Wall::renderWall(glm::mat4 vp)
{
    this->drawPoly(this->x, this->x, this->x + this->x_span, this->x + this->x_span, this->y, this->y + this->y_span, this->y, this->y + this->y_span, vp);
}
