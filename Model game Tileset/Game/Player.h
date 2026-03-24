#ifndef PLAYER_H
#define PLAYER_H

#include "Common.h"

#define PLAYER_WIDTH 32
#define PLAYER_HEIGHT 32

typedef struct Player
{
	sfSprite* sprite;
	sfTexture* texture;

	float speed;
	sfVector2f velocity;
	sfVector2f position;
	sfFloatRect playerRect;
} Player;

void LoadPlayer(void);
void UpdatePlayer(float _dt);
void DrawPlayer(sfRenderWindow* _renderWindow);
void CleanUpPlayer(void);

#endif // !PLAYER_H
