#include "Player.h"

void CollisionPlayerPlatformsX();
void CollisionPlayerPlatformsY();
void CheckCollisionPlayerPlatforms(float _dt);

void LoadAnimationPlayer(void);

void SetAnimation(PlayerState _state);

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

	LoadAnimationPlayer();

}

void LoadAnimationPlayer(void)
{
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
	firstFrame = (sfIntRect){ 0, PLAYER_HEIGHT * DASH_GROUND , PLAYER_WIDTH, PLAYER_HEIGHT };
	player.animationPlayer[DASH_GROUND] = CreateAnimation(player.sprite, 4, 7, sfTrue, sfTrue, firstFrame);
	firstFrame = (sfIntRect){ 4 * PLAYER_WIDTH, DASH_UP * PLAYER_HEIGHT, PLAYER_WIDTH, PLAYER_HEIGHT };
	player.animationPlayer[DASH_UP] = CreateAnimation(player.sprite, 4, 7, sfTrue, sfTrue, firstFrame);
	firstFrame = (sfIntRect){ 8 * PLAYER_WIDTH, DASH_DIAGONAL * PLAYER_HEIGHT, PLAYER_WIDTH, PLAYER_HEIGHT };
	player.animationPlayer[DASH_DIAGONAL] = CreateAnimation(player.sprite, 4, 7, sfTrue, sfTrue, firstFrame);
	firstFrame = (sfIntRect){ 0, SLIDE * PLAYER_HEIGHT, PLAYER_WIDTH, PLAYER_HEIGHT };
	player.animationPlayer[SLIDE] = CreateAnimation(player.sprite, 4, 7, sfTrue, sfTrue, firstFrame);
	firstFrame = (sfIntRect){ 0, CLIMB_WALL * PLAYER_HEIGHT, PLAYER_WIDTH, PLAYER_HEIGHT };
	player.animationPlayer[CLIMB_WALL] = CreateAnimation(player.sprite, 6, 9, sfTrue, sfTrue, firstFrame);
	firstFrame = (sfIntRect){ 6 * PLAYER_WIDTH, LADDER * PLAYER_HEIGHT, PLAYER_WIDTH, PLAYER_HEIGHT };
	player.animationPlayer[LADDER] = CreateAnimation(player.sprite, 6, 9, sfTrue, sfTrue, firstFrame);
	firstFrame = (sfIntRect){ 0, WALL_GRIP_FALL * PLAYER_HEIGHT, PLAYER_WIDTH, PLAYER_HEIGHT };
	player.animationPlayer[WALL_GRIP_FALL] = CreateAnimation(player.sprite, 2, 4, sfTrue, sfTrue, firstFrame);
	firstFrame = (sfIntRect){ 2 * PLAYER_WIDTH, WALL_JUMP * PLAYER_HEIGHT, PLAYER_WIDTH, PLAYER_HEIGHT };
	player.animationPlayer[WALL_JUMP] = CreateAnimation(player.sprite, 3, 5, sfTrue, sfTrue, firstFrame);



	SetAnimation(JUMP);


}

void SetAnimation(PlayerState _state)
{
	player.currentAnimation = &player.animationPlayer[_state];
	player.currentAnimation->timer = 0.f;
	player.currentAnimation->isPlaying = sfTrue;
	player.currentAnimation->currentFrame = 0;
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

	player.position = sfSprite_getPosition(player.sprite);
}