#include "main.h"
#include "timer.h"
#include "ball.h"
#include "wall.h"
#include "person.h"
#include "TR.h"
#include "obstacle.h"

using namespace std;

GLMatrices Matrices;
GLuint programID;
GLFWwindow *window;
#define MAX_TIME 300
#define WALL_WIDTH 0.5f
long int time_elapsed;
struct Character
{
    unsigned int TextureID; // ID handle of the glyph texture
    glm::ivec2 Size;        // Size of glyph
    glm::ivec2 Bearing;     // Offset from baseline to left/top of glyph
    unsigned int Advance;   // Horizontal offset to advance to next glyph
};
std::map<GLchar, Character> Characters;
int stval = 0;

// variables for game elements are declared here
Wall **wall;
float ppx = 0.0f, ppy = 0.0f;
Person person1;
Person imposter;
int door_x = -1, door_y = -1;
bool moved = false;
int light_mode = 0;
int treasure_cracked = 0;
int imposter_vapourized = 0;
// idk wtf are these pieces of shit
int life = 100, points = 0;
float screen_zoom = 1, screen_center_x = 0, screen_center_y = 0;
float camera_rotation_angle = 90;
int zod[2][2] = {{-1, -1}, {-1, -1}};
Obstacle **obs;
// fps nonsense

TR ob = TR(0, 0, 0.3, COLOR_METAL);
int dist[30][30];
bool flag[30][30];
bool c_flag = false;
Timer t60(1.0 / 60);

vector<pair<int, int>> graph[3030];

void display_health(glm::mat4 VP)
{
    float x_ = person1.x - 3.3, y_ = person1.y + 3.3f;
    wall[0][0].color = COLOR_DG;
    wall[0][0].drawPoly(x_, x_, x_ + (4.0 * life / 100.0f), x_ + (4.0 * life / 100.0f), y_, y_ + 0.2f, y_, y_ + 0.2f, VP);
    wall[0][0].color = COLOR_RED;
    x_ = x_ + (4.0 * life / 100.0f);

    wall[0][0].drawPoly(x_, x_, x_ + (4.0 * (100 - life) / 100.0f), x_ + (4.0 * (100 - life) / 100.0f), y_, y_ + 0.2f, y_, y_ + 0.2f, VP);
    wall[0][0].color = COLOR_GREEN;
}

int hashed(int x, int y)
{
    return x * 100 + y;
}

void make_graph(int px, int py, int cx, int cy)
{
    for (int x = -1; x <= 1; x++)
    {
        for (int y = -1; y <= 1; y++)
        {
            if (!(x == y || x >= 30 || y >= 30 || x < 0 || y < 0 || (px == x && py == y)))
            {
                graph[hashed(cx, cy)].push_back({x, y});
            }
        }
    }
}

void upd_dist(int x, int y, int d)
{
    // std::cout << x << " " << y << " " << d << std::endl;
    if (x >= 30 || y >= 30 || x < 0 || y < 0)
    {
        return;
    }
    if (wall[x][y].color.r == 150)
    {
        dist[x][y] = INT32_MAX;
        return;
    }
    if (flag[x][y] == c_flag && dist[x][y] <= d)
    {
        return;
    }
    else if (flag[x][y] != c_flag)
    {
        flag[x][y] = c_flag;
        dist[x][y] = d;
    }
    else if (dist[x][y] > d)
    {
        dist[x][y] = d;
    }
    upd_dist(x + 1, y, d + 1);
    // upd_dist(x + 1, y + 1, d + 1);
    // upd_dist(x + 1, y - 1, d + 1);
    upd_dist(x - 1, y, d + 1);
    // upd_dist(x - 1, y + 1, d + 1);
    // upd_dist(x - 1, y - 1, d + 1);
    upd_dist(x, y + 1, d + 1);
    upd_dist(x, y - 1, d + 1);
}

void move_imposter()
{
    if (imposter_vapourized == 1)
    {
        return;
    }
    int cord_x = (int)(person1.x + WALL_WIDTH);
    int cord_y = (int)(-1 * (person1.y - WALL_WIDTH));
    if (dist[cord_x][cord_y] != 0)
    {
        c_flag = !c_flag;
        upd_dist(cord_x, cord_y, 0);
    }
    int max_x, max_y;
    cord_x = (int)(imposter.x + WALL_WIDTH);
    cord_y = (int)(-1 * (imposter.y - WALL_WIDTH));

    max_x = cord_x;
    max_y = cord_y;
    for (int i = -1 + cord_x; i <= 1 + cord_x; i++)
    {
        for (int j = -1 + cord_y; j <= cord_y + 1; j++)
        {
            if (i < 0 || i >= 30 || j >= 30 || j < 0 || (i != cord_x && j != cord_y))
            {
                continue;
            }
            else
            {
                // std::cout << (i - cord_x) << " " << j - cord_y << " Dist : " << dist[i][j] << std::endl;
                if (dist[i][j] <= dist[max_x][max_y])
                {
                    max_x = i;
                    max_y = j;
                }
            }
        }
    }
    // std::cout << max_x - cord_x << " " << max_y - cord_y << std::endl;
    imposter.x += ((max_x - cord_x) * WALL_WIDTH / 10.0);
    imposter.y += (-1 * (max_y - cord_y) * WALL_WIDTH / 10.0);
}

// saved me from suicide (jk)
void deleteEverything(VAO *vao)
{
    glDeleteVertexArrays(1, &vao->VertexArrayID);
    glDeleteBuffers(1, &vao->VertexBuffer);
    glDeleteBuffers(1, &vao->ColorBuffer);
    delete (vao);
}

bool inWall(float x, float y)
{
    if (x < -WALL_WIDTH || y > 0.5)
    {
        return true;
    }
    for (int a = 0; a < 30; a++)
    {
        for (int b = 0; b < 30; b++)
        {
            if (wall[a][b].x < x and wall[a][b].y < y and wall[a][b].x + wall[a][b].x_span > x and wall[a][b].y + wall[a][b].y_span > y)
            {
                if (wall[a][b].color.r != 135)
                {
                    return true;
                }
                return false;
            }
        }
    }
    return false;
}

// aalas ka badshah
bool valid_motion(float xf, float yf, int xm, int ym)
{
    // how to check for valid motion?
    float new_x = person1.x + xm * (WALL_WIDTH / 5.0);
    float new_y = person1.y + ym * (WALL_WIDTH / 5.0); // this is poopshit wrong
    // motion
    int cord_x = (int)(new_x + WALL_WIDTH);        //
    int cord_y = (int)(-1 * (new_y - WALL_WIDTH)); //  i can kill for solution to this bug
    // world to local coordinates

    color_t c1 = COLOR_BRICKS;

    if (new_x < -WALL_WIDTH || new_y > WALL_WIDTH || cord_x == 30 || cord_y == 30)
    {
        return false;
    }
    // std::cout << wall[cord_x][cord_y].color.r << " " << cord_x << " " << cord_y << " " << person1.y << " " << person1.x << std::endl;
    if (wall[cord_x][cord_y].color.r != 135 && wall[cord_x][cord_y].color.b != 200)
    {
        return false;
    }
    return true;
}

