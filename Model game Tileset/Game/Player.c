#include "Player.h"

Player player;
PlayerSaveData save;
sfBool keyWasPressed = sfFalse;
unsigned mobCount;
Mob* mob;

void LoadAnimationPlayer(void);
void SetAnimation(PlayerState _state);
void createCollisionAttack();

void UpdatePlayer(float _dt);
void ApplyPhysic(float _dt);
void MovePlayer(float _dt);

void CollisionPlayerPlatformsX(float _dx);
void CollisionPlayerPlatformsY(float _dy);

sfBool CheckCollisionPlayerPlatformsX(float _dx);
void CheckCollisionPlayerSpike(unsigned _index, float _dt);
void CheckCollisionPlayerPlatforms(float _dt);
void CheckCollisionPlayerAttackMob(float _dt);
void CheckCollisionPlayerMob(float _dt);
void CollisionPlayerTrigger();
void CollisionPlayerDeathZone();

void CollisionPlayerItems(unsigned index);
float RandomFloat(float min, float max);

void BasePlayer();
void SetSavedStat(PlayerSaveData* save);

void UpdateAttackShape();
void CheckPlayerHP(void);


void LoadPlayer(PlayerSaveData* save)
{
	BasePlayer();
	LoadAnimationPlayer();

}

void LoadAnimationPlayer(void)
{
	sfIntRect firstFrame = { 0, 0 * PLAYER_HEIGHT , PLAYER_WIDTH, PLAYER_HEIGHT };
	player.animationPlayer[IDLE] = CreateAnimation(player.sprite, 5, 7, sfTrue, sfTrue, firstFrame);

	firstFrame = (sfIntRect){ 0, 1 * PLAYER_HEIGHT, PLAYER_WIDTH, PLAYER_HEIGHT };
	player.animationPlayer[RUN] = CreateAnimation(player.sprite, 6, 11, sfTrue, sfTrue, firstFrame);

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


	UpdateAttackShape();


	MovePlayer(_dt);

	CheckCollisionPlayerPlatforms(_dt);
	CheckCollisionPlayerSpike(NULL, _dt);
	CollisionPlayerTrigger();
	CheckCollisionPlayerAttackMob(_dt);
	CheckCollisionPlayerMob(_dt);
	CollisionPlayerDeathZone();

	CheckPlayerHP();
	UpdateAnimation(player.currentAnimation, _dt);
}

void CheckCollisionPlayerAttackMob(float _dt)
{


	for (int i = 0; i < GetMobCount(); i++)
	{
		if (mob[i].act != IS_DEATH)
		{
			if (mob[i].act == IS_ATTACK && mob[i].timer.timerAttack > mob[i].timer.timerAttackLimit)
			{

				StateMobMachine(ATTACK_MOB, i);
				mob[i].timer.timerAttack = 0;
			}
			else if (!mob[i].currentMobAnimation->isPlaying && mob[i].act != IS_TAKE_HIT)
			{
				mob[i].act = IS_IDLE;
				StateMobMachine(IDLE_MOB, i);
			}
		}
	}



	sfFloatRect hitPlayer = sfSprite_getGlobalBounds(player.sprite);
	sfFloatRect hitMob = { 0 };
	sfFloatRect intersection;
	for (int i = 0; i < GetMobCount(); i++)
	{
		hitMob = sfRectangleShape_getGlobalBounds(mob[i].rect);

		if (player.data.health > 0)
		{


			if (mob[i].currentState == ATTACK_MOB && player.data.timerInvincible > TIMER_INVINCIBLE)
			{
				if (mob[i].currentMobAnimation->currentFrame == (mob[i].currentMobAnimation->frameCount - 2) && !player.action.damageEnable && sfFloatRect_intersects(&hitMob, &hitPlayer, &intersection))
				{
					player.action.damageEnable = sfTrue;

					player.data.health -= mob[i].damage + rand() % mob[i].damage;

					player.data.health -= mob[i].damage + rand() % mob[i].damage;
					

				}
				else if (mob[i].currentMobAnimation->currentFrame != (mob[i].currentMobAnimation->frameCount - 2))
				{
					player.action.damageEnable = sfFalse;
				}
			}

		}

	}


}

