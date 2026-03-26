#include "Player.h"
Player player;
void CollisionPlayerPlatformsX(float _dx);
void CollisionPlayerPlatformsY(float _dy);
void CheckCollisionPlayerPlatforms(float _dt);

void LoadAnimationPlayer(void);

void StateMachine(PlayerState _state);
void SetAnimation(PlayerState _state);
void ApplyPhysic(float _dt);
void MovePlayer(float _dt);


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
	player.slideVelocityX = 0;
	player.lastDirection = 1;
	player.isSliding = sfFalse;
	player.lastState = IDLE;
	player.isAttacking = sfFalse;

	sfSprite_setOrigin(player.sprite, (sfVector2f) { PLAYER_WIDTH / 2.f, PLAYER_HEIGHT });
	player.isGrounded = sfFalse;
	player.isMoving = sfFalse;

	player.position = sfSprite_getPosition(player.sprite);
	player.collisionRect = sfRectangleShape_getGlobalBounds(player.collisionShape);
	player.playerRect = sfSprite_getGlobalBounds(player.sprite);
	player.slideCooldownTimer = 0.f;

	LoadAnimationPlayer();
}





void UpdatePlayer(float _dt)
{
	ApplyPhysic(_dt);
	MovePlayer(_dt);
	CheckCollisionPlayerPlatforms(_dt);
	UpdateAnimation(player.currentAnimation, _dt);
}


void MovePlayer(float _dt)
{
	sfBool movingLeft = sfKeyboard_isKeyPressed(sfKeyQ);
	sfBool movingRight = sfKeyboard_isKeyPressed(sfKeyD);
	sfBool slideKey = sfKeyboard_isKeyPressed(sfKeyLControl) || sfKeyboard_isKeyPressed(sfKeyRControl);
	sfBool jumpKey = sfKeyboard_isKeyPressed(sfKeySpace);

	static sfBool jumpPressed = sfFalse;

	if (player.slideCooldownTimer > 0.f)
	{
		player.slideCooldownTimer -= _dt;

	}

	if (player.isSlideJumping)
	{
		float sign = (player.slideVelocityX > 0) ? 1.f : -1.f;
		player.slideVelocityX -= sign * SLIDE_FRICTION * _dt;

		if (sign > 0 && player.slideVelocityX < MIN_SLIDE_JUMP_SPEED)
		{
			player.slideVelocityX = MIN_SLIDE_JUMP_SPEED;
		}
		if (sign < 0 && player.slideVelocityX > -MIN_SLIDE_JUMP_SPEED)
		{
			player.slideVelocityX = -MIN_SLIDE_JUMP_SPEED;
		}

		float inputVelocity = 0;
		if (movingRight)
		{
			inputVelocity = player.speed;
		}
		if (movingLeft)
		{
			inputVelocity = -player.speed;
		}

		player.velocity.x = player.slideVelocityX + inputVelocity * 0.4f;

		if (player.isGrounded)
		{
			player.isSlideJumping = sfFalse;
		}
	}


	else if (player.isSliding)
	{
		player.slideTimer -= _dt;

		float sign = (player.lastDirection > 0) ? 1.f : -1.f;
		player.slideVelocityX -= sign * SLIDE_FRICTION * _dt;

		if (sign > 0 && player.slideVelocityX < 0)
		{
			player.slideVelocityX = 0;
		}
		if (sign < 0 && player.slideVelocityX > 0)
		{
			player.slideVelocityX = 0;
		}

		float inputVelocity = 0;
		if (movingRight)
		{
			inputVelocity = player.speed;
			player.lastDirection = 1;
			sfSprite_setScale(player.sprite, (sfVector2f) { GAME_SCALE, GAME_SCALE });
		}
		else if (movingLeft)
		{
			inputVelocity = -player.speed;
			player.lastDirection = -1;
			sfSprite_setScale(player.sprite, (sfVector2f) { -GAME_SCALE, GAME_SCALE });
		}

		player.velocity.x = player.slideVelocityX + inputVelocity * 0.5f;

		if (player.slideTimer <= 0 || !slideKey)
		{
			player.isSliding = sfFalse;
			player.slideVelocityX = 0;
			player.slideCooldownTimer = SLIDE_COOLDOWN;
			StateMachine(player.isMoving ? RUN : IDLE);
		}
	}
	// DEPLACEMENT GAUCHE DROITE
	else
	{
		if (movingRight)
		{
			player.velocity.x = player.speed;
			player.lastDirection = 1;
			sfSprite_setScale(player.sprite, (sfVector2f) { GAME_SCALE, GAME_SCALE });
			player.isMoving = sfTrue;
		}
		else if (movingLeft)
		{
			player.velocity.x = -player.speed;
			player.lastDirection = -1;
			sfSprite_setScale(player.sprite, (sfVector2f) { -GAME_SCALE, GAME_SCALE });
			player.isMoving = sfTrue;
		}
		else
		{
			player.velocity.x = 0;
			player.isMoving = sfFalse;
		}

		// SLIDE
		if (slideKey && player.isGrounded && !player.isSliding && player.slideCooldownTimer <= 0.f)
		{
			player.isSliding = sfTrue;
			player.slideTimer = SLIDE_DURATION;
			player.slideVelocityX = player.lastDirection * SLIDE_SPEED;
			StateMachine(SLIDE);
		}
	}

	// JUMP
	if (jumpKey && !jumpPressed)
	{
		jumpPressed = sfTrue;
		if (player.isGrounded || player.isSliding)
		{
			if (player.isSliding)
			{
				player.isSlideJumping = sfTrue;
				player.slideVelocityX = player.velocity.x;
				player.isSliding = sfFalse;
				player.slideCooldownTimer = SLIDE_COOLDOWN;
			}

			player.velocity.y = -JUMP_FORCE;
			player.isGrounded = sfFalse;
			StateMachine(JUMP);
		}
	}
	if (!jumpKey)
	{
		jumpPressed = sfFalse;
	}




	// ANIMATION
	if (player.isGrounded && !player.isSliding)
	{
		if (player.currentState != TURN)
		{
			if (player.isMoving)
			{
				StateMachine(RUN);
			}
			else
			{
				StateMachine(IDLE);
			}
		}
		if (player.currentState == TURN &&
			player.currentAnimation->currentFrame >= player.currentAnimation->frameCount - 1)
		{
			if (player.isMoving)
			{
				StateMachine(RUN);
			}
			else
			{
				StateMachine(IDLE);

			}
		}
	}
	else if (!player.isGrounded)
	{
		if (player.velocity.y < 0)
		{
			if (player.currentState != JUMP)
			{

				StateMachine(JUMP);
			}
		}
		else
		{
			StateMachine(FALL);
		}
	}
}



void ApplyPhysic(float _dt)
{
	if (!player.isGrounded)
	{
		player.velocity.y += GRAVITY * _dt;
	}
	else
	{
		player.velocity.y = 50.f;
	}
}
void SetAnimation(PlayerState _state)
{
	player.lastState = player.currentState;
	player.currentAnimation = &player.animationPlayer[_state];
	player.currentAnimation->timer = 0.f;
	player.currentAnimation->isPlaying = sfTrue;
	player.currentAnimation->currentFrame = 0;
	player.currentState = _state;

}


void DrawPlayer(sfRenderWindow* _renderWindow)
{
	sfRenderWindow_drawSprite(_renderWindow, player.sprite, NULL);
	//sfRenderWindow_drawRectangleShape(_renderWindow, player.collisionShape, NULL);
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
				printf("côté droit du joueur\n");
			}
			else if (player.velocity.x < 0)
			{

				hitbox.left = platform.left + platform.width;
				printf("côté gauche du joueur\n");
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

void StateMachine(PlayerState _state)
{
	if (player.currentState == _state)
	{
		return;
	}
	SetAnimation(_state);
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

	firstFrame = (sfIntRect){ 0, 7 * PLAYER_HEIGHT, PLAYER_WIDTH, PLAYER_HEIGHT };
	player.animationPlayer[AXE] = CreateAnimation(player.sprite, 10, 15, sfTrue, sfFalse, firstFrame);

	firstFrame = (sfIntRect){ 0, 8 * PLAYER_HEIGHT, 48, PLAYER_HEIGHT };
	player.animationPlayer[SWORD] = CreateAnimation(player.sprite, 4, 18, sfTrue, sfTrue, firstFrame);

	firstFrame = (sfIntRect){ 0, 9 * PLAYER_HEIGHT, PLAYER_WIDTH, PLAYER_HEIGHT };
	player.animationPlayer[DASH] = CreateAnimation(player.sprite, 2, 7, sfTrue, sfFalse, firstFrame);

	SetAnimation(IDLE);

}

