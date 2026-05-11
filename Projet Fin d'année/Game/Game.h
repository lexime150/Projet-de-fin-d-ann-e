#ifndef GAME_H
#define GAME_H



#include "Common.h"
#include "GameState.h"

#include "Map.h"
#include "GameHUD.h"
#include "Key.h"

#include "Camera.h"
#include "Player.h"
#include "Mob.h"
#include "Collectibles.h"
#include "Boss.h"
#include "Angel_Statue.h"


void LoadGame(void);
void PollEventGame(sfRenderWindow* _renderWindow);
void UpdateGame(sfRenderWindow* _renderWindow, float _dt);
void DrawGame(sfRenderWindow* _renderWindow);
void CleanupGame(void);

void KeyPressedGame(sfRenderWindow* _renderWindow, sfKeyEvent _keyEvent);

void ChangeLevel(const char* level);




#endif // !GAME_H