void CheckCollisionPlayerMob(float _dt)
{
	player.data.timerInvincible += _dt;

	sfRectangleShape_setPosition(player.shape.rectCollisionPlayerMob, sfSprite_getPosition(player.sprite));
	player.shape.hitRectCollisionPlayerMob = sfRectangleShape_getGlobalBounds(player.shape.rectCollisionPlayerMob);

	player.data.timerPlayerMob += _dt;

	sfFloatRect hitPlayer = player.shape.hitRectCollisionPlayerMob;
	sfFloatRect hitMob;
	sfFloatRect intersection;
	float playerCenterX = hitPlayer.left + (hitPlayer.width * 0.5f);
	float mobCenterX;

	for (int i = 0; i < GetMobCount(); i++)
	{
		hitMob = mob[i].collisionMob;
		mobCenterX = hitMob.left + (hitMob.width * 0.5f);

		if (player.currentState != AXE && player.currentState != SWORD && !player.action.isInvincible && mob[i].currentState != DEATH)
		{
			if (sfFloatRect_intersects(&hitMob, &hitPlayer, &intersection))
			{
				player.data.timerPlayerMob = 0;
				player.data.knockBackTimer += 0.2f;
				player.action.isSlideJumping = sfFalse;
				player.action.isTouchingWall = sfFalse;
				player.action.isInvincible = sfTrue;
				player.data.timerInvincible = 0;
				player.data.health -= 15;

				if (playerCenterX > (mobCenterX)+PLAYER_MOB_MARGE)
				{
					player.side = LEFT_PLAYER;
				}
				else if (playerCenterX < (mobCenterX)-PLAYER_MOB_MARGE)
				{
					player.side = WIDTH_PLAYER;
				}

			}
		}

	}

	if (player.side != NOTHING_PLAYER)
	{
		player.action.isGrounded = sfFalse;
		player.data.velocity.y = -PLAYER_MOB_VELOCITY_Y;

		if (player.side == LEFT_PLAYER)
		{
			sfSprite_setScale(player.sprite, (sfVector2f) { GAME_SCALE, GAME_SCALE });
			player.data.velocity.x = PLAYER_MOB_VELOCITY_X;

		}
		else if (player.side == WIDTH_PLAYER)
		{
			sfSprite_setScale(player.sprite, (sfVector2f) { -GAME_SCALE, GAME_SCALE });
			player.data.velocity.x = -PLAYER_MOB_VELOCITY_X;
		}

		player.side = NOTHING_PLAYER;
	}

	if (player.data.timerInvincible > TIMER_INVINCIBLE)
	{
		player.action.isInvincible = sfFalse;
	}

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

void ApplyHorizontalInput(sfBool movingLeft, sfBool movingRight)
{
	if (movingRight)
	{
		player.data.velocity.x = player.data.speed;
		player.data.lastDirection = 1;
		player.action.isMoving = sfTrue;
		sfSprite_setScale(player.sprite, (sfVector2f) { GAME_SCALE, GAME_SCALE });
	}
	else if (movingLeft)
	{
		player.data.velocity.x = -player.data.speed;
		player.data.lastDirection = -1;
		player.action.isMoving = sfTrue;
		sfSprite_setScale(player.sprite, (sfVector2f) { -GAME_SCALE, GAME_SCALE });
	}
	else
	{
		player.data.velocity.x = 0;
		player.action.isMoving = sfFalse;
	}
}


void HandleAttackInput(float _dt, sfBool movingLeft, sfBool movingRight)
{
	player.data.attackCooldownTimer += _dt;

	if (sfMouse_isButtonPressed(sfMouseLeft) &&
		player.data.attackCooldownTimer >= ATTACK_AXE_COOLDOWN &&
		player.action.isGrounded)
	{
		createCollisionAttack();
		player.action.isAttacking = sfTrue;
		player.data.attackCooldownTimer = 0.f;
		player.data.velocity.x = 0;
		sfSound_setPitch(player.sound.axeSound, RandomFloat(0.8f, 1.2f));
		sfSound_play(player.sound.axeSound);
		StateMachine(AXE);
	}

	if (sfMouse_isButtonPressed(sfMouseRight) &&
		player.data.attackCooldownTimer >= ATTACK_SWORD_COOLDOWN &&
		player.action.isGrounded)
	{
		createCollisionAttack();
		player.action.isAttacking = sfTrue;
		player.data.attackCooldownTimer = 0.f;
		sfSound_setPitch(player.sound.swordSound, RandomFloat(0.9f, 1.2f));
		sfSound_play(player.sound.swordSound);
		StateMachine(SWORD);
	}
}


sfBool HandleAttackState(sfBool movingLeft, sfBool movingRight)
{
	if (!player.action.isAttacking)
	{
		return sfFalse;

	}

	if (player.currentState == AXE)
	{
		player.data.velocity.x = 300 * player.data.lastDirection;

		if (player.currentAnimation->currentFrame >= player.currentAnimation->frameCount - 1)
		{
			sfRectangleShape_destroy(player.shape.collisionAttackShape);
			player.action.isAttacking = sfFalse;
			StateMachine(player.action.isGrounded ? (player.action.isMoving ? RUN : IDLE) : FALL);
		}
		return sfTrue;
	}

	if (player.currentState == SWORD)
	{
		float horizontalInput = 0.f;

		if (movingRight && player.data.lastDirection == 1)
		{
			horizontalInput = player.data.speed;
			player.action.isMoving = sfTrue;
		}
		else if (movingLeft && player.data.lastDirection == -1)
		{
			horizontalInput = -player.data.speed;
			player.action.isMoving = sfTrue;
		}
		else
		{
			horizontalInput = 0.f;
			player.action.isMoving = sfFalse;
		}

		player.data.velocity.x = horizontalInput;


		if (player.currentAnimation->currentFrame >= player.currentAnimation->frameCount - 1)
		{
			sfRectangleShape_destroy(player.shape.collisionAttackShape);
			player.action.isAttacking = sfFalse;
			StateMachine(player.action.isGrounded ? (player.action.isMoving ? RUN : IDLE) : FALL);
		}
		return sfTrue;
	}

	return sfFalse;
}


void HandleWallJumping(float _dt, sfBool movingLeft, sfBool movingRight)
{
	float sign = (player.data.wallJumpVelocityX > 0) ? 1.f : -1.f;
	player.data.wallJumpVelocityX -= sign * WALL_JUMP_FRICTION * _dt;

	if (sign > 0 && player.data.wallJumpVelocityX < MIN_WALL_JUMP_SPEED)
		player.data.wallJumpVelocityX = MIN_WALL_JUMP_SPEED;
	if (sign < 0 && player.data.wallJumpVelocityX > -MIN_WALL_JUMP_SPEED)
		player.data.wallJumpVelocityX = -MIN_WALL_JUMP_SPEED;

	float inputVelocity = movingRight ? player.data.speed : (movingLeft ? -player.data.speed : 0);
	player.data.velocity.x = player.data.wallJumpVelocityX + inputVelocity * 0.3f;

	if (player.action.isGrounded)
		player.action.isWallJumping = sfFalse;
}

void HandleSlideJumping(float _dt, sfBool movingLeft, sfBool movingRight)
{
	float sign = (player.data.slideVelocityX > 0) ? 1.f : -1.f;
	player.data.slideVelocityX -= sign * SLIDE_FRICTION * _dt;

	if (sign > 0 && player.data.slideVelocityX < MIN_SLIDE_JUMP_SPEED)
		player.data.slideVelocityX = MIN_SLIDE_JUMP_SPEED;
	if (sign < 0 && player.data.slideVelocityX > -MIN_SLIDE_JUMP_SPEED)
		player.data.slideVelocityX = -MIN_SLIDE_JUMP_SPEED;

	float inputVelocity = movingRight ? player.data.speed : (movingLeft ? -player.data.speed : 0);
	player.data.velocity.x = player.data.slideVelocityX + inputVelocity * 0.4f;

	if (player.action.isGrounded)
		player.action.isSlideJumping = sfFalse;
}

void HandleSliding(float _dt, sfBool movingLeft, sfBool movingRight, sfBool slideKey)
{
	player.data.slideTimer -= _dt;

	float sign = (player.data.lastDirection > 0) ? 1.f : -1.f;
	player.data.slideVelocityX -= sign * SLIDE_FRICTION * _dt;

	if (sign > 0 && player.data.slideVelocityX < 0) player.data.slideVelocityX = 0;
	if (sign < 0 && player.data.slideVelocityX > 0) player.data.slideVelocityX = 0;

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

void HandleGroundMovement(sfBool movingLeft, sfBool movingRight, sfBool slideKey)
{
	ApplyHorizontalInput(movingLeft, movingRight);

	if (slideKey && player.action.isGrounded && !player.action.isSliding && player.data.slideCooldownTimer <= 0.f)
	{
		player.action.isSliding = sfTrue;
		player.data.slideTimer = SLIDE_DURATION;
		player.data.slideVelocityX = player.data.lastDirection * SLIDE_SPEED;
		StateMachine(SLIDE);
	}
}


void HandleJump(float _dt, sfBool movingLeft, sfBool movingRight, sfBool jumpKey)
{
	static sfBool jumpPressed = sfFalse;

	if (jumpKey && !jumpPressed)
	{
		jumpPressed = sfTrue;
		player.data.jumpStartPosition = player.data.position.y;

		if (player.action.isGrounded)
		{
			player.action.justWallJumped = sfFalse;

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

				float wallJumpHX = 550.f;

				if (player.action.isTouchingRightWall && movingRight)
				{
					player.action.isWallJumping = sfTrue;
					player.action.justWallJumped = sfTrue;
					player.data.wallJumpVelocityX = -wallJumpHX;
					player.data.lastDirection = -1;
					player.data.velocity.y = -JUMP_FORCE * 0.75f;
					sfSprite_setScale(player.sprite, (sfVector2f) { -GAME_SCALE, GAME_SCALE });
				}
				else if (player.action.isTouchingLeftWall && movingLeft)
				{
					player.action.isWallJumping = sfTrue;
					player.action.justWallJumped = sfTrue;
					player.data.wallJumpVelocityX = wallJumpHX;
					player.data.lastDirection = 1;
					player.data.velocity.y = -JUMP_FORCE * 0.75f;
					sfSprite_setScale(player.sprite, (sfVector2f) { GAME_SCALE, GAME_SCALE });
				}

				player.action.isGrounded = sfFalse;
				player.action.isSlideJumping = sfFalse;
				StateMachine(WALL_JUMP);
			}
		}
	}

	if (!jumpKey)
		jumpPressed = sfFalse;
}


void HandleAirAnimation(float _dt, sfBool movingLeft, sfBool movingRight)
{
	float dx = player.data.velocity.x * _dt;
	CheckCollisionPlayerPlatformsX(dx);

	if (player.data.velocity.y < 0)
	{
		if (movingLeft && player.action.isTouchingLeftWall && player.currentState == JUMP)
		{
			float fallen = player.data.position.y - player.data.jumpStartPosition;
			if (-fallen > MIN_WALL_GRIP_DISTANCE)
			{
				player.data.velocity.y = 0;
				StateMachine(WALL_GRIP_FALL);
			}
		}
		else if (movingRight && player.action.isTouchingRightWall && player.currentState == JUMP)
		{
			float fallen = player.data.position.y - player.data.jumpStartPosition;
			if (-fallen > MIN_WALL_GRIP_DISTANCE)
			{
				player.data.velocity.y = 0;
				StateMachine(WALL_GRIP_FALL);
			}
		}

		if (player.currentState != JUMP && player.currentState != WALL_JUMP)
			StateMachine(JUMP);
	}
	else
	{
		if (movingLeft && player.action.isTouchingLeftWall)
		{
			player.data.velocity.y -= 10;
			if (player.data.velocity.y >= MAX_GRIP_WALL_SPEED)
				player.data.velocity.y = MAX_GRIP_WALL_SPEED;
			StateMachine(WALL_GRIP_FALL);
		}
		else if (movingRight && player.action.isTouchingRightWall)
		{
			player.data.velocity.y -= 10;
			if (player.data.velocity.y >= MAX_GRIP_WALL_SPEED)
				player.data.velocity.y = MAX_GRIP_WALL_SPEED;
			StateMachine(WALL_GRIP_FALL);
		}
		else
		{
			StateMachine(FALL);
		}
	}
}

void HandleAnimationState(float _dt, sfBool movingLeft, sfBool movingRight)
{
	if (player.action.isGrounded && !player.action.isSliding)
	{
		if (player.currentState != TURN)
			StateMachine(player.action.isMoving ? RUN : IDLE);

		if (player.currentState == TURN &&
			player.currentAnimation->currentFrame >= player.currentAnimation->frameCount - 1)
			StateMachine(player.action.isMoving ? RUN : IDLE);
	}
	else if (!player.action.isGrounded)
	{
		HandleAirAnimation(_dt, movingLeft, movingRight);
	}
}


void MovePlayer(float _dt)
{
	if (player.data.knockBackTimer > 0.f)
	{
		player.data.knockBackTimer -= _dt;
		return;
	}

	sfBool movingLeft = sfKeyboard_isKeyPressed(sfKeyQ);
	sfBool movingRight = sfKeyboard_isKeyPressed(sfKeyD);
	sfBool slideKey = sfKeyboard_isKeyPressed(sfKeyLControl) || sfKeyboard_isKeyPressed(sfKeyRControl);
	sfBool jumpKey = sfKeyboard_isKeyPressed(sfKeySpace);

	if (player.data.slideCooldownTimer > 0.f)
	{
		player.data.slideCooldownTimer -= _dt;

	}

	HandleAttackInput(_dt, movingLeft, movingRight);

	if (HandleAttackState(movingLeft, movingRight))
	{
		return;

	}

	if (player.action.isWallJumping)
		HandleWallJumping(_dt, movingLeft, movingRight);
	else if (player.action.isSlideJumping)
	{
		HandleSlideJumping(_dt, movingLeft, movingRight);

	}
	else if (player.action.isSliding)
	{
		HandleSliding(_dt, movingLeft, movingRight, slideKey);

	}
	else
		HandleGroundMovement(movingLeft, movingRight, slideKey);

	HandleJump(_dt, movingLeft, movingRight, jumpKey);
	HandleAnimationState(_dt, movingLeft, movingRight);
}


void CollisionPlayerPlatformsX(float _dx)
{
	sfBool sKey = sfKeyboard_isKeyPressed(sfKeyS);
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
	sfBool sKey = sfKeyboard_isKeyPressed(sfKeyS);
	float playerHalfWidth = (PLAYER_HITBOX_WIDTH * GAME_SCALE) / 2.f;
	float playerWidth = PLAYER_HITBOX_WIDTH * GAME_SCALE;
	float playerHeight = PLAYER_HITBOX_HEIGHT * GAME_SCALE;

	float previousBottom = player.data.position.y;

	sfFloatRect hitbox = { player.data.position.x - playerHalfWidth,player.data.position.y - playerHeight + _dy,playerWidth,playerHeight };
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

				player.action.justWallJumped = sfFalse;
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

	for (unsigned i = 0; i < GetSemiSolidCollisionTabSize(); i++)
	{
		sfFloatRect semi = GetSemiSolidCollisionTab(i);

		if (!sfFloatRect_intersects(&hitbox, &semi, NULL))
		{
			continue;
		}

		sfBool wasAbove = previousBottom <= semi.top;
		sfBool isFalling = player.data.velocity.y > 0;

		if (isFalling && wasAbove && !sKey)
		{
			hitbox.top = semi.top - hitbox.height;
			player.action.isGrounded = sfTrue;

			player.action.justWallJumped = sfFalse;


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


void CheckCollisionPlayerSpike(unsigned _index, float _dt)
{
	player.data.timerSpikeWidth += _dt;
	player.data.timerSpikeHeight += _dt;
	sfFloatRect hitPlayer = sfSprite_getGlobalBounds(player.sprite);
	sfFloatRect hitSpike = { 0 };
	sfFloatRect intersection = { 0 };
	float playerCenterX = hitPlayer.left + (hitPlayer.width / 2);



	for (unsigned i = 0; i < GetSpikeTabSize(); i++)
	{
		hitSpike = GetSpikeTab(i);
		float spikeCenterX = hitSpike.left + (hitSpike.width / 2);

		if (sfFloatRect_intersects(&hitSpike, &hitPlayer, &intersection) && player.data.timerSpikeWidth > TIMER_SPIKE)
		{
			player.data.knockBackTimer += 0.15f;
			if (playerCenterX < spikeCenterX && player.data.position.y >(hitSpike.top + (hitSpike.height / 2)))
			{

				player.spikeSide = WIDTH;
			}
			else if (playerCenterX > spikeCenterX && player.data.position.y > (hitSpike.top + (hitSpike.height / 2)))
			{

				player.spikeSide = LEFT;
			}

		}

	}

	if (player.spikeSide != NOTHING)
	{
		player.action.isGrounded = sfFalse;


		StateMachine(FALL);
		if (player.spikeSide == LEFT)
		{
			printf("LEFT\n");
			//player.action.isGrounded = sfFalse;
			player.data.velocity.y = -SPIKE_VELOCITY;
			player.data.velocity.x = SPIKE_VELOCITY;
			sfSprite_setScale(player.sprite, (sfVector2f) { GAME_SCALE, GAME_SCALE });


		}
		else if (player.spikeSide == WIDTH)
		{
			printf("RIGHT\n");
			//player.action.isGrounded = sfFalse;
			player.data.velocity.y = -SPIKE_VELOCITY;
			player.data.velocity.x = -SPIKE_VELOCITY;
			sfSprite_setScale(player.sprite, (sfVector2f) { -GAME_SCALE, GAME_SCALE });

		}

		player.data.timerSpikeHeight = 0.f;
		player.data.timerSpikeWidth = 0.f;
		player.data.health -= 20;
		player.spikeSide = NOTHING;
	}


	float playerCenterY = hitPlayer.top + (hitPlayer.height / 2);
	float spikeCenterY = 0;


	for (int i = 0; i < GetSpikeTabSize(); i++)
	{
		hitSpike = GetSpikeTab(i);
		spikeCenterY = hitSpike.top + (hitSpike.height / 2);


		if (sfFloatRect_intersects(&hitSpike, &hitPlayer, &intersection) && player.data.timerSpikeHeight > TIMER_SPIKE)
		{
			player.data.knockBackTimer += _dt;
			if (playerCenterY < spikeCenterY && (hitPlayer.left < (hitSpike.left + hitSpike.width)) && (hitPlayer.left + hitPlayer.width) > hitSpike.left)
			{
				player.spikeSide = TOP;
			}
			else if (playerCenterY > spikeCenterY && (hitPlayer.left < (hitSpike.left + hitSpike.width)) && (hitPlayer.left + hitPlayer.width) > hitSpike.left)
			{
				player.spikeSide = HEIGHT;
			}


		}
	}

	if (player.spikeSide != NOTHING)
	{
		player.action.isTouchingLeftWall = sfFalse;
		player.action.isTouchingRightWall = sfFalse;
		player.action.isTouchingWall = sfFalse;


		if (player.spikeSide == TOP)
		{
			printf("TOP\n");
			player.data.velocity.y = -SPIKE_VELOCITY;
		}
		else if (player.spikeSide == HEIGHT)
		{
			printf("HEIGHT\n");
			player.data.velocity.y = SPIKE_VELOCITY;
		}


		player.data.timerSpikeWidth = 0.f;
		player.data.timerSpikeHeight = 0.f;
		player.data.health -= 20;
		player.spikeSide = NOTHING;
		player.action.isMoving = sfTrue;

	}



}

void CheckPlayerHP(void)
{
	if (player.data.health <= 0)
	{
		ChangeLevel("Level_01");
		player.data.health = player.data.maxHealth;
	}
}


void BasePlayer()
{
	player.action.isTransitioning = sfTrue;
	player.sprite = sfSprite_create();
	player.texture = sfTexture_createFromFile("Assets/Sprites/Player/PlayerState.png", NULL);
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

	player.action.damageEnable = sfFalse;
	player.action.isInvincible = sfFalse;

	player.data.position = sfSprite_getPosition(player.sprite);
	player.shape.collisionPlayerRect = sfRectangleShape_getGlobalBounds(player.shape.collisionPlayerShape);
	player.shape.playerRect = sfSprite_getGlobalBounds(player.sprite);

	player.shape.rectCollisionPlayerMob = sfRectangleShape_create();
	sfRectangleShape_setSize(player.shape.rectCollisionPlayerMob, (sfVector2f){PLAYER_COLLISION_WIDTH, PLAYER_COLLISION_HEIGHT});
	sfRectangleShape_setOrigin(player.shape.rectCollisionPlayerMob, (sfVector2f){PLAYER_COLLISION_WIDTH / 2, PLAYER_COLLISION_HEIGHT});
	sfRectangleShape_setScale(player.shape.rectCollisionPlayerMob, (sfVector2f){GAME_SCALE, GAME_SCALE});


	snprintf(player.data.level, sizeof(player.data.level), "level_00");
	printf("player level: %s\n", player.data.level);
	player.data.attackCooldownTimer = 0.5f;


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

	player.data.timerTakeIt = 0;
	player.data.timerSpikeWidth = 0;
	player.data.timerSpikeHeight = 0;
	player.data.knockBackTimer = 0;
	player.data.timerPlayerMob = 0;
	player.data.timerInvincible = 0;

	player.spikeSide = NOTHING;
	player.side = NOTHING_PLAYER;

	player.sound.swordSound = sfSound_create();
	player.sound.buffer = sfSoundBuffer_createFromFile("Assets/Sounds/Sword_Attack_Sound.wav");
	sfSound_setBuffer(player.sound.swordSound, player.sound.buffer);

	player.sound.axeSound = sfSound_create();
	player.sound.buffer = sfSoundBuffer_createFromFile("Assets/Sounds/Axe_Attack_Sound.wav");
	sfSound_setBuffer(player.sound.axeSound, player.sound.buffer);

	player.data.keyNumber = 0;

}

void SetSavedStat(PlayerSaveData* save)
{

	player.data.health = save->health;
	player.data.canDoubleJump = save->canDoubleJump;
	player.data.canWallJump = save->canWallJump;

	snprintf(player.data.level, sizeof(player.data.level), "%s", save->level);
	printf("buffer: %s\n", player.data.level);
}

void UpdateAttackShape()
{
	if (player.shape.collisionAttackShape && player.data.lastDirection == -1)
	{
		sfRectangleShape_setPosition(player.shape.collisionAttackShape, (sfVector2f) { player.shape.collisionPlayerRect.left - player.shape.collisionAttackRect.width, player.shape.collisionPlayerRect.top + player.shape.collisionPlayerRect.width / 2 - 10 });
	}
	else if (player.shape.collisionAttackShape && player.data.lastDirection == 1)
	{
		sfRectangleShape_setPosition(player.shape.collisionAttackShape, (sfVector2f) { player.shape.collisionPlayerRect.left + player.shape.collisionPlayerRect.width, player.shape.collisionPlayerRect.top + player.shape.collisionPlayerRect.width / 2 - 10 });

	}
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
			if (keyIsPressed && !keyWasPressed && player.data.keyNumber >= GetMobCount())
			{
				player.data.velocity.x = 0;
				player.data.velocity.y = 0;
				snprintf(player.data.level, sizeof(player.data.level), "%s", GetMapTrigger(i).name);
				player.action.isTransitioning = sfTrue;
				ChangeLevel(player.data.level);
			}
		}
	}

	keyWasPressed = keyIsPressed;
}

void CollisionPlayerDeathZone()
{
	for (int i = 0; i < GetDeathZoneTabSize(); i++)
	{
		sfFloatRect deathZone = GetDeathZoneTab(i);

		if (sfFloatRect_intersects(&player.shape.collisionPlayerRect, &deathZone, NULL))
		{
			player.data.health = 0;
		}
	}
}

void DrawPlayer(sfRenderWindow* _renderWindow)
{
	sfRenderWindow_drawSprite(_renderWindow, player.shape.rectCollisionPlayerMob, NULL);
	sfRenderWindow_drawSprite(_renderWindow, player.sprite, NULL);
	//sfRenderWindow_drawRectangleShape(_renderWindow, player.shape.collisionPlayerShape, NULL);
	if (player.action.isAttacking)
	{
		//sfRenderWindow_drawRectangleShape(_renderWindow, player.shape.collisionAttackShape, NULL);

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

void CollisionPlayerItems(unsigned index)
{

}

float RandomFloat(float min, float max)
{
	return min + (float)rand() / (float)RAND_MAX * (max - min);
}