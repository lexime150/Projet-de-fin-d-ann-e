#include "Player.h"
Player player;
void CollisionPlayerPlatformsX(float _dx);
void CollisionPlayerPlatformsY(float _dy);
void CheckCollisionPlayerPlatforms(float _dt);

void LoadAnimationPlayer(void);

void StateMachine(PlayerState _state);
void SetAnimation(PlayerState _state);
void ApplyPhysic(float _dt);

void ApplyPhysic(float _dt)
{
	if (!player.isGrounded)
	{
		player.velocity.y += GRAVITY * _dt;
	}
}


void MovePlayer(float _dt)
{
	player.velocity.x = 0;
	player.isSliding = sfFalse;

	if (sfKeyboard_isKeyPressed(sfKeyD))
	{
		if (player.lastDirection != 1)
		{
			player.lastDirection = 1;
			StateMachine(TURN);
		}
		if (player.velocity.x == 0.f)
		{
			player.velocity.x = player.speed;

		}

		player.isMoving = sfTrue;

		if (player.isGrounded && sfKeyboard_isKeyPressed(sfKeyLControl))
		{
			player.isSliding = sfTrue;
			player.velocity.x = player.speed * 5.f;
			StateMachine(SLIDE);
		}

		if (player.isGrounded && !player.isSliding)
		{

			StateMachine(RUN);
		}
	}
	if (sfKeyboard_isKeyPressed(sfKeyQ))
	{
		player.lastDirection = -1;
		player.isMoving = sfTrue;

		if (player.velocity.x == 0.f)
		{
			player.velocity.x = -player.speed;

		}

		if (player.isGrounded && sfKeyboard_isKeyPressed(sfKeyLControl))
		{
			player.isSliding = sfTrue;
			player.velocity.x = -player.speed * 5.f;
			StateMachine(SLIDE);
		}

		if (player.isGrounded && !player.isSliding)
		{

			StateMachine(RUN);
		}
	}
	if (sfKeyboard_isKeyPressed(sfKeySpace) && player.isGrounded)
	{
		player.velocity.y = -500;
	}
	sfSprite_setScale(player.sprite, (sfVector2f) { (float)player.lastDirection* GAME_SCALE, GAME_SCALE });

	if (!sfKeyboard_isKeyPressed(sfKeyD) && !sfKeyboard_isKeyPressed(sfKeyQ) && !sfKeyboard_isKeyPressed(sfKeySpace) && player.currentState != IDLE && player.isGrounded)
	{
		StateMachine(IDLE);

	}

	if (player.lastState == SLIDE && player.currentState == RUN)
	{
		if (player.velocity.x > player.speed)
		{
			player.velocity.x -= 0.02f;

		}
	}
	if (player.velocity.y < 0)
	{
		StateMachine(JUMP);

	}
	if (player.velocity.y > 0)
	{
		StateMachine(FALL);
	}
	ApplyPhysic(_dt);
	CheckCollisionPlayerPlatforms(_dt);
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
	sfRectangleShape_setPosition(player.collisionShape, (sfVector2f) { spritePos.x - (PLAYER_WIDTH * GAME_SCALE) / 2.f, spritePos.y - (PLAYER_HEIGHT * GAME_SCALE) });
	sfRectangleShape_setOutlineColor(player.collisionShape, sfRed);
	sfRectangleShape_setFillColor(player.collisionShape, sfColor_fromRGBA(255, 255, 255, 50));

	player.velocity.x = 0;
	player.velocity.y = 0;
	player.lastDirection = 1;
	player.isSliding = sfFalse;
	player.lastState = IDLE;

	sfSprite_setOrigin(player.sprite, (sfVector2f) { PLAYER_WIDTH / 2.f, PLAYER_HEIGHT });
	player.isGrounded = sfFalse;
	player.isMoving = sfFalse;

	player.position = sfSprite_getPosition(player.sprite);
	player.collisionRect = sfRectangleShape_getGlobalBounds(player.collisionShape);
	player.playerRect = sfSprite_getGlobalBounds(player.sprite);

	LoadAnimationPlayer();
}

