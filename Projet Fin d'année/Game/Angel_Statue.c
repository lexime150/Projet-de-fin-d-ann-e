#include "Angel_Statue.h"
Player* player;
AngelStatue angelStatue;
void LoadAngelStatue(void)
{
	if (strcmp(player->data.level, "Level_04") == 0)
	{
		angelStatue.sprite = sfSprite_create();
		sfTexture* texture = sfTexture_createFromFile("Assets/Sprites/Game/Props/Angel Statue.png", NULL);
		sfSprite_setTexture(angelStatue.sprite, texture, sfTrue);
		sfSprite_setOrigin(angelStatue.sprite, (sfVector2f) { 32, 64 });
		sfSprite_setScale(angelStatue.sprite, (sfVector2f) { GAME_SCALE, GAME_SCALE });
		printf("%f, %f\n", GetPlayerSpawn().x, GetPlayerSpawn().y);
		sfSprite_setPosition(angelStatue.sprite, (sfVector2f) {450 ,384});
	}
}


void UpdateAngelStatue(float _dt)
{
	if (angelStatue.sprite)
	{

	}
}

void DrawAngelStatue(sfRenderWindow* _renderWindow)
{
	if (strcmp(player->data.level, "Level_04") == 0)
	{
		if (angelStatue.sprite)
		{
			sfRenderWindow_drawSprite(_renderWindow, angelStatue.sprite, NULL);
		}
	}
}

void CleanupAngelStatue(void)
{
	if (angelStatue.sprite)
	{
		sfSprite_destroy(angelStatue.sprite);
		angelStatue.sprite = NULL;
	}
}
