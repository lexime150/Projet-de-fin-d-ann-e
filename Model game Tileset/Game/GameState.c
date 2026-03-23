#include "GameState.h"

GameState gameState = -1;

GameState GetGameState(void)
{
    return gameState;
}

void SetGameState(GameState _gameState)
{
    // Cleanup the previous gameState
    switch (gameState)
    {
    case MENU:
        CleanupMenu();
        break;
    case GAME:
        CleanupGame();
        break;
    case GAME_OVER:
        CleanupGameOver();
        break;
    default:
        break;
    }

    gameState = _gameState;

    switch (gameState)
    {
    case MENU:
        LoadMenu();
        break;
    case GAME:
        LoadGame();
        break;
    case GAME_OVER:
        LoadGameOver();
        break;
    default:
        break;
    }
}
