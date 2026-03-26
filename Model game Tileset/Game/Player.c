#include "Player.h"

void CollisionPlayerPlatformsX();
void CollisionPlayerPlatformsY();
void CheckCollisionPlayerPlatforms(float _dt);

void LoadAnimationPlayer(void);


void StateMachine(PlayerState _state);


void SetAnimation(PlayerState _state);

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
	sfSprite_move(player.sprite, (sfVector2f) { player.velocity.x* player.lastDirection* _dt, player.velocity.y* _dt });
}
void MovePlayer(float _dt)
{
	player.velocity.x = 0;
	//player.isMoving = sfFalse;
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
			//player.velocity.x = -player.speed;
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
		sfSprite_setScale(player.sprite, (sfVector2f) { player.lastDirection* GAME_SCALE, GAME_SCALE });
	
	
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
	sfRectangleShape_setFillColor(player.collisionShape, sfColor_fromRGBA(255, 255, 255, 50));


	player.velocity.x = 0;
	player.velocity.y = 0;
	player.lastDirection = 1;
	player.isSliding = sfFalse;
	player.lastState = IDLE;


	sfSprite_setOrigin(player.sprite, (sfVector2f) { PLAYER_WIDTH / 2, PLAYER_HEIGHT });
	player.isGrounded = sfFalse;
	player.isMoving = sfFalse;

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
	ApplyPhysic(_dt);
	MovePlayer(_dt);
	ApplyMovement(_dt);
	CheckCollisionPlayerPlatforms(_dt);
	UpdateAnimation(player.currentAnimation, _dt);

	if (player.velocity.y < 0)
	{
		StateMachine(JUMP);
	}
	if (player.velocity.y > 0)
	{
		StateMachine(FALL);
	}
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
	case RUN :
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
	default :
		break;
	}

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
				player.playerRect.top = platformRect.top + platformRect.height;

			}

			
			 player.velocity.y = 0;

			sfRectangleShape_setPosition(player.collisionShape, pos);
			player.collisionRect = sfRectangleShape_getGlobalBounds(player.collisionShape);

			sfSprite_setPosition(player.sprite, (sfVector2f) { player.playerRect.left, player.playerRect.top });
			
				
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

			sfSprite_setPosition(player.sprite, (sfVector2f) { player.playerRect.left, player.playerRect.top });

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
	sfSprite_setPosition(player.sprite, (sfVector2f) { hitboxRect.left + hitboxRect.width / 2, hitboxRect.top + hitboxRect.height });
	player.position = sfSprite_getPosition(player.sprite);


	
}