void LoadAnimationPlayer(void)
{
	sfIntRect firstFrame = { 0, 0 * PLAYER_HEIGHT , PLAYER_WIDTH, PLAYER_HEIGHT };
	player.animationPlayer[IDLE] = CreateAnimation(player.sprite, 5, 7, sfTrue, sfTrue, firstFrame);

	firstFrame = (sfIntRect){ 0, 1 * PLAYER_HEIGHT, PLAYER_WIDTH, PLAYER_HEIGHT };
	player.animationPlayer[RUN] = CreateAnimation(player.sprite, 6, 9, sfTrue, sfTrue, firstFrame);

	firstFrame = (sfIntRect){ 0, 1 * PLAYER_HEIGHT, PLAYER_WIDTH, PLAYER_HEIGHT };
	player.animationPlayer[TURN] = CreateAnimation(player.sprite, 4, 7, sfTrue, sfTrue, firstFrame);

	firstFrame = (sfIntRect){ 0, 2 * PLAYER_HEIGHT, PLAYER_WIDTH, PLAYER_HEIGHT };
	player.animationPlayer[JUMP] = CreateAnimation(player.sprite, 3, 6, sfTrue, sfTrue, firstFrame);

	firstFrame = (sfIntRect){ 3 * PLAYER_WIDTH, 2 * PLAYER_HEIGHT, PLAYER_WIDTH, PLAYER_HEIGHT };
	player.animationPlayer[FALL] = CreateAnimation(player.sprite, 3, 6, sfTrue, sfTrue, firstFrame);

	firstFrame = (sfIntRect){ 0, 2 * PLAYER_HEIGHT, PLAYER_WIDTH, PLAYER_HEIGHT };
	player.animationPlayer[D_JUMP] = CreateAnimation(player.sprite, 3, 6, sfTrue, sfTrue, firstFrame);

	firstFrame = (sfIntRect){ 0, PLAYER_HEIGHT * 3 , PLAYER_WIDTH, PLAYER_HEIGHT };
	player.animationPlayer[DASH_GROUND] = CreateAnimation(player.sprite, 4, 7, sfTrue, sfTrue, firstFrame);

	firstFrame = (sfIntRect){ 4 * PLAYER_WIDTH, 3 * PLAYER_HEIGHT, PLAYER_WIDTH, PLAYER_HEIGHT };
	player.animationPlayer[DASH_UP] = CreateAnimation(player.sprite, 4, 7, sfTrue, sfTrue, firstFrame);

	firstFrame = (sfIntRect){ 8 * PLAYER_WIDTH, 3 * PLAYER_HEIGHT, PLAYER_WIDTH, PLAYER_HEIGHT };
	player.animationPlayer[DASH_DIAGONAL] = CreateAnimation(player.sprite, 4, 7, sfTrue, sfTrue, firstFrame);

	firstFrame = (sfIntRect){ 0, 4 * PLAYER_HEIGHT, PLAYER_WIDTH, PLAYER_HEIGHT };
	player.animationPlayer[SLIDE] = CreateAnimation(player.sprite, 4, 7, sfTrue, sfTrue, firstFrame);

	firstFrame = (sfIntRect){ 0, 5 * PLAYER_HEIGHT, PLAYER_WIDTH, PLAYER_HEIGHT };
	player.animationPlayer[CLIMB_WALL] = CreateAnimation(player.sprite, 6, 9, sfTrue, sfTrue, firstFrame);

	firstFrame = (sfIntRect){ 6 * PLAYER_WIDTH, 5 * PLAYER_HEIGHT, PLAYER_WIDTH, PLAYER_HEIGHT };
	player.animationPlayer[LADDER] = CreateAnimation(player.sprite, 6, 7, sfTrue, sfTrue, firstFrame);

	firstFrame = (sfIntRect){ 0, 6 * PLAYER_HEIGHT, PLAYER_WIDTH, PLAYER_HEIGHT };
	player.animationPlayer[WALL_GRIP_FALL] = CreateAnimation(player.sprite, 2, 4, sfTrue, sfTrue, firstFrame);

	firstFrame = (sfIntRect){ 2 * PLAYER_WIDTH, 6 * PLAYER_HEIGHT, PLAYER_WIDTH, PLAYER_HEIGHT };
	player.animationPlayer[WALL_JUMP] = CreateAnimation(player.sprite, 3, 6, sfTrue, sfTrue, firstFrame);

	SetAnimation(IDLE);
}

void SetAnimation(PlayerState _state)
{
	player.lastState = player.currentState;
	printf("%d\n", player.currentState);
	player.currentAnimation = &player.animationPlayer[_state];
	player.currentAnimation->timer = 0.f;
	player.currentAnimation->isPlaying = sfTrue;
	player.currentAnimation->currentFrame = 0;
	player.currentState = _state;
	printf("%d\n", player.currentState);
}

void UpdatePlayer(float _dt)
{
	MovePlayer(_dt);
	UpdateAnimation(player.currentAnimation, _dt);


}

void StateMachine(PlayerState _state)
{
	switch (_state)
	{
	case IDLE:
		if (player.currentState != _state)
		{
			player.currentState = _state;
			SetAnimation(IDLE);
		}
		break;
	case RUN:
		if (player.currentState != _state)
		{
			player.currentState = _state;
			SetAnimation(RUN);
		}
		break;
	case JUMP:
		if (player.currentState != _state)
		{
			player.currentState = _state;
			SetAnimation(JUMP);
		}
		break;
	case FALL:
		if (player.currentState != _state)
		{
			player.currentState = _state;
			SetAnimation(FALL);
		}
		break;
	case TURN:
		if (player.currentState != _state)
		{
			player.currentState = _state;
			SetAnimation(TURN);
		}
		break;
	case SLIDE:
		if (player.currentState != _state)
		{
			player.currentState = _state;
			SetAnimation(SLIDE);
		}
		break;
	default:
		break;
	}
}

