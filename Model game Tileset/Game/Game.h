#ifndef GAME_H
#define GAME_H

#include "Common.h"
#include "GameState.h"
#include "Animation.h"

#include "Map.h"
#include "Player.h"

typedef struct Game
{
	int test;
}Game;

typedef struct player
{
	sfTexture* texture;
	sfSprite* sprite;

	Animation animation;
	Animation* currentAnimation;

}Player;


void LoadGame(void);
void PollEventGame(sfRenderWindow* _renderWindow);
void UpdateGame(float _dt);
void DrawGame(sfRenderWindow* _renderWindow);
void CleanupGame(void);

void KeyPressedGame(sfRenderWindow* _renderWindow, sfKeyEvent _keyEvent);

#endif // !GAME_H
