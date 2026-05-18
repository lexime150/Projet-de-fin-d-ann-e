#ifndef GAMEHUD_H
#define GAMEHUD_H

#include "Common.h"
#include "Player.h"
#include "Mob.h"
typedef struct HUD
{
	sfSprite* healthBarContainerSprite;
	sfSprite* healthBarSprite;
	sfSprite* healthbarBackgroundSprite;
	sfSprite* playerIconSprite;

	sfTexture* healthBarContainerTexture;
	sfTexture* healthBarTexture;
	sfTexture* healthbarBackgroundTexture;
	sfTexture* playerIconTexture;

	sfSprite* keyFragmentSprite;
	sfTexture* keyFragmentTexture;
	sfText* keyFragmentText;
	sfFont* font;

	char buffer[16];

}HUD;

void LoadHUD();
void DrawHUD(sfRenderWindow* _renderWindow);
void UpdateHUD();
void CleanupHUD();


#endif