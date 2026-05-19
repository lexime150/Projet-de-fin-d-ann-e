#include "GameHUD.h"
Player* player;
HUD hud;


void CreateHealthBarHUD();
void CreateKeyNumHUD();


void LoadHUD()
{
	CreateHealthBarHUD();
	CreateKeyNumHUD();
}

void DrawHUD(sfRenderWindow* _renderWindow)
{
	//sfRenderWindow_drawSprite(_renderWindow, hud.healthBarSprite, NULL);
	sfRenderWindow_drawSprite(_renderWindow, hud.healthbarBackgroundSprite, NULL);
	sfRenderWindow_drawSprite(_renderWindow, hud.healthBarSprite, NULL);
	sfRenderWindow_drawSprite(_renderWindow, hud.healthBarContainerSprite, NULL);
	sfRenderWindow_drawSprite(_renderWindow, hud.playerIconSprite, NULL);

	if (GetEnemySpawnTabSize() > 0)
	{
		sfRenderWindow_drawSprite(_renderWindow, hud.keyFragmentSprite, NULL);
		sfRenderWindow_drawText(_renderWindow, hud.keyFragmentText, NULL);

	}
}

void UpdateHUD()
{

	if (player->data.health < 0) player->data.health = 0;
	if (player->data.health > player->data.maxHealth)
		player->data.health = player->data.maxHealth;

	float ratio = (float)player->data.health / (float)player->data.maxHealth;

	if (ratio < 0.f)
	{
		ratio = 0.f;
	}
	if (ratio > 1.f)
	{
		ratio = 1.f;
	}

	sfIntRect rect = {47,9,(int)(80 * ratio),13};

	sfSprite_setTextureRect(hud.healthBarSprite, rect);

	sfSprite_setPosition(hud.healthBarSprite, (sfVector2f) { 49 * GAME_SCALE, 11 * GAME_SCALE });

	if (player->data.health < 0)
	{
		player->data.health = 0;

	}

	if (player->data.health > player->data.maxHealth)
	{
		player->data.health = player->data.maxHealth;

	}

	snprintf(hud.buffer, sizeof(hud.buffer), "%d/%d", player->data.keyNumber, GetMobCount());

	sfText_setString(hud.keyFragmentText, hud.buffer);
}

void CleanupHUD()
{
}

void CreateHealthBarHUD()
{
	hud.healthBarContainerSprite = sfSprite_create();
	hud.healthBarSprite = sfSprite_create();
	hud.healthbarBackgroundSprite = sfSprite_create();
	hud.playerIconSprite = sfSprite_create();

	hud.healthBarContainerTexture = sfTexture_createFromFile("Assets/Sprites/HUD/Container.png", NULL);
	hud.healthBarTexture = sfTexture_createFromFile("Assets/Sprites/HUD/HealthBar.png", NULL);
	hud.healthbarBackgroundTexture = sfTexture_createFromFile("Assets/Sprites/HUD/HealthBar background.png", NULL);
	hud.playerIconTexture = sfTexture_createFromFile("Assets/Sprites/HUD/PlayerIcon.png", NULL);

	sfSprite_setTexture(hud.healthBarContainerSprite, hud.healthBarContainerTexture, sfTrue);
	sfSprite_setTexture(hud.healthBarSprite, hud.healthBarTexture, sfTrue);
	sfSprite_setTexture(hud.healthbarBackgroundSprite, hud.healthbarBackgroundTexture, sfTrue);
	sfSprite_setTexture(hud.playerIconSprite, hud.playerIconTexture, sfTrue);

	sfSprite_setScale(hud.healthBarContainerSprite, (sfVector2f) { GAME_SCALE, GAME_SCALE });
	sfSprite_setScale(hud.healthBarSprite, (sfVector2f) { GAME_SCALE, GAME_SCALE });
	sfSprite_setScale(hud.healthbarBackgroundSprite, (sfVector2f) { GAME_SCALE, GAME_SCALE });
	sfSprite_setScale(hud.playerIconSprite, (sfVector2f) { GAME_SCALE, GAME_SCALE });

	sfSprite_setPosition(hud.healthBarContainerSprite, (sfVector2f) { 2 * GAME_SCALE, 2 * GAME_SCALE });
	sfSprite_setPosition(hud.healthBarSprite, (sfVector2f) { 2 * GAME_SCALE, 2 * GAME_SCALE });
	sfSprite_setPosition(hud.healthbarBackgroundSprite, (sfVector2f) { 2 * GAME_SCALE, 2 * GAME_SCALE });
	sfSprite_setPosition(hud.playerIconSprite, (sfVector2f) { 2 * GAME_SCALE, 2 * GAME_SCALE });

}

void CreateKeyNumHUD()
{

	hud.keyFragmentSprite = sfSprite_create();
	hud.keyFragmentTexture = sfTexture_createFromFile("Assets/Sprites/Game/HUD/Keys.png", NULL);
	sfIntRect keyFragmentRect = { 48,0,16,16 };
	sfSprite_setTexture(hud.keyFragmentSprite, hud.keyFragmentTexture, sfTrue);
	sfSprite_setTextureRect(hud.keyFragmentSprite, keyFragmentRect);
	sfSprite_setScale(hud.keyFragmentSprite, (sfVector2f) { GAME_SCALE * 1.15f, GAME_SCALE * 1.15f });
	sfFloatRect keyFragmentBound = sfSprite_getGlobalBounds(hud.keyFragmentSprite);
	sfSprite_setPosition(hud.keyFragmentSprite, (sfVector2f) { SCREEN_WIDTH - keyFragmentBound.width * 1.2f, 2 * GAME_SCALE });
	keyFragmentBound = sfSprite_getGlobalBounds(hud.keyFragmentSprite);

	hud.keyFragmentText = sfText_create();
	hud.font = sfFont_createFromFile("Assets/Fonts/Arcade.ttf");

	sfText_setFont(hud.keyFragmentText, hud.font);
	sfText_setCharacterSize(hud.keyFragmentText, 28);
	sfText_setString(hud.keyFragmentText, "0/mobCount");
	sfText_getGlobalBounds(hud.keyFragmentText);
	sfFloatRect keyFragmentTextBound = sfText_getGlobalBounds(hud.keyFragmentText);
	sfText_setPosition(hud.keyFragmentText, (sfVector2f) { keyFragmentBound.left - keyFragmentTextBound.width / 2, keyFragmentBound.top + keyFragmentTextBound.height / 2 + 10 });
}
