#ifndef GAME_STATE_H
#define GAME_STATE_H

#include "Menu.h"
#include "Game.h"
#include "GameOver.h"

typedef enum GameState
{
	MENU,
	GAME,
	GAME_OVER
}GameState;

GameState GetGameState(void);
void SetGameState(GameState _gameState);

#endif // !GAME_STATE_H
