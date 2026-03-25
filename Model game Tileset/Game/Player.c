#include "Player.h"

void CollisionPlayerPlatformsX();
void CollisionPlayerPlatformsY();
void CheckCollisionPlayerPlatforms(float _dt);
Player player;

void ApplyPhysic(float _dt)
{
	if (!player.isGrounded)
	{
		player.velocity.y += GRAVITY * _dt;

	}
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
		player.lastDirection = 1;
	}
	if (sfKeyboard_isKeyPressed(sfKeyQ))
	{
		player.velocity.x = -player.speed;
		player.lastDirection = -1;
	}
	if (sfKeyboard_isKeyPressed(sfKeySpace) && player.isGrounded)
	{
		player.velocity.y = -500;
	}
	sfSprite_setScale(player.sprite, (sfVector2f) {player.lastDirection * GAME_SCALE, GAME_SCALE});
}
void LoadPlayer(void)
{
	player.sprite = sfSprite_create();
	player.texture = sfTexture_createFromFile("Assets/Sprites/IDLE.png", NULL);
	sfSprite_setTexture(player.sprite, player.texture, sfTrue);
	sfSprite_setScale(player.sprite, (sfVector2f) { GAME_SCALE, GAME_SCALE });
	sfSprite_setPosition(player.sprite, (sfVector2f) { 100, 0 });
	player.speed = 350.f;

	player.collisionShape = sfRectangleShape_create();
	sfRectangleShape_setSize(player.collisionShape, (sfVector2f) { PLAYER_WIDTH* GAME_SCALE, PLAYER_HEIGHT* GAME_SCALE });

	sfVector2f spritePos = sfSprite_getPosition(player.sprite);

	sfRectangleShape_setPosition(player.collisionShape, (sfVector2f) { spritePos.x - (PLAYER_WIDTH * GAME_SCALE) / 2, spritePos.y - (PLAYER_HEIGHT * GAME_SCALE) });
	sfRectangleShape_setOutlineColor(player.collisionShape, sfRed);
	sfRectangleShape_setFillColor(player.collisionShape, sfColor_fromRGBA(255,255,255, 50));

	player.velocity.x = 0;
	player.velocity.y = 0;
	player.lastDirection = 1;


	sfSprite_setOrigin(player.sprite, (sfVector2f) {PLAYER_WIDTH /2, PLAYER_HEIGHT});
	player.isGrounded = sfFalse;
	sfIntRect firstFrame = { 0,IDLE * PLAYER_HEIGHT , PLAYER_WIDTH, PLAYER_HEIGHT };
	player.animationPlayer[IDLE] = CreateAnimation(player.sprite, 5, 8, sfTrue, sfTrue, firstFrame);
	firstFrame = (sfIntRect){ 0, RUN * PLAYER_HEIGHT, PLAYER_WIDTH, PLAYER_HEIGHT };
	player.animationPlayer[RUN] = CreateAnimation(player.sprite, 6, 10, sfTrue, sfTrue, firstFrame);
	firstFrame = (sfIntRect){ 6 * PLAYER_WIDTH, TURN * PLAYER_HEIGHT, PLAYER_WIDTH, PLAYER_HEIGHT };
	player.animationPlayer[TURN] = CreateAnimation(player.sprite, 4, 6, sfTrue, sfTrue, firstFrame);
	firstFrame = (sfIntRect){ 0, JUMP * PLAYER_HEIGHT, PLAYER_WIDTH, PLAYER_HEIGHT };
	player.animationPlayer[JUMP] = CreateAnimation(player.sprite, 3, 6, sfTrue, sfTrue, firstFrame);
	firstFrame = (sfIntRect){ 3 * PLAYER_WIDTH, FALL * PLAYER_HEIGHT, PLAYER_WIDTH, PLAYER_HEIGHT };
	player.animationPlayer[FALL] = CreateAnimation(player.sprite, 3, 6, sfTrue, sfTrue, firstFrame);
	firstFrame = (sfIntRect){ 6 * PLAYER_WIDTH, D_JUMP * PLAYER_HEIGHT, PLAYER_WIDTH, PLAYER_HEIGHT };
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
	sfRenderWindow_drawRectangleShape(_renderWindow, player.collisionShape, NULL);
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

		if (sfFloatRect_intersects(&player.collisionRect, &platformRect, NULL))
		{
			sfVector2f pos = sfRectangleShape_getPosition(player.collisionShape);

			if (player.velocity.y > 0)
			{
				pos.y = platformRect.top - player.collisionRect.height;
				player.isGrounded = sfTrue;
			}
			else if (player.velocity.y < 0)
			{
				pos.y = platformRect.top + platformRect.height;
			}

			player.velocity.y = 0;

			sfRectangleShape_setPosition(player.collisionShape, pos);
			player.collisionRect = sfRectangleShape_getGlobalBounds(player.collisionShape);
		}
	}
}
void CollisionPlayerPlatformsX()
{
	for (unsigned i = 0; i < GetCollisionTabSize(); i++)
	{
		sfFloatRect platformRect = GetMapCollision(i);

		if (sfFloatRect_intersects(&player.collisionRect, &platformRect, NULL))
		{
			sfVector2f pos = sfRectangleShape_getPosition(player.collisionShape);

			if (player.velocity.x > 0)
			{
				pos.x = platformRect.left - player.collisionRect.width;
			}
			else if (player.velocity.x < 0)
			{
				pos.x = platformRect.left + platformRect.width;
			}

			player.velocity.x = 0;

			sfRectangleShape_setPosition(player.collisionShape, pos);
			player.collisionRect = sfRectangleShape_getGlobalBounds(player.collisionShape);
		}
	}
}
void CheckCollisionPlayerPlatforms(float _dt)
{
	sfVector2f pos = sfRectangleShape_getPosition(player.collisionShape);

	pos.x += player.velocity.x * _dt;
	sfRectangleShape_setPosition(player.collisionShape, pos);

	player.collisionRect = sfRectangleShape_getGlobalBounds(player.collisionShape);
	CollisionPlayerPlatformsX();

	pos = sfRectangleShape_getPosition(player.collisionShape);
	pos.y += player.velocity.y * _dt;

	sfRectangleShape_setPosition(player.collisionShape, pos);
	player.collisionRect = sfRectangleShape_getGlobalBounds(player.collisionShape);

	CollisionPlayerPlatformsY();
	sfVector2f hitboxPos = sfRectangleShape_getPosition(player.collisionShape);
	sfFloatRect hitboxRect = sfRectangleShape_getGlobalBounds(player.collisionShape);
	sfSprite_setPosition(player.sprite, (sfVector2f) { hitboxRect.left + hitboxRect.width/2, hitboxRect.top + hitboxRect.height });
	player.position = sfSprite_getPosition(player.sprite);
}