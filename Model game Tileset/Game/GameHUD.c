#include "GameHUD.h"
Player player;
HUD hud;

void CreateHealthBarHUD();

void LoadHUD()
{
	CreateHealthBarHUD();
}

void DrawHUD(sfRenderWindow* _renderWindow)
{
	sfRenderWindow_drawSprite(_renderWindow, hud.healthBarSprite, NULL);
	sfRenderWindow_drawSprite(_renderWindow, hud.healthBarContainerSprite, NULL);
}

void CleanupHUD()
{
}

void CreateHealthBarHUD()
{
	hud.healthBarContainerSprite = sfSprite_create();
	hud.healthBarSprite = sfSprite_create();

	hud.healthBarTexture = sfTexture_createFromFile("Assets/Sprites/HUD/Health bar.png", NULL);
	sfIntRect healthBarContainerRect = { 0 ,6 ,45 ,7 };
	sfIntRect healthBarRect = { 1 ,0 ,45 , 5 };

	sfSprite_setTexture(hud.healthBarContainerSprite, hud.healthBarTexture, sfTrue);
	sfSprite_setTextureRect(hud.healthBarContainerSprite, healthBarContainerRect);
	sfSprite_setPosition(hud.healthBarContainerSprite, (sfVector2f) { 0, 0 });
	sfSprite_setScale(hud.healthBarContainerSprite, (sfVector2f) {2.5 * GAME_SCALE, 2.5 * GAME_SCALE});


	sfSprite_setTexture(hud.healthBarSprite, hud.healthBarTexture, sfTrue);
	sfSprite_setTextureRect(hud.healthBarSprite, healthBarRect);
	sfSprite_setPosition(hud.healthBarSprite, (sfVector2f) { 10, 10 });
	sfSprite_setScale(hud.healthBarSprite, (sfVector2f) {2.5 * GAME_SCALE, 2.5 * GAME_SCALE});

}
