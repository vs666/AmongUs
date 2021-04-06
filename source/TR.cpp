#include "TR.h"
#include "main.h"
TR::TR(float x, float y, float size, color_t color)
{
    this->position = glm::vec3(x, y, 0); // set bottom left of the wall
    this->x = x;
    this->y = y;
    this->size = size;
    this->color = color;
}

void TR::renderChar(float x, float y, float size, char str)
{
    float w = 0.01; // width
    GLfloat ar[] = {
        // 4
        x,
        y + 2 * size,
        0,
        x,
        y + 2 * size + w,
        0,
        x + size,
        y + 2 * size + w,
        0,
        x + size,
        y + 2 * size + w,
        0,
        x + size,
        y + 2 * size,
        0,
        x,
        y + 2 * size,
        0,

        // 5
        x + size,
        y + 2 * size,
        0,
        x + size,
        y + 2 * size + w,
        0,
        x + size + size,
        y + 2 * size + w,
        0,
        x + size + size,
        y + 2 * size + w,
        0,
        x + size + size,
        y + 2 * size,
        0,
        x + size,
        y + 2 * size,
        0,

        // 2
        x,
        y + size,
        0,
        x,
        y + size + w,
        0,
        x + size,
        y + size + w,
        0,
        x + size,
        y + size + w,
        0,
        x + size,
        y + size,
        0,
        x,
        y + size,
        0,

        // 3
        x + size,
        y + size,
        0,
        x + size,
        y + size + w,
        0,
        x + size + size,
        y + size + w,
        0,
        x + size + size,
        y + size + w,
        0,
        x + size + size,
        y + size,
        0,
        x + size,
        y + size,
        0,

        // 0
        x,
        y,
        0,
        x,
        y + w,
        0,
        x + size,
        y + w,
        0,
        x + size,
        y + w,
        0,
        x + size,
        y,
        0,
        x,
        y,
        0,

        // 1
        x + size,
        y,
        0,
        x + size,
        y + w,
        0,
        x + size + size,
        y + w,
        0,
        x + size + size,
        y + w,
        0,
        x + size + size,
        y,
        0,
        x + size,
        y,
        0,

        //
        x,
        y + size,
        0,
        x + w,
        y + size,
        0,
        x + w,
        y + size + size,
        0,
        x + w,
        y + size + size,
        0,
        x,
        y + size + size,
        0,
        x,
        y + size,
        0,

        //
        x,
        y,
        0,
        x + w,
        y,
        0,
        x + w,
        y + size,
        0,
        x + w,
        y + size,
        0,
        x,
        y + size,
        0,
        x,
        y,
        0,

        //
        x + size,
        y + size,
        0,
        x + size + w,
        y + size,
        0,
        x + size + w,
        y + size + size,
        0,
        x + size + w,
        y + size + size,
        0,
        x + size,
        y + size + size,
        0,
        x + size,
        y + size,
        0,

        //
        x + size,
        y,
        0,
        x + size + w,
        y,
        0,
        x + size + w,
        y + size,
        0,
        x + size + w,
        y + size,
        0,
        x + size,
        y + size,
        0,
        x + size,
        y,
        0,

        //
        x + size + size,
        y + size,
        0,
        x + size + size + w,
        y + size,
        0,
        x + size + size + w,
        y + size + size,
        0,
        x + size + size + w,
        y + size + size,
        0,
        x + size + size,
        y + size + size,
        0,
        x + size + size,
        y + size,
        0,

        //
        x + size + size,
        y,
        0,
        x + size + size + w,
        y,
        0,
        x + size + size + w,
        y + size,
        0,
        x + size + size + w,
        y + size,
        0,
        x + size + size,
        y + size,
        0,
        x + size + size,
        y,
        0,

    };

    int chti[] = {
        1,
        0,
        0,
        0,
        1,
        0,
        1,
        1,
        1,
        1,
        0,
        0, //0
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        1,
        1,
        0,
        0, //1
        1,
        0,
        1,
        0,
        1,
        0,
        0,
        1,
        1,
        0,
        0,
        0, //2
        1,
        0,
        1,
        0,
        1,
        0,
        0,
        0,
        1,
        1,
        0,
        0, //3
        0,
        0,
        1,
        0,
        0,
        0,
        1,
        0,
        1,
        1,
        0,
        0, //4
        1,
        0,
        1,
        0,
        1,
        0,
        1,
        0,
        0,
        1,
        0,
        0, //5
        1,
        0,
        1,
        0,
        1,
        0,
        1,
        1,
        0,
        1,
        0,
        0, //6
        1,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        1,
        1,
        0,
        0, //7
        1,
        0,
        1,
        0,
        1,
        0,
        1,
        1,
        1,
        1,
        0,
        0, //8
        1,
        0,
        1,
        0,
        1,
        0,
        1,
        0,
        1,
        1,
        0,
        0,                                  //9
        1, 0, 1, 0, 0, 0, 1, 1, 1, 1, 0, 0, // A
        1, 1, 0, 1, 1, 1, 0, 0, 1, 1, 1, 1, // B
        1, 1, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, // C
        1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 1, 1, // D
        1, 1, 1, 0, 1, 1, 1, 1, 0, 0, 0, 0, // E
        1, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, // F
        1, 1, 0, 1, 1, 1, 1, 1, 0, 0, 0, 1, // G
        0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, // H
        1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, // I
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // J
        0, 1, 1, 1, 0, 0, 1, 1, 1, 0, 0, 1, // K (problem)
        0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, // L
        1, 1, 0, 0, 0, 0, 1, 1, 1, 0, 1, 1, // M
        0, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, // N
        1, 1, 0, 0, 1, 1, 1, 1, 0, 0, 1, 1, // O
        1, 1, 1, 1, 0, 0, 1, 1, 0, 0, 1, 0, // P
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // Q
        1, 1, 1, 0, 0, 0, 1, 1, 0, 1, 1, 0, // R
        1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, // S
        1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, // T
        0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 1, 1, // U
        0, 0, 0, 1, 1, 0, 1, 1, 0, 1, 1, 0, // V
        0, 0, 0, 0, 1, 1, 1, 1, 0, 1, 1, 1, // W
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // X
        0, 0, 1, 1, 0, 0, 1, 0, 0, 1, 1, 0, // Y
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // Z
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // -
    };
    int n = str - '0';
    if (str >= 'A')
    {
        n = 10 + str - 'A';
    }
    int count = 0;
    for (int y = 0; y < 12; y++)
    {
        if (chti[y + n * 12] == 1) // || true)
        // if(y==2)
        {
            VAO *vo = create3DObject(GL_TRIANGLES, 2 * 3, &ar[18 * y], this->color, GL_FILL);
            draw3DObject(vo);
            deleteEverything(vo);
            // do something
        }
    }
}

void TR::renderString(float x, float y, float size, int len, char *str)
{
    for (int a = 0; a < len; a++)
    {
        if (str[a] == ' ')
        {
            x += (2 * size + 0.1);
            continue;
        }
        renderChar(x, y, size, str[a]);
        x += (2 * size + 0.04);
    }
}