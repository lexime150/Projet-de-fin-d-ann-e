#include "Player.h"

Player player;
PlayerSaveData save;
sfBool keyWasPressed = sfFalse;
void LoadAnimationPlayer(void);
void StateMachine(PlayerState _state);
void SetAnimation(PlayerState _state);
void createCollisionAttack();
void UpdatePlayer(float _dt);
void ApplyPhysic(float _dt);
void MovePlayer(float _dt);
void CollisionPlayerPlatformsX(float _dx);
void CollisionPlayerPlatformsY(float _dy);
sfBool CheckCollisionPlayerPlatformsX(float _dx);
void CheckCollisionPlayerPlatforms(float _dt);

void basePlayer();
void setSavedStat(PlayerSaveData* save);

void CollisionPlayerTrigger();


void LoadPlayer(PlayerSaveData* save)
{
	basePlayer();
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
	player.animationPlayer[WALL_JUMP] = CreateAnimation(player.sprite, 3, 9, sfTrue, sfFalse, firstFrame);

	firstFrame = (sfIntRect){ 0, 7 * PLAYER_HEIGHT, 48, PLAYER_HEIGHT };
	player.animationPlayer[AXE] = CreateAnimation(player.sprite, 10, 15, sfTrue, sfFalse, firstFrame);

	firstFrame = (sfIntRect){ 0, 8 * PLAYER_HEIGHT, 48, PLAYER_HEIGHT };
	player.animationPlayer[SWORD] = CreateAnimation(player.sprite, 4, 18, sfTrue, sfFalse, firstFrame);

	firstFrame = (sfIntRect){ 0, 9 * PLAYER_HEIGHT, PLAYER_WIDTH, PLAYER_HEIGHT };
	player.animationPlayer[DASH] = CreateAnimation(player.sprite, 2, 7, sfTrue, sfFalse, firstFrame);

	SetAnimation(IDLE);
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

void UpdatePlayer(float _dt)
{
	ApplyPhysic(_dt);
	MovePlayer(_dt);
	CheckCollisionPlayerPlatforms(_dt);
	CollisionPlayerTrigger();
	UpdateAnimation(player.currentAnimation, _dt);
}
void ApplyPhysic(float _dt)
{
	if (!player.action.isGrounded)
	{
		player.data.velocity.y += GRAVITY * _dt;
		if (player.data.velocity.y >= GRAVITY * 100 * _dt)
		{
			player.data.velocity.y = GRAVITY * 100 * _dt;
		}
	}
	else
	{
		player.data.velocity.y = 50.f;
	}
}
void createCollisionAttack()
{
	player.shape.collisionAttackShape = sfRectangleShape_create();
	sfRectangleShape_setSize(player.shape.collisionAttackShape, (sfVector2f) { ATTACK_HITBOX_WIDTH* GAME_SCALE, ATTACK_HITBOX_HEIGHT* GAME_SCALE });
	sfRectangleShape_setFillColor(player.shape.collisionAttackShape, sfColor_fromRGBA(255, 0, 0, 150));
	player.shape.collisionAttackRect = sfRectangleShape_getGlobalBounds(player.shape.collisionAttackShape);
	if (player.data.lastDirection == -1)
	{
		sfRectangleShape_setPosition(player.shape.collisionAttackShape, (sfVector2f) { player.shape.collisionPlayerRect.left - player.shape.collisionAttackRect.width, player.shape.collisionPlayerRect.top + player.shape.collisionPlayerRect.width / 2 - 10 });
	}
	else if (player.data.lastDirection == 1)
	{
		sfRectangleShape_setPosition(player.shape.collisionAttackShape, (sfVector2f) { player.shape.collisionPlayerRect.left + player.shape.collisionPlayerRect.width, player.shape.collisionPlayerRect.top + player.shape.collisionPlayerRect.width / 2 - 10 });

	}

}
void MovePlayer(float _dt)
{
	sfBool movingLeft = sfKeyboard_isKeyPressed(sfKeyQ);
	sfBool movingRight = sfKeyboard_isKeyPressed(sfKeyD);
	sfBool slideKey = sfKeyboard_isKeyPressed(sfKeyLControl) || sfKeyboard_isKeyPressed(sfKeyRControl);
	sfBool jumpKey = sfKeyboard_isKeyPressed(sfKeySpace);

	static sfBool jumpPressed = sfFalse;

	player.data.attackCooldownTimer += _dt;

	if (player.data.slideCooldownTimer > 0.f)
	{
		player.data.slideCooldownTimer -= _dt;
	}

	if (sfMouse_isButtonPressed(sfMouseLeft) && player.data.attackCooldownTimer >= ATTACK_AXE_COOLDOWN && player.action.isGrounded)
	{
		createCollisionAttack();
		player.action.isAttacking = sfTrue;
		player.data.attackCooldownTimer = 0.f;
		player.data.velocity.x = 0;
		StateMachine(AXE);
	}
	if (sfMouse_isButtonPressed(sfMouseRight) && player.data.attackCooldownTimer >= ATTACK_SWORD_COOLDOWN && player.action.isGrounded)
	{
		createCollisionAttack();
		player.action.isAttacking = sfTrue;
		player.data.attackCooldownTimer = 0.f;
		sfSound_play(player.sound.swordSound);
		StateMachine(SWORD);
	}
	if (player.action.isAttacking && player.currentState == AXE)
	{
		player.data.velocity.x = 0;
		if (player.currentAnimation->currentFrame >= player.currentAnimation->frameCount - 1)
		{
			sfRectangleShape_destroy(player.shape.collisionAttackShape);
			player.action.isAttacking = sfFalse;

			if (player.action.isGrounded)
			{
				StateMachine(player.action.isMoving ? RUN : IDLE);
			}
			else
			{
				StateMachine(FALL);
			}
		}
		return;
	}
	if (player.action.isAttacking && player.currentState == SWORD)
	{
		player.data.velocity.x = 0;
		if (player.currentAnimation->currentFrame >= player.currentAnimation->frameCount - 1)
		{
			sfRectangleShape_destroy(player.shape.collisionAttackShape);

			player.action.isAttacking = sfFalse;
			if (player.action.isGrounded)
			{
				StateMachine(player.action.isMoving ? RUN : IDLE);
			}
			else
			{
				StateMachine(FALL);
			}
		}
		return;
	}


	if (player.action.isWallJumping)
	{
		float sign = (player.data.wallJumpVelocityX > 0) ? 1.f : -1.f;
		player.data.wallJumpVelocityX -= (player.data.wallJumpVelocityX > 0 ? 1.f : -1.f) * WALL_JUMP_FRICTION * _dt;
		if (sign > 0 && player.data.wallJumpVelocityX < MIN_WALL_JUMP_SPEED)
		{
			player.data.wallJumpVelocityX = MIN_WALL_JUMP_SPEED;
		}
		if (sign < 0 && player.data.wallJumpVelocityX > -MIN_WALL_JUMP_SPEED)
		{
			player.data.wallJumpVelocityX = -MIN_WALL_JUMP_SPEED;
		}

		float inputVelocity = 0;
		if (movingRight)
		{
			inputVelocity = player.data.speed;
		}
		if (movingLeft)
		{
			inputVelocity = -player.data.speed;
		}

		player.data.velocity.x = player.data.wallJumpVelocityX + inputVelocity * 0.3f;

		if (player.action.isGrounded)
		{
			player.action.isWallJumping = sfFalse;
		}
	}

	else if (player.action.isSlideJumping)
	{
		float sign = (player.data.slideVelocityX > 0) ? 1.f : -1.f;
		player.data.slideVelocityX -= sign * SLIDE_FRICTION * _dt;

		if (sign > 0 && player.data.slideVelocityX < MIN_SLIDE_JUMP_SPEED)
		{
			player.data.slideVelocityX = MIN_SLIDE_JUMP_SPEED;
		}
		if (sign < 0 && player.data.slideVelocityX > -MIN_SLIDE_JUMP_SPEED)
		{
			player.data.slideVelocityX = -MIN_SLIDE_JUMP_SPEED;
		}

		float inputVelocity = 0;
		if (movingRight)
		{
			inputVelocity = player.data.speed;
		}
		if (movingLeft)
		{
			inputVelocity = -player.data.speed;
		}

		player.data.velocity.x = player.data.slideVelocityX + inputVelocity * 0.4f;

		if (player.action.isGrounded)
		{
			player.action.isSlideJumping = sfFalse;
		}
	}

	else if (player.action.isSliding)
	{
		player.data.slideTimer -= _dt;

		float sign = (player.data.lastDirection > 0) ? 1.f : -1.f;
		player.data.slideVelocityX -= sign * SLIDE_FRICTION * _dt;

		if (sign > 0 && player.data.slideVelocityX < 0)
		{
			player.data.slideVelocityX = 0;
		}
		if (sign < 0 && player.data.slideVelocityX > 0)
		{
			player.data.slideVelocityX = 0;
		}

		float inputVelocity = 0;
		if (movingRight)
		{
			inputVelocity = player.data.speed;
			player.data.lastDirection = 1;
			sfSprite_setScale(player.sprite, (sfVector2f) { GAME_SCALE, GAME_SCALE });
		}
		else if (movingLeft)
		{
			inputVelocity = -player.data.speed;
			player.data.lastDirection = -1;
			sfSprite_setScale(player.sprite, (sfVector2f) { -GAME_SCALE, GAME_SCALE });
		}

		player.data.velocity.x = player.data.slideVelocityX + inputVelocity * 0.5f;

		if (player.data.slideTimer <= 0 || !slideKey)
		{
			player.action.isSliding = sfFalse;
			player.data.slideVelocityX = 0;
			player.data.slideCooldownTimer = SLIDE_COOLDOWN;
			StateMachine(player.action.isMoving ? RUN : IDLE);
		}
	}
	// DEPLACEMENT GAUCHE DROITE
	else
	{
		if (movingRight)
		{
			player.data.velocity.x = player.data.speed;
			player.data.lastDirection = 1;
			sfSprite_setScale(player.sprite, (sfVector2f) { GAME_SCALE, GAME_SCALE });
			player.action.isMoving = sfTrue;
		}
		else if (movingLeft)
		{
			player.data.velocity.x = -player.data.speed;
			player.data.lastDirection = -1;
			sfSprite_setScale(player.sprite, (sfVector2f) { -GAME_SCALE, GAME_SCALE });
			player.action.isMoving = sfTrue;
		}
		else
		{
			player.data.velocity.x = 0;
			player.action.isMoving = sfFalse;
		}

		// SLIDE
		if (slideKey && player.action.isGrounded && !player.action.isSliding && player.data.slideCooldownTimer <= 0.f)
		{
			player.action.isSliding = sfTrue;
			player.data.slideTimer = SLIDE_DURATION;
			player.data.slideVelocityX = player.data.lastDirection * SLIDE_SPEED;
			StateMachine(SLIDE);
		}
	}


	if (jumpKey && !jumpPressed)
	{
		jumpPressed = sfTrue;
		player.data.jumpStartPosition = player.data.position.y;

		if (player.action.isGrounded || (player.action.isSliding && player.action.isGrounded))
		{
			player.data.lastWallTouched = 0;
			if (player.action.isSliding)
			{
				player.action.isSlideJumping = sfTrue;
				player.data.slideVelocityX = player.data.velocity.x;
				player.action.isSliding = sfFalse;
				player.data.slideCooldownTimer = SLIDE_COOLDOWN;
			}

			player.data.velocity.y = -JUMP_FORCE;
			player.action.isGrounded = sfFalse;
			StateMachine(JUMP);
		}
		else
		{
			float dx = player.data.velocity.x * _dt;
			if (CheckCollisionPlayerPlatformsX(dx))
			{
				player.data.currentWallTouched = player.action.isTouchingRightWall ? 1 : -1;
				if (player.data.lastWallTouched == player.data.currentWallTouched)
				{
					return;
				}

				player.data.lastWallTouched = player.data.currentWallTouched;

				float wallJumpHX = 550.f;
				player.data.velocity.y = -JUMP_FORCE * 0.75;

				if (player.action.isTouchingRightWall)
				{
					player.action.isWallJumping = sfTrue;
					player.action.justWallJumped = sfTrue;
					player.data.wallJumpVelocityX = -wallJumpHX;
					player.data.lastDirection = -1;
					sfSprite_setScale(player.sprite, (sfVector2f) { -GAME_SCALE, GAME_SCALE });
				}
				else if (player.action.isTouchingLeftWall)
				{
					player.action.isWallJumping = sfTrue;
					player.action.justWallJumped = sfTrue;
					player.data.wallJumpVelocityX = wallJumpHX;
					player.data.lastDirection = 1;
					sfSprite_setScale(player.sprite, (sfVector2f) { GAME_SCALE, GAME_SCALE });
				}

				player.action.isGrounded = sfFalse;
				player.action.isSlideJumping = sfFalse;
				StateMachine(WALL_JUMP);
			}
		}
	}

	if (!jumpKey)
	{
		jumpPressed = sfFalse;
	}


	// ANIMATION
	if (player.action.isGrounded && !player.action.isSliding)
	{
		if (player.currentState != TURN)
		{
			if (player.action.isMoving)
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
			if (player.action.isMoving)
			{
				StateMachine(RUN);
			}
			else
			{
				StateMachine(IDLE);
			}
		}
	}
	else if (!player.action.isGrounded)
	{
		if (player.data.velocity.y < 0)
		{
			float dx = player.data.velocity.x * _dt;
			CheckCollisionPlayerPlatformsX(dx);

			if (movingLeft && player.action.isTouchingLeftWall && player.currentState == JUMP)
			{
				// Verifie la distance verticale du saut avant de grip le wall
				// pour ne pas rester coincé au sol si on se colle à un mur et qu'on saute
				//cheh Damien mon code marche mieux que toi :)
				float fallenDistance = player.data.position.y - player.data.jumpStartPosition;
				if (-fallenDistance > MIN_WALL_GRIP_DISTANCE)
				{
					player.data.velocity.y = 0;
					StateMachine(WALL_GRIP_FALL);
				}
			}
			else if (movingRight && player.action.isTouchingRightWall && player.currentState == JUMP)
			{
				float fallenDistance = player.data.position.y - player.data.jumpStartPosition;
				if (-fallenDistance > MIN_WALL_GRIP_DISTANCE)
				{
					player.data.velocity.y = 0;
					StateMachine(WALL_GRIP_FALL);
				}
			}

			if (player.currentState != JUMP && player.currentState != WALL_JUMP)
			{
				StateMachine(JUMP);
			}
		}
		else
		{
			float dx = player.data.velocity.x * _dt;
			CheckCollisionPlayerPlatformsX(dx);
			if (movingLeft && player.action.isTouchingLeftWall)
			{
				StateMachine(WALL_GRIP_FALL);
				player.data.velocity.y -= 10;
			}
			else if (movingRight && player.action.isTouchingRightWall)
			{
				StateMachine(WALL_GRIP_FALL);
				player.data.velocity.y -= 10;
			}
			else
			{
				StateMachine(FALL);
			}
		}
	}
}

void CollisionPlayerPlatformsX(float _dx)
{
	player.action.isTouchingLeftWall = sfFalse;
	player.action.isTouchingRightWall = sfFalse;

	float playerHalfWidth = (PLAYER_HITBOX_WIDTH * GAME_SCALE) / 2.f;
	float playerWidth = PLAYER_HITBOX_WIDTH * GAME_SCALE;
	float playerHeight = PLAYER_HITBOX_HEIGHT * GAME_SCALE;

	sfFloatRect hitbox = { player.data.position.x - playerHalfWidth + _dx, player.data.position.y - playerHeight, playerWidth, playerHeight };

	for (unsigned i = 0; i < GetCollisionTabSize(); i++)
	{
		sfFloatRect platform = GetMapCollision(i);
		if (sfFloatRect_intersects(&hitbox, &platform, NULL))
		{
			if (player.data.velocity.x > 0)
			{
				hitbox.left = platform.left - hitbox.width;
			}
			else if (player.data.velocity.x < 0)
			{
				hitbox.left = platform.left + platform.width;
			}

			if (!player.action.justWallJumped)
			{
				player.data.velocity.x = 0;
				player.data.position.x = hitbox.left + hitbox.width / 2.f;
			}

			sfSprite_setPosition(player.sprite, player.data.position);
			sfRectangleShape_setPosition(player.shape.collisionPlayerShape, (sfVector2f) { hitbox.left, hitbox.top });
			player.shape.collisionPlayerRect = sfRectangleShape_getGlobalBounds(player.shape.collisionPlayerShape);
			player.shape.playerRect = sfSprite_getGlobalBounds(player.sprite);
			return;
		}
	}

	player.action.justWallJumped = sfFalse;
	player.data.position.x += _dx;
	sfSprite_setPosition(player.sprite, player.data.position);
	sfRectangleShape_setPosition(player.shape.collisionPlayerShape, (sfVector2f) { player.data.position.x - playerHalfWidth, player.data.position.y - playerHeight });
	player.shape.collisionPlayerRect = sfRectangleShape_getGlobalBounds(player.shape.collisionPlayerShape);
	player.shape.playerRect = sfSprite_getGlobalBounds(player.sprite);
}
void CollisionPlayerPlatformsY(float _dy)
{
	float playerHalfWidth = (PLAYER_HITBOX_WIDTH * GAME_SCALE) / 2.f;
	float playerWidth = PLAYER_HITBOX_WIDTH * GAME_SCALE;
	float playerHeight = PLAYER_HITBOX_HEIGHT * GAME_SCALE;

	player.data.lastWallTouched = 0;
	sfFloatRect hitbox = { player.data.position.x - playerHalfWidth, player.data.position.y - playerHeight + _dy, playerWidth, playerHeight };
	player.action.isGrounded = sfFalse;

	for (unsigned i = 0; i < GetCollisionTabSize(); i++)
	{
		sfFloatRect platform = GetMapCollision(i);
		if (sfFloatRect_intersects(&hitbox, &platform, NULL))
		{
			if (player.data.velocity.y > 0)
			{
				hitbox.top = platform.top - hitbox.height;
				player.action.isGrounded = sfTrue;
			}
			else if (player.data.velocity.y < 0)
			{
				hitbox.top = platform.top + platform.height;
			}

			player.data.velocity.y = 0;
			player.data.position.y = hitbox.top + hitbox.height;
			sfSprite_setPosition(player.sprite, player.data.position);
			sfRectangleShape_setPosition(player.shape.collisionPlayerShape, (sfVector2f) { hitbox.left, hitbox.top });
			player.shape.collisionPlayerRect = sfRectangleShape_getGlobalBounds(player.shape.collisionPlayerShape);
			player.shape.playerRect = sfSprite_getGlobalBounds(player.sprite);
			return;
		}
	}

	player.data.position.y += _dy;
	sfSprite_setPosition(player.sprite, player.data.position);
	sfRectangleShape_setPosition(player.shape.collisionPlayerShape, (sfVector2f) { player.data.position.x - playerHalfWidth, player.data.position.y - playerHeight });
	player.shape.collisionPlayerRect = sfRectangleShape_getGlobalBounds(player.shape.collisionPlayerShape);
	player.shape.playerRect = sfSprite_getGlobalBounds(player.sprite);
}

sfBool CheckCollisionPlayerPlatformsX(float _dx)
{
	player.action.isTouchingLeftWall = sfFalse;
	player.action.isTouchingRightWall = sfFalse;

	float playerHalfWidth = (PLAYER_HITBOX_WIDTH * GAME_SCALE) / 2.f;
	float playerWidth = PLAYER_HITBOX_WIDTH * GAME_SCALE;
	float playerHeight = PLAYER_HITBOX_HEIGHT * GAME_SCALE;

	sfFloatRect hitbox = { player.data.position.x - playerHalfWidth + _dx, player.data.position.y - playerHeight, playerWidth, playerHeight };

	for (unsigned i = 0; i < GetCollisionTabSize(); i++)
	{
		sfFloatRect platform = GetMapCollision(i);
		if (sfFloatRect_intersects(&hitbox, &platform, NULL))
		{
			float playerCenterX = player.data.position.x;
			float platformCenterX = platform.left + platform.width * 0.5f;

			if (playerCenterX < platformCenterX)
			{
				player.action.isTouchingRightWall = sfTrue;
			}
			else
			{
				player.action.isTouchingLeftWall = sfTrue;
			}

			return sfTrue;
		}
	}
	return sfFalse;
}
void CheckCollisionPlayerPlatforms(float _dt)
{
	float dx = player.data.velocity.x * _dt;
	CollisionPlayerPlatformsX(dx);

	float dy = player.data.velocity.y * _dt;
	CollisionPlayerPlatformsY(dy);

	sfRectangleShape_setPosition(player.shape.collisionPlayerShape, (sfVector2f) { player.data.position.x - (PLAYER_HITBOX_WIDTH * GAME_SCALE) / 2.f, player.data.position.y - (PLAYER_HITBOX_HEIGHT * GAME_SCALE) });
	player.shape.collisionPlayerRect = sfRectangleShape_getGlobalBounds(player.shape.collisionPlayerShape);
	player.shape.playerRect = sfSprite_getGlobalBounds(player.sprite);
}

void basePlayer()
{
	player.sprite = sfSprite_create();
	player.texture = sfTexture_createFromFile("Assets/Sprites/IDLE.png", NULL);
	sfSprite_setTexture(player.sprite, player.texture, sfTrue);
	sfSprite_setScale(player.sprite, (sfVector2f) { GAME_SCALE, GAME_SCALE });
	sfSprite_setPosition(player.sprite, GetPlayerSpawn());

	player.shape.collisionPlayerShape = sfRectangleShape_create();
	sfRectangleShape_setSize(player.shape.collisionPlayerShape, (sfVector2f) { PLAYER_HITBOX_WIDTH* GAME_SCALE, PLAYER_HITBOX_HEIGHT* GAME_SCALE });

	sfVector2f spritePos = sfSprite_getPosition(player.sprite);
	sfRectangleShape_setPosition(player.shape.collisionPlayerShape, (sfVector2f) { spritePos.x - (PLAYER_HITBOX_WIDTH * GAME_SCALE) / 2.f, spritePos.y - (PLAYER_HITBOX_HEIGHT * GAME_SCALE) });

	sfRectangleShape_setOutlineColor(player.shape.collisionPlayerShape, sfRed);
	sfRectangleShape_setFillColor(player.shape.collisionPlayerShape, sfColor_fromRGBA(255, 255, 255, 50));

	sfSprite_setOrigin(player.sprite, (sfVector2f) { PLAYER_WIDTH / 2.f, PLAYER_HEIGHT });


	player.lastState = IDLE;

	player.action.isAttacking = sfFalse;
	player.action.isGrounded = sfFalse;
	player.action.isMoving = sfFalse;

	player.action.isSlideJumping = sfFalse;
	player.action.isSliding = sfFalse;

	player.action.isTouchingLeftWall = sfFalse;
	player.action.isTouchingRightWall = sfFalse;
	player.action.isTouchingWall = sfFalse;
	player.action.isWallJumping = sfFalse;

	player.data.position = sfSprite_getPosition(player.sprite);
	player.shape.collisionPlayerRect = sfRectangleShape_getGlobalBounds(player.shape.collisionPlayerShape);
	player.shape.playerRect = sfSprite_getGlobalBounds(player.sprite);

	snprintf(player.data.level, sizeof(player.data.level), "level_00");
	printf("player level: %s\n", player.data.level);
	player.data.attackCooldownTimer = 0.5f;
	player.data.lastWallTouched = 0;

	player.data.maxHealth = 200;
	player.data.health = player.data.maxHealth;

	player.data.speed = 350.f;
	player.data.jumpStartPosition = 0;

	player.data.velocity.x = 0;
	player.data.velocity.y = 0;

	player.data.slideVelocityX = 0;
	player.data.slideCooldownTimer = 0.f;

	player.data.lastDirection = 1;

	player.data.canDoubleJump = 0;
	player.data.canWallJump = 0;

	player.sound.swordSound = sfSound_create();
	player.sound.buffer = sfSoundBuffer_createFromFile("Assets/Sounds/Sword_Attack_Sound.wav");
	sfSound_setBuffer(player.sound.swordSound, player.sound.buffer);

}

void setSavedStat(PlayerSaveData* save)
{

	player.data.health = save->health;
	player.data.canDoubleJump = save->canDoubleJump;
	player.data.canWallJump = save->canWallJump;

	snprintf(player.data.level, sizeof(player.data.level), "%s", save->level);
	printf("buffer: %s\n", player.data.level);
}
 

void CollisionPlayerTrigger()
{
	sfBool keyIsPressed = sfKeyboard_isKeyPressed(sfKeyE);

	for (int i = 0; i < GetTriggerTabSize(); i++)
	{
		sfFloatRect trigger = {
			GetMapTrigger(i).left,
			GetMapTrigger(i).top,
			GetMapTrigger(i).width,
			GetMapTrigger(i).height
		};

		if (sfFloatRect_intersects(&player.shape.collisionPlayerRect, &trigger, NULL))
		{
			if (keyIsPressed && !keyWasPressed)
			{
				snprintf(player.data.level, sizeof(player.data.level), "%s", GetMapTrigger(i).name);
				LoadMap(player.data.level);
				CleanupGame();
				LoadGame();
			}
		}
	}

	keyWasPressed = keyIsPressed;
}

void DrawPlayer(sfRenderWindow* _renderWindow)
{
	sfRenderWindow_drawSprite(_renderWindow, player.sprite, NULL);
	sfRenderWindow_drawRectangleShape(_renderWindow, player.shape.collisionPlayerShape, NULL);
	if (player.action.isAttacking)
	{
		sfRenderWindow_drawRectangleShape(_renderWindow, player.shape.collisionAttackShape, NULL);

	}
}
void CleanUpPlayer(void)
{
	sfSprite_destroy(player.sprite);
	sfTexture_destroy(player.texture);
	sfRectangleShape_destroy(player.shape.collisionPlayerShape);
}

void StateMachine(PlayerState _state)
{
	if (player.currentState == _state)
	{
		return;
	}
	SetAnimation(_state);
}