bool check_points()
{
    int cord_x = (int)(person1.x + WALL_WIDTH);          //
    int cord_y = (int)(-1 * (person1.y - WALL_WIDTH));   //  i can kill for solution to this bug
    int icord_x = (int)(imposter.x + WALL_WIDTH);        //
    int icord_y = (int)(-1 * (imposter.y - WALL_WIDTH)); //  i can kill for solution to this bug
    if (cord_x == icord_x && cord_y == icord_y && imposter_vapourized != 1)
    {
        std::cout << "GAME OVER \n";
        stval = 1;
    }
    if (ppx == person1.x && ppy == person1.y)
    {
        return false;
    }
    else
    {
        ppx = person1.x;
        ppy = person1.y;
        if (obs[cord_x][cord_y].x != -10 && treasure_cracked == 1)
        {
            if (person1.x > obs[cord_x][cord_y].x && person1.x < obs[cord_x][cord_y].x + obs[cord_x][cord_y].x_span && person1.y > obs[cord_x][cord_y].y && person1.y < obs[cord_x][cord_y].y + obs[cord_x][cord_y].y_span)
            {
                if (obs[cord_x][cord_y].type == 1)
                {
                    life -= 1;
                }
                else if (obs[cord_x][cord_y].type == 2)
                {
                    points += 10;
                    std::cout << "Points : " << points << "\n";
                    obs[cord_x][cord_y].type = 0;
                }
                if (life == 0)
                {
                    std::cout << "GAME OVER" << std::endl;
                    stval = 1;
                }
                return true;
            }
        }
        return false;
    }
}

// good work jhonny boi
void gen_random(int **ar, int r, int c)
{
    // for now, r = c
    int m = r;

    for (int x = 0; x < m; x++)
    {
        for (int y = 0; y < m; y++)
        {
            ar[x][y] = 0;
        }
    }
    ar[0][0] = 1;
    for (int x = 0; x < m; x++)
    {
        for (int y = 0; y < m; y++)
        {
            if (x % 2 == 0 and y % 2 == 0)
            {
                if ((rand() % 2 == 0 || y == 0) && x != 0)
                {
                    ar[x][y] = 1;
                    ar[x - 1][y] = 1;
                }
                else
                {
                    ar[x][y] = 1;
                    ar[x][y - 1] = 1;
                }
            }
        }
    }
    while (true)
    {
        int x = 0, y = 0;
        x = rand() % 10;
        y = rand() % 10;
        if (ar[x][y] == 1)
        {
            ar[x][y] = 2;
            zod[0][0] = x;
            zod[0][1] = y;
            break;
        }
    }
    while (true)
    {
        int x = 0, y = 0;
        x = rand() % 10 + 20;
        y = rand() % 10 + 20;
        if (ar[x][y] == 1)
        {
            ar[x][y] = 2;
            zod[1][0] = x;
            zod[1][1] = y;
            break;
        }
    }
}

void lightsOff(glm::mat4 VP, float x_, float y_, float R)
{
    float x11, x22, y11, y22;
    x11 = x_ + R;
    x22 = x_ - R;
    y11 = y_ + R;
    y22 = y_ - R;

    GLfloat ar_bag[] = {
        -10.0f + x_, -10.0f + y_, 0.0f,
        10.0f + x_, -10.0f + y_, 0.0f,
        -10.0f + x_, y22, 0.0f,
        10.0f + x_, -10.0f + y_, 0.0f,
        -10.0f + x_, y22, 0.0f,
        10.0f + x_, y22, 0.0f,
        -10.0f + x_, y11, 0.0f,
        10.0f + x_, y11, 0.0f,
        -10.0f + x_, 10.0f + y_, 0.0f,
        10.0f + x_, y11, 0.0f,
        -10.0f + x_, 10.0f + y_, 0.0f,
        10.0f + x_, 10.0f + y_, 0.0f,
        -10.0f + x_, y22, 0.0f,
        x22, y22, 0.0f,
        x22, y11, 0.0f,
        x22, y11, 0.0f,
        -10.0f + x_, y11, 0.0f,
        -10.0f + x_, y22, 0.0f,
        10.0f + x_, y22, 0.0f,
        x11, y22, 0.0f,
        x11, y11, 0.0f,
        x11, y11, 0.0f,
        10.0f + x_, y11, 0.0f,
        10.0f + x_, y22, 0.0f

    };
    VAO *vao = create3DObject(GL_TRIANGLES, 3 * 8, ar_bag, COLOR_BLACK, GL_FILL);
    draw3DObject(vao);
    deleteEverything(vao);
    for (int x = 0; x < 360; x++)
    {
        float R1, R2;
        R1 = R / 2.5f;
        while (R1 < R)
        {
            R1 += 0.05;
            if (inWall(x_ + R1 * cos(x * M_PI / 180.0f), y_ + R1 * sin(x * M_PI / 180.0f)))
            {
                break;
            }
        }
        // R2 = R / 5.0f;
        // while (R2 < R)
        // {
        //     R2 += 0.05;
        //     if (inWall(x_ + R1 * cos((x + 1) * M_PI / 180.0f), y_ + R1 * sin((x + 1) * M_PI / 180.0f)))
        //     {
        //         break;
        //     }
        // }
        R2 = R1;
        GLfloat arr[] = {
            x_ + R1 * cos(x * M_PI / 180.0f),
            y_ + R1 * sin(x * M_PI / 180.0f),
            0.0f,
            x_ + R2 * cos((x + 1) * M_PI / 180.0f),
            y_ + R2 * sin((x + 1) * M_PI / 180.0f),
            0.0f,
            x_ + 2 * R * cos(x * M_PI / 180.0f),
            y_ + 2 * R * sin(x * M_PI / 180.0f),
            0.0f,
            x_ + R2 * cos((x + 1) * M_PI / 180.0f),
            y_ + R2 * sin((x + 1) * M_PI / 180.0f),
            0.0f,
            x_ + 2 * R * cos(x * M_PI / 180.0f),
            y_ + 2 * R * sin(x * M_PI / 180.0f),
            0.0f,
            x_ + 2 * R * cos((x + 1) * M_PI / 180.0f),
            y_ + R * 2 * sin((x + 1) * M_PI / 180.0f),
            0.0f,
        };
        VAO *vo = create3DObject(GL_TRIANGLES, 3 * 2, arr, COLOR_BLACK, GL_FILL);
        draw3DObject(vo);
        deleteEverything(vo);
    }
}

void func(glm::mat4 VP, VAO *vo)
{

    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate(glm::vec3(0.0f, 0.0f, 0.0f)); // glTranslatef
    glm::mat4 rotate = glm::rotate((float)(0 * M_PI / 180.0f), glm::vec3(1, 0, 0));
    // No need as coords centered at 0, 0, 0 of cube arouund which we waant to rotate
    // rotate          = rotate * glm::translate(glm::vec3(0, -0.6, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(vo);
    deleteEverything(vo);
}

/* Render the scene with openGL */
/* Edit this function according to your assignment */
void draw()
{
    if (light_mode == 1 && time_elapsed % 30 == 0)
    {
        points += 1;
    }
    // clear the color and depth in the frame buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // use the loaded shader program
    // Don't change unless you know what you are doing
    glUseProgram(programID);

    // Eye - Location of camera. Don't change unless you are sure!!
    glm::vec3 eye(person1.x, person1.y, 5);
    // Target - Where is the camera looking at.  Don't change unless you are sure!!
    glm::vec3 target(person1.x, person1.y, 0);
    // Up - Up vector defines tilt of camera.  Don't change unless you are sure!!
    if (stval != 0)
    {
        eye = glm::vec3(0, 0, 5);
        target = glm::vec3(0, 0, 0);
    }
    glm::vec3 up(0, 1, 0);
    // Compute Camera matrix (view)
    Matrices.view = glm::lookAt(eye, target, up); // Rotating Camera for 3D
    // Don't change unless you are sure!!
    // Matrices.view = glm::lookAt(glm::vec3(0, 0, 3), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0)); // Fixed camera for 2D (ortho) in XY plane

    // Compute ViewProject matrix as view/camera might not be changed for this frame (basic scenario)
    // Don't change unless you are sure!!
    glm::mat4 VP = Matrices.projection * Matrices.view;

    // Send our transformation to the currently bound shader, in the "MVP" uniform
    // For each model you render, since the MVP will be different (at least the M part)
    // Don't change unless you are sure!!

    if (stval == 1)
    {
        char *ch1 = (char *)malloc(100);
        ch1[0] = 'G';
        ch1[1] = 'A';
        ch1[2] = 'M';
        ch1[3] = 'E';
        ch1[4] = ' ';
        ch1[5] = 'O';
        ch1[6] = 'U'; // see this nonsense
        ch1[7] = 'E';
        ch1[8] = 'R';
        ob.color = COLOR_METAL;
        ob.renderString(-1 + person1.x, 0 + person1.y, 0.1, 9, ch1);
        free(ch1);
        return;
    }
    else if (stval == 2)
    {
        char *ch1 = (char *)malloc(100);
        ch1[0] = 'T';
        ch1[1] = 'I';
        ch1[2] = 'M';
        ch1[3] = 'E';
        ch1[4] = ' ';
        ch1[5] = 'O';
        ch1[6] = 'U'; // see this nonsense
        ch1[7] = 'E';
        ch1[8] = 'R';
        ob.color = COLOR_METAL;
        ob.renderString(-1 + person1.x, 0 + person1.y, 0.1, 9, ch1);
        free(ch1);
        return;
    }
    else if (stval == 3)
    {
        char *ch1 = (char *)malloc(100);
        ch1[0] = 'Y';
        ch1[1] = 'O';
        ch1[2] = 'U';
        ch1[3] = ' ';
        ch1[4] = 'W';
        ch1[5] = 'O';
        ch1[6] = 'N'; // see this nonsense
        ob.color = COLOR_METAL;
        ob.renderString(-1 + person1.x, 0 + person1.y, 0.1, 7, ch1);
        free(ch1);
        return;
    }

    for (int x = 0; x < 30; x++)
    {
        for (int y = 0; y < 30; y++)
        {
            wall[x][y].renderWall(VP);
            if (treasure_cracked)
                obs[x][y].renderObstacle(VP);
        }
    }
    person1.renderPerson(VP);
    if (imposter_vapourized != 1)
    {
        move_imposter();
        imposter.renderPerson(VP);
    }
    if (light_mode == 1)
    {
        lightsOff(VP, person1.x, person1.y, 2.0f);
    }
    check_points();
    display_health(VP);
    if (light_mode == 1)
    {
        ob.color = COLOR_GOLD;
    }
    else
    {
        ob.color = COLOR_BLACK;
    }
    char *ch = (char *)malloc(100);
    ch[0] = 'L';
    ch[1] = 'I';
    ch[2] = 'F';
    ch[3] = 'E';
    ch[4] = (((char)('0' + life / 100)) == '1' ? '1' : ' ');
    ch[5] = (char)('0' + (life / 10) % 10);
    ch[6] = (char)('0' + life % 10);
    ob.renderString(person1.x - 2.5, person1.y + 3.0, 0.08, 7, ch);
    ch[0] = 'L';
    ch[1] = 'I';
    ch[2] = 'G';
    ch[3] = 'H';
    ch[4] = 'T';
    if (light_mode == 0)
    {
        ch[5] = ' ';
        ch[6] = 'O';
        ch[7] = 'N';
    }
    else
    {
        ch[5] = 'O';
        ch[6] = 'F';
        ch[7] = 'F';
    }
    ob.renderString(person1.x - 2.5, person1.y + 2.7, 0.08, 8, ch);
    ch[0] = 'P';
    ch[1] = 'O';
    ch[2] = 'I';
    ch[3] = 'N';
    ch[4] = 'T';
    ch[5] = 'S';
    ch[6] = ' ';
    ch[7] = (((char)('0' + points / 100)) == '1' ? '1' : ' ');
    ch[8] = (char)('0' + (points / 10) % 10);
    ch[9] = (char)('0' + points % 10);
    ob.renderString(person1.x - 2.5, person1.y + 2.4, 0.08, 10, ch);
    ch[0] = 'T';
    ch[1] = 'A';
    ch[2] = 'S';
    ch[3] = 'K';
    ch[4] = ' ';
    ch[5] = 'L';
    ch[6] = 'E';
    ch[7] = 'F';
    ch[8] = 'T';
    ch[9] = ' ';
    ch[10] = '1';
    ch[11] = ' ';
    ch[12] = '2';

    if (imposter_vapourized == 1)
    {
        ch[10] = ' ';
    }
    if (treasure_cracked == 1)
    {
        ch[12] = ' ';
    }
    ob.renderString(person1.x - 2.5, person1.y + 2.1, 0.08, 13, ch);

    ch[0] = 'T';
    ch[1] = 'A';
    ch[2] = 'S';
    ch[3] = 'K';
    ch[4] = ' ';
    ch[5] = 'D';
    ch[6] = 'O';
    ch[7] = 'N';
    ch[8] = 'E';
    ch[9] = ' ';
    ch[10] = ' ';
    ch[11] = ' ';
    ch[12] = ' ';

    if (imposter_vapourized == 1)
    {
        ch[10] = '1';
    }
    if (treasure_cracked == 1)
    {
        ch[12] = '2';
    }
    ob.renderString(person1.x - 2.5, person1.y + 1.8, 0.08, 13, ch);

    int tt = MAX_TIME * 60 - time_elapsed;
    ch[0] = (char)(((tt / 60) / 100) + '0');
    ch[1] = (char)(((tt / 60) / 10) % 10 + '0');
    ch[2] = (char)(((tt / 60) % 10) + '0');
    ob.renderString(person1.x + 2.5, person1.y + 3.0, 0.08, 3, ch);

    if (time_elapsed % 60 == 0)
    {
        if (time_elapsed / 60 == MAX_TIME)
        {
            std::cout << "TIME'S UP" << std::endl;
            stval = 2;
            // exit(0);
        }
    }
    free(ch); // func(VP,create3DObject(GL_TRIANGLES, 3 * 2, ar, COLOR_RED, GL_FILL));
}

void check_action()
{
    int cord_x = (int)(person1.x + WALL_WIDTH);        //
    int cord_y = (int)(-1 * (person1.y - WALL_WIDTH)); //  i can kill for solution to this bug
    if (cord_x == door_x && cord_y == door_y)
    {
        std::cout << "GAME COMPLETED\nPoints : " << points << "\nHealth : " << life << "%\n";
        stval = 3;
        // exit(0);
    }
    if (cord_x == zod[0][0] and cord_y == zod[0][1] and imposter_vapourized != 1)
    {
        std::cout << "Imposter Vapourized\n";
        imposter_vapourized = 1;
        wall[cord_x][cord_y] = Wall(cord_x - WALL_WIDTH, -1 * cord_y - WALL_WIDTH, 2 * WALL_WIDTH, 2 * WALL_WIDTH, COLOR_GREEN);
        if (imposter_vapourized == 1 and treasure_cracked == 1)
        {
            std::cout << "Exit Unlocked\n";
        }
    }
    else if (cord_x == zod[1][0] and cord_y == zod[1][1] and treasure_cracked != 1)
    {
        std::cout << "Treasure Found\n";
        treasure_cracked = 1;
        int a = 0;
        int c = 400;
        while (a < 10 && c > 0)
        {
            int xx = random() % 30;
            int yy = random() % 30;
            if (obs[xx][yy].x != -10 + WALL_WIDTH && obs[xx][yy].type == 1)
            {
                obs[xx][yy].type = 2;
                obs[xx][yy].color = COLOR_GOLD;
                a++;
                // change color here
            }
            c--;
        }
        wall[cord_x][cord_y] = Wall(cord_x - WALL_WIDTH, -1 * cord_y - WALL_WIDTH, 2 * WALL_WIDTH, 2 * WALL_WIDTH, COLOR_GREEN);
        if (imposter_vapourized == 1 and treasure_cracked == 1)
        {
            std::cout << "Exit Unlocked\n";
            while (true)
            {
                door_x = random() % 30;
                door_y = random() % 30;
                if (wall[door_x][door_y].color.r == 135)
                {
                    int a = 0;
                    if (door_x != 0 && wall[door_x - 1][door_y].color.r == 135)
                        a++;
                    if (door_y != 29 && wall[door_x][door_y + 1].color.r == 135)
                        a++;
                    if (door_y != 0 && wall[door_x][door_y - 1].color.r == 135)
                        a++;
                    if (door_x != 29 && wall[door_x + 1][door_y].color.r == 135)
                        a++;
                    if (a == 1)
                    {
                        wall[door_x][door_y].gate = true;
                        break;
                    }
                }
            }
        }
    }
}

void tick_input(GLFWwindow *window)
{
    // process game controls
    int w = glfwGetKey(window, GLFW_KEY_W);
    int a = glfwGetKey(window, GLFW_KEY_A);
    int s = glfwGetKey(window, GLFW_KEY_S);
    int d = glfwGetKey(window, GLFW_KEY_D);
    if (w && valid_motion(person1.x, person1.y, 0, 1))
    {
        moved = true;
        person1.y += WALL_WIDTH / 5;
    }
    else if (a && valid_motion(person1.x, person1.y, -1, 0))
    {
        moved = true;
        person1.x -= WALL_WIDTH / 5;
        person1.face = -1;
    }
    else if (s && valid_motion(person1.x, person1.y, 0, -1))
    {
        moved = true;
        person1.y -= WALL_WIDTH / 5;
    }
    else if (d && valid_motion(person1.x, person1.y, 1, 0))
    {
        moved = true;
        person1.x += WALL_WIDTH / 5;
        person1.face = 1;
    }
    else if (glfwGetKey(window, GLFW_KEY_0))
    {
        light_mode = 0;
    }
    else if (glfwGetKey(window, GLFW_KEY_1))
    {
        light_mode = 1;
    }
    else if (glfwGetKey(window, GLFW_KEY_SPACE))
    {
        check_action(); // action check if there is any motion
    }
}

void tick_elements()
{
    time_elapsed += 1;

    // tasks for each iteration before rendering
    // ball1.tick();
    // camera_rotation_angle += 1;
}

/* Initialize the OpenGL rendering properties */
/* Add all the models to be created here */
void initGL(GLFWwindow *window, int width, int height)
{
    /* Objects should be created before any other gl function and shaders */
    // Create the models
    for (int x = 0; x < 30; x++)
    {
        for (int y = 0; y < 30; y++)
        {
            dist[x][y] = 1;
            flag[x][y] = false;
        }
    }
    int **arr;
    wall = (Wall **)malloc(30 * sizeof(Wall *));
    obs = (Obstacle **)malloc(30 * sizeof(Obstacle *));
    arr = (int **)malloc(30 * sizeof(int *));
    for (int x = 0; x < 30; x++)
    {
        wall[x] = (Wall *)malloc(30 * sizeof(Wall));
        arr[x] = (int *)malloc(30 * sizeof(int));
        obs[x] = (Obstacle *)malloc(30 * sizeof(Obstacle));
    }
    gen_random(arr, 30, 30);
    int max_x = 0, max_y = 0;
    for (int x = 0; x < 30; x++)
    {
        for (int y = 0; y < 30; y++)
        {
            float xc = x - WALL_WIDTH;
            float yc = -1 * y - WALL_WIDTH;
            if (arr[x][y] == 1)
            {
                wall[x][y] = Wall(xc, yc, 2 * WALL_WIDTH, 2 * WALL_WIDTH, COLOR_GREEN);
                if (x + y > max_x + max_y)
                {
                    max_x = x;
                    max_y = y;
                }
            }
            else if (arr[x][y] == 2)
            {
                // power ups
                wall[x][y] = Wall(xc, yc, 2 * WALL_WIDTH, 2 * WALL_WIDTH, COLOR_BLUE);
            }
            else
            {
                wall[x][y] = Wall(xc, yc, WALL_WIDTH * 2, WALL_WIDTH * 2, COLOR_BRICKS); // wastage of memory but how to handle this??
            }
            if ((arr[x][y] == 1 or arr[x][y] == 2) and rand() % 7 == 1)
            {
                switch (rand() % 4)
                {
                case 0:
                    obs[x][y] = Obstacle(xc, yc, WALL_WIDTH, WALL_WIDTH, COLOR_RED);
                    break;
                case 1:
                    obs[x][y] = Obstacle(xc + WALL_WIDTH, yc, WALL_WIDTH, WALL_WIDTH, COLOR_RED);
                    break;
                case 2:
                    obs[x][y] = Obstacle(xc, yc + WALL_WIDTH, WALL_WIDTH, WALL_WIDTH, COLOR_RED);
                    break;
                case 3:
                    obs[x][y] = Obstacle(xc + WALL_WIDTH, yc + WALL_WIDTH, WALL_WIDTH, WALL_WIDTH, COLOR_RED);
                    break;
                default:
                    break;
                }
            }
            else
            {
                obs[x][y] = Obstacle(-10 + WALL_WIDTH, -10 + WALL_WIDTH, WALL_WIDTH, WALL_WIDTH, COLOR_BLUE);
            }
        }
    }

    imposter = Person(max_x, max_y * -1, COLOR_RED, ALIVE);
    person1 = Person(0.0f, 0.0f, COLOR_BLUE, ALIVE);
    // Create and compile our GLSL program from the shaders
    programID = LoadShaders("../source/shaders/shader.vert", "../source/shaders/shader.frag");
    // Get a handle for our "MVP" uniform
    Matrices.MatrixID = glGetUniformLocation(programID, "MVP");
    reshapeWindow(window, width, height);
    // Background color of the scene
    glClearColor(COLOR_BACKGROUND.r / 256.0, COLOR_BACKGROUND.g / 256.0, COLOR_BACKGROUND.b / 256.0, 0.0f); // R, G, B, A
    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    cout << "VENDOR: " << glGetString(GL_VENDOR) << endl;
    cout << "RENDERER: " << glGetString(GL_RENDERER) << endl;
    cout << "VERSION: " << glGetString(GL_VERSION) << endl;
    cout << "GLSL: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;
}

int main(int argc, char **argv)
{

    srand(time(0));
    int width = 600;
    int height = 600;

    window = initGLFW(width, height);
    initGL(window, width, height);

    int c = 0;
    /* Draw in loop */
    while (!glfwWindowShouldClose(window))
    {
        // Process timers

        if (t60.processTick())
        {
            // OpenGL Draw commands
            draw();
            // Swap Frame Buffer in double buffering
            glfwSwapBuffers(window);

            tick_elements();
            tick_input(window);
        }

        // Poll for Keyboard and mouse events
        glfwPollEvents();
    }

    quit(window);
}

bool detect_collision(bounding_box_t a, bounding_box_t b)
{
    return (abs(a.x - b.x) * 2 < (a.width + b.width)) &&
           (abs(a.y - b.y) * 2 < (a.height + b.height));
}

void reset_screen()
{
    float top = screen_center_y + 4 / screen_zoom;
    float bottom = screen_center_y - 4 / screen_zoom;
    float left = screen_center_x - 4 / screen_zoom;
    float right = screen_center_x + 4 / screen_zoom;
    Matrices.projection = glm::ortho(left, right, bottom, top, 0.1f, 500.0f);
}
