#include "Player.h"

void CollisionPlayerPlatformsX();
void CollisionPlayerPlatformsY();
void CheckCollisionPlayerPlatforms(float _dt);
Player player;

void ApplyPhysic(float _dt)
{
	player.velocity.y += GRAVITY * _dt;
}

void ApplyMovement(float _dt)
{
	sfSprite_move(player.sprite, (sfVector2f) { player.velocity.x* _dt, player.velocity.y* _dt });
}
void MovePlayer(float _dt)
{
	player.velocity.x = 0;

	if (sfKeyboard_isKeyPressed(sfKeyD))
	{
		player.velocity.x = player.speed;
	}
	if (sfKeyboard_isKeyPressed(sfKeyQ))
	{
		player.velocity.x = -player.speed;
	}
	if (sfKeyboard_isKeyPressed(sfKeySpace) && player.isGrounded)
	{
		player.velocity.y = -500;
	}

}
void LoadPlayer(void)
{
	player.sprite = sfSprite_create();
	player.texture = sfTexture_createFromFile("Assets/Sprites/IDLE.png", NULL);
	sfSprite_setTexture(player.sprite, player.texture, sfTrue);
	sfSprite_setScale(player.sprite, (sfVector2f) { GAME_SCALE, GAME_SCALE });
	sfSprite_setPosition(player.sprite, (sfVector2f) { 0, 0 });
	player.speed = 350.f;

	player.velocity.x = 0;
	player.velocity.y = 0;

	player.isGrounded = sfFalse;
	sfIntRect firstFrame = { 0,IDLE * PLAYER_HEIGHT , PLAYER_WIDTH, PLAYER_HEIGHT };
	player.animationPlayer[IDLE] = CreateAnimation(player.sprite, 5, 8, sfTrue, sfTrue, firstFrame);
	firstFrame = (sfIntRect){ 0, RUN * PLAYER_HEIGHT, PLAYER_WIDTH, PLAYER_HEIGHT };
	player.animationPlayer[RUN] = CreateAnimation(player.sprite, 6, 10, sfTrue, sfTrue, firstFrame);
	firstFrame = (sfIntRect){6 * PLAYER_WIDTH, TURN * PLAYER_HEIGHT, PLAYER_WIDTH, PLAYER_HEIGHT};
	player.animationPlayer[TURN] = CreateAnimation(player.sprite, 4, 6, sfTrue, sfTrue, firstFrame);
	firstFrame = (sfIntRect){0, JUMP * PLAYER_HEIGHT, PLAYER_WIDTH, PLAYER_HEIGHT};
	player.animationPlayer[JUMP] = CreateAnimation(player.sprite, 3, 6, sfTrue, sfTrue, firstFrame);
	firstFrame = (sfIntRect){3 * PLAYER_WIDTH, FALL * PLAYER_HEIGHT, PLAYER_WIDTH, PLAYER_HEIGHT};
	player.animationPlayer[FALL] = CreateAnimation(player.sprite, 3, 6, sfTrue, sfTrue, firstFrame);
	firstFrame = (sfIntRect){6 * PLAYER_WIDTH, D_JUMP * PLAYER_HEIGHT, PLAYER_WIDTH, PLAYER_HEIGHT};
	player.animationPlayer[D_JUMP] = CreateAnimation(player.sprite, 3, 6, sfTrue, sfTrue, firstFrame);


	player.currentAnimation = &player.animationPlayer;
}

void UpdatePlayer(float _dt)
{
	ApplyPhysic(_dt);
	MovePlayer(_dt);
	CheckCollisionPlayerPlatforms(_dt);
	UpdateAnimation(player.currentAnimation, _dt);
}

void DrawPlayer(sfRenderWindow* _renderWindow)
{
	sfRenderWindow_drawSprite(_renderWindow, player.sprite, NULL);
}

void CleanUpPlayer(void)
{
	sfSprite_destroy(player.sprite);
}

void CollisionPlayerPlatformsY()
{
	player.isGrounded = sfFalse;
	for (unsigned i = 0; i < GetCollisionTabSize(); i++)
	{
		sfFloatRect platformRect = GetMapCollision(i);

		if (sfFloatRect_intersects(&player.playerRect, &platformRect, NULL))
		{
			if (player.velocity.y > 0)
			{
				player.playerRect.top = platformRect.top - player.playerRect.height;
				player.isGrounded = sfTrue;
			}
			else if (player.velocity.y < 0)
			{
				player.playerRect.top = platformRect.top + platformRect.height;
			}

			player.velocity.y = 0;

			sfSprite_setPosition(player.sprite, (sfVector2f) { player.playerRect.left, player.playerRect.top });
		}
	}
}
void CollisionPlayerPlatformsX()
{
	for (unsigned i = 0; i < GetCollisionTabSize(); i++)
	{
		sfFloatRect platformRect = GetMapCollision(i);

		if (sfFloatRect_intersects(&player.playerRect, &platformRect, NULL))
		{
			if (player.velocity.x > 0)
			{
				player.playerRect.left = platformRect.left - player.playerRect.width;
			}
			else if (player.velocity.x < 0)
			{
				player.playerRect.left = platformRect.left + platformRect.width;
			}

			player.velocity.x = 0;

			sfSprite_setPosition(player.sprite, (sfVector2f) { player.playerRect.left, player.playerRect.top });
		}
	}
}
void CheckCollisionPlayerPlatforms(float _dt)
{

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