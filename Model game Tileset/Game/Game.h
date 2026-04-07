#ifndef GAME_H
#define GAME_H

#include "Common.h"
#include "GameState.h"

#include "Map.h"
#include "Player.h"
#include "Camera.h"
#include "Mob.h"
#include "GameHUD.h"

void LoadGame(void);
void PollEventGame(sfRenderWindow* _renderWindow);
void UpdateGame(float _dt);
void DrawGame(sfRenderWindow* _renderWindow);
void CleanupGame(void);

void KeyPressedGame(sfRenderWindow* _renderWindow, sfKeyEvent _keyEvent);
void changeLevel(const char* level);



#endif // !GAME_H
