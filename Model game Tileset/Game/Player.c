#include "Player.h"


Player player;

void ApplyPhysic(float _dt)
{
	player.velocity.y += GRAVITY * _dt;
}

void ApplyMovement(float _dt)
{
	sfSprite_move(player.sprite, (sfVector2f) { player.velocity.x * _dt, player.velocity.y* _dt });
}
void MovePlayer(float _dt)
{
	player.velocity.x = 0;

	if (sfKeyboard_isKeyPressed(sfKeyD))
	{
		player.velocity.x = player.speed;
	}
	else if (sfKeyboard_isKeyPressed(sfKeyQ))
	{
		player.velocity.x = -player.speed;
	}
}
void LoadPlayer(void)
{
	player.sprite = sfSprite_create();
	player.texture = sfTexture_createFromFile("Assets/Sprites/Player.png", NULL);
	sfSprite_setTexture(player.sprite, player.texture, sfTrue);
	sfSprite_setScale(player.sprite, (sfVector2f) { GAME_SCALE, GAME_SCALE });
	sfSprite_setPosition(player.sprite, (sfVector2f) { 0, 0 });
	player.speed = 350.f;
	player.velocity.x = 0;
	player.velocity.y = 0;
}

void UpdatePlayer(float _dt)
{
    ApplyPhysic(_dt);
    MovePlayer(_dt);

    sfVector2f pos = sfSprite_getPosition(player.sprite);

    pos.x += player.velocity.x * _dt;
    sfSprite_setPosition(player.sprite, pos);
    player.playerRect = sfSprite_getGlobalBounds(player.sprite);
    CollisionPlayerPlatformsX();

    pos = sfSprite_getPosition(player.sprite);
    pos.y += player.velocity.y * _dt;
    sfSprite_setPosition(player.sprite, pos);
    player.playerRect = sfSprite_getGlobalBounds(player.sprite);
    CollisionPlayerPlatformsY();
}

void DrawPlayer(sfRenderWindow* _renderWindow)
{
	sfRenderWindow_drawSprite(_renderWindow, player.sprite, NULL);
}

void CleanUpPlayer(void)
{
	sfSprite_destroy(player.sprite);
}
