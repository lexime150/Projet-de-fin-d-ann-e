#ifndef GAMEHUD_H
#define GAMEHUD_H

#include "Common.h"
#include "Player.h"
typedef struct HUD
{
	sfSprite* healthBarContainerSprite;
	sfSprite* healthBarSprite;

	sfTexture* healthBarTexture;


}HUD;

void LoadHUD();
void DrawHUD(sfRenderWindow* _renderWindow);
void UpdateHUD();
void CleanupHUD();


#endif