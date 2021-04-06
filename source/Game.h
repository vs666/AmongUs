#ifndef __GAME_H_
#define __GAME_H_

#include "main.h"

class Game
{
public:
    enum GameState
    {
        GAME_ACTIVE,
        GAME_PAUSE,
        GAME_OVER
    };
    GameState State;
    bool keys[1024];
    unsigned int Width, Height;

    Game(unsigned int width, unsigned int height);
    ~Game();
    void Init();
    void ProcessInputs(float dt);
    void Update(float dt);
    void Render();
}

#endif