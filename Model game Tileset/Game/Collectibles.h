#ifndef COLLECTIBLES_H
#define COLLECTIBLES_H

#include "Common.h"
#include "Player.h"
#include "Mob.h"
#include "Map.h"

typedef struct Orbs
{
	sfSprite* orbSprite;
	sfTexture* orbTexture;

	sfVector2f orbPosition;
	sfVector2f velocity;

	sfBool isGrounded;
}Orbs;

void LoadOrb(void);
void AddOrb(float _x, float _y);
void UpdateOrb( float _dt);
void DrawOrb(sfRenderWindow* _renderWindow);
void CleanupOrb(void);

#endif // !COLLECTIBLES_H
