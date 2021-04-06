#include <random>
#include <time.h>
#ifndef __MAZE_H_
#define __MAZE_H_

#define WALL_CODE 0
#define PASSAGE_CODE 1

// uses the fastest and truly memoryless maze generation algorithm using binary trees.
void gen_random(int **ar, int r, int c);// for now r = c

#endif
