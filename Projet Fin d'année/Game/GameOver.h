#ifndef GAME_OVER_H
#define GAME_OVER_H

#include "Common.h"
#include "GameState.h"

void LoadGameOver(void);
void PollEventGameOver(sfRenderWindow* _renderWindow);
void UpdateGameOver(float _dt);
void DrawGameOver(sfRenderWindow* _renderWindow);
void CleanupGameOver(void);

void KeyPressedGameOver(sfRenderWindow* _renderWindow, sfKeyEvent _keyEvent);

#endif // !GAME_OVER_H