void DrawPlayer(sfRenderWindow* _renderWindow)
{
	sfRenderWindow_drawSprite(_renderWindow, player.sprite, NULL);
}

void CleanUpPlayer(void)
{
	sfSprite_destroy(player.sprite);
	sfTexture_destroy(player.texture);
	sfRectangleShape_destroy(player.collisionShape);
}

void CollisionPlayerPlatformsX(float _dx)
{
	float playerHalfWidth = (PLAYER_WIDTH * GAME_SCALE) / 2.f;
	float playerWidth = PLAYER_WIDTH * GAME_SCALE;
	float playerHeight = PLAYER_HEIGHT * GAME_SCALE;

	sfFloatRect hitbox = { player.position.x - playerHalfWidth + _dx, player.position.y - playerHeight, playerWidth, playerHeight };

	for (unsigned i = 0; i < GetCollisionTabSize(); i++)
	{
		sfFloatRect platform = GetMapCollision(i);
		if (sfFloatRect_intersects(&hitbox, &platform, NULL))
		{
			if (player.velocity.x > 0)
			{
				hitbox.left = platform.left - hitbox.width;

			}
			else if (player.velocity.x < 0)
			{

				hitbox.left = platform.left + platform.width;
			}

			player.velocity.x = 0;
			player.position.x = hitbox.left + hitbox.width / 2.f;
			sfSprite_setPosition(player.sprite, player.position);
			sfRectangleShape_setPosition(player.collisionShape, (sfVector2f) { hitbox.left, hitbox.top });
			player.collisionRect = sfRectangleShape_getGlobalBounds(player.collisionShape);
			player.playerRect = sfSprite_getGlobalBounds(player.sprite);
			return;
		}
	}

	player.position.x += _dx;
	sfSprite_setPosition(player.sprite, player.position);
	sfRectangleShape_setPosition(player.collisionShape, (sfVector2f) { player.position.x - playerHalfWidth, player.position.y - playerHeight });
	player.collisionRect = sfRectangleShape_getGlobalBounds(player.collisionShape);
	player.playerRect = sfSprite_getGlobalBounds(player.sprite);
}

void CollisionPlayerPlatformsY(float _dy)
{
	float playerHalfWidth = (PLAYER_WIDTH * GAME_SCALE) / 2.f;
	float playerWidth = PLAYER_WIDTH * GAME_SCALE;
	float playerHeight = PLAYER_HEIGHT * GAME_SCALE;

	sfFloatRect hitbox = { player.position.x - playerHalfWidth, player.position.y - playerHeight + _dy, playerWidth, playerHeight };
	player.isGrounded = sfFalse;

	for (unsigned i = 0; i < GetCollisionTabSize(); i++)
	{
		sfFloatRect platform = GetMapCollision(i);
		if (sfFloatRect_intersects(&hitbox, &platform, NULL))
		{
			if (player.velocity.y > 0)
			{
				hitbox.top = platform.top - hitbox.height;
				player.isGrounded = sfTrue;
			}
			else if (player.velocity.y < 0)
			{
				hitbox.top = platform.top + platform.height;
			}

			player.velocity.y = 0;
			player.position.y = hitbox.top + hitbox.height;
			sfSprite_setPosition(player.sprite, player.position);
			sfRectangleShape_setPosition(player.collisionShape, (sfVector2f) { hitbox.left, hitbox.top });
			player.collisionRect = sfRectangleShape_getGlobalBounds(player.collisionShape);
			player.playerRect = sfSprite_getGlobalBounds(player.sprite);
			return;
		}
	}

	player.position.y += _dy;
	sfSprite_setPosition(player.sprite, player.position);
	sfRectangleShape_setPosition(player.collisionShape, (sfVector2f) { player.position.x - playerHalfWidth, player.position.y - playerHeight });
	player.collisionRect = sfRectangleShape_getGlobalBounds(player.collisionShape);
	player.playerRect = sfSprite_getGlobalBounds(player.sprite);
}

void CheckCollisionPlayerPlatforms(float _dt)
{
	float dx = player.velocity.x * _dt;
	CollisionPlayerPlatformsX(dx);

	float dy = player.velocity.y * _dt;
	CollisionPlayerPlatformsY(dy);

	sfRectangleShape_setPosition(player.collisionShape, (sfVector2f) { player.position.x - (PLAYER_WIDTH * GAME_SCALE) / 2.f, player.position.y - (PLAYER_HEIGHT * GAME_SCALE) });
	player.collisionRect = sfRectangleShape_getGlobalBounds(player.collisionShape);
	player.playerRect = sfSprite_getGlobalBounds(player.sprite);
}