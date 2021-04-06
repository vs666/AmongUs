// #include "maze.h"



// void gen_random(int **ar,int r, int c)
// {
//     // for now, r = c
//     int m = r;
//     for (int x = 0; x < m; x++)
//     {
//         for (int y = 0; y < m; y++)
//         {
//             ar[x][y] = 0;
//         }
//     }
//     ar[0][0] = 1;
//     for (int x = 0; x < m; x++)
//     {
//         for (int y = 0; y < m; y++)
//         {
//             if (x % 2 == 0 and y % 2 == 0)
//             {
//                 if ((rand() % 2 == 0 || y == 0) && x != 0)
//                 {
//                     ar[x][y] = 1;
//                     ar[x - 1][y] = 1;
//                 }
//                 else
//                 {
//                     ar[x][y] = 1;
//                     ar[x][y - 1] = 1;
//                 }
//             }
//         }
//     }
//     for (int x = 0; x < m; x++)
//     {
//         for (int y = 0; y < m; y++)
//         {
//             cout << ar[x][y];
//         }
//         cout << endl;
//     }
//     for (int x = 0; x < m; x++)
//     {
//         for (int y = 0; y < m; y++)
//         {
//             if (ar[x][y] == 1)
//             {
//                 cout << " ";
//             }
//             else
//             {
//                 cout << ".";
//             }
//         }
//         cout << endl;
//     }
// }