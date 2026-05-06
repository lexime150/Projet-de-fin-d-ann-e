#include "Player.h"

Player* player;
PlayerSaveData save;
sfBool keyWasPressed = sfFalse;
unsigned mobCount;
Mob* mob;

void LoadAnimationPlayer(void);
void SetAnimation(PlayerState _state);
void createCollisionSideAttack();

void UpdatePlayer(sfRenderWindow* _renderWindow, float _dt);
void ApplyPhysic(float _dt);
void MovePlayer(sfRenderWindow* _renderWindow, float _dt);

void CollisionPlayerPlatformsX(float _dx);
void CollisionPlayerPlatformsY(float _dy);

sfBool CheckCollisionPlayerPlatformsX(float _dx);
void CheckCollisionPlayerSpike(float _dt);
void CheckCollisionPlayerPlatforms(float _dt);
void CheckCollisionPlayerAttackMob(float _dt);
void CheckCollisionPlayerMob(float _dt);
void CollisionPlayerTrigger();
void CollisionPlayerDeathZone();

float RandomFloat(float min, float max);

void BasePlayer();
void SetSavedStat(PlayerSaveData* save);

void CheckPlayerHP(void);

static void GetPlayerHitboxSize(float* outWidth, float* outHeight)
{
	if (player->action.isSliding)
	{
		*outWidth = PLAYER_HITBOX_WIDTH * GAME_SCALE;
		*outHeight = (PLAYER_HITBOX_HEIGHT * GAME_SCALE) * 0.5f;
	}
	else
	{
		*outWidth = PLAYER_HITBOX_WIDTH * GAME_SCALE;
		*outHeight = PLAYER_HITBOX_HEIGHT * GAME_SCALE;
	}
}
void LoadPlayer(PlayerSaveData* save)
{
	BasePlayer();
	if (save != NULL)
	{
		SetSavedStat(save);
	}
	LoadAnimationPlayer();
}

void LoadAnimationPlayer(void)
{
	sfIntRect firstFrame = { 0, 0 * PLAYER_HEIGHT , PLAYER_WIDTH, PLAYER_HEIGHT };
	player->animationPlayer[IDLE] = CreateAnimation(player->sprite, 5, 7, sfTrue, sfTrue, firstFrame);

	firstFrame = (sfIntRect){ 0, 1 * PLAYER_HEIGHT, PLAYER_WIDTH, PLAYER_HEIGHT };
	player->animationPlayer[RUN] = CreateAnimation(player->sprite, 6, 11, sfTrue, sfTrue, firstFrame);

	firstFrame = (sfIntRect){ 0, 1 * PLAYER_HEIGHT, PLAYER_WIDTH, PLAYER_HEIGHT };
	player->animationPlayer[TURN] = CreateAnimation(player->sprite, 4, 7, sfTrue, sfTrue, firstFrame);

	firstFrame = (sfIntRect){ 0, 2 * PLAYER_HEIGHT, PLAYER_WIDTH, PLAYER_HEIGHT };
	player->animationPlayer[JUMP] = CreateAnimation(player->sprite, 3, 6, sfTrue, sfTrue, firstFrame);

	firstFrame = (sfIntRect){ 3 * PLAYER_WIDTH, 2 * PLAYER_HEIGHT, PLAYER_WIDTH, PLAYER_HEIGHT };
	player->animationPlayer[FALL] = CreateAnimation(player->sprite, 3, 6, sfTrue, sfTrue, firstFrame);

	firstFrame = (sfIntRect){ PLAYER_WIDTH * 6, 2 * PLAYER_HEIGHT, PLAYER_WIDTH, PLAYER_HEIGHT };
	player->animationPlayer[D_JUMP] = CreateAnimation(player->sprite, 3, 6, sfTrue, sfTrue, firstFrame);

	firstFrame = (sfIntRect){ 0, PLAYER_HEIGHT * 3 , PLAYER_WIDTH, PLAYER_HEIGHT };
	player->animationPlayer[DASH_GROUND] = CreateAnimation(player->sprite, 3, 11, sfTrue, sfFalse, firstFrame);

	firstFrame = (sfIntRect){ 4 * PLAYER_WIDTH, 3 * PLAYER_HEIGHT, PLAYER_WIDTH, PLAYER_HEIGHT };
	player->animationPlayer[DASH_UP] = CreateAnimation(player->sprite, 4, 7, sfTrue, sfFalse, firstFrame);

	firstFrame = (sfIntRect){ 8 * PLAYER_WIDTH, 3 * PLAYER_HEIGHT, PLAYER_WIDTH, PLAYER_HEIGHT };
	player->animationPlayer[DASH_DIAGONAL] = CreateAnimation(player->sprite, 4, 7, sfTrue, sfFalse, firstFrame);

	firstFrame = (sfIntRect){ 0, 4 * PLAYER_HEIGHT, PLAYER_WIDTH, PLAYER_HEIGHT };
	player->animationPlayer[SLIDE] = CreateAnimation(player->sprite, 4, 7, sfTrue, sfTrue, firstFrame);

	firstFrame = (sfIntRect){ 0, 5 * PLAYER_HEIGHT, PLAYER_WIDTH, PLAYER_HEIGHT };
	player->animationPlayer[CLIMB_WALL] = CreateAnimation(player->sprite, 6, 9, sfTrue, sfTrue, firstFrame);

	firstFrame = (sfIntRect){ 6 * PLAYER_WIDTH, 5 * PLAYER_HEIGHT, PLAYER_WIDTH, PLAYER_HEIGHT };
	player->animationPlayer[LADDER] = CreateAnimation(player->sprite, 6, 7, sfTrue, sfTrue, firstFrame);

	firstFrame = (sfIntRect){ 0, 6 * PLAYER_HEIGHT, PLAYER_WIDTH, PLAYER_HEIGHT };
	player->animationPlayer[WALL_GRIP_FALL] = CreateAnimation(player->sprite, 2, 4, sfTrue, sfTrue, firstFrame);

	firstFrame = (sfIntRect){ 2 * PLAYER_WIDTH, 6 * PLAYER_HEIGHT, PLAYER_WIDTH, PLAYER_HEIGHT };
	player->animationPlayer[WALL_JUMP] = CreateAnimation(player->sprite, 3, 9, sfTrue, sfFalse, firstFrame);

	firstFrame = (sfIntRect){ 0, 7 * PLAYER_HEIGHT, 48, PLAYER_HEIGHT };
	player->animationPlayer[AXE] = CreateAnimation(player->sprite, 10, 15, sfTrue, sfFalse, firstFrame);

	firstFrame = (sfIntRect){ 0, 8 * PLAYER_HEIGHT, 48, PLAYER_HEIGHT };
	player->animationPlayer[SWORD] = CreateAnimation(player->sprite, 4, 18, sfTrue, sfFalse, firstFrame);

	firstFrame = (sfIntRect){ 0, 9 * PLAYER_HEIGHT, 32, 48 };
	player->animationPlayer[SWORD_UP] = CreateAnimation(player->sprite, 4, 9, sfTrue, sfFalse, firstFrame);

	firstFrame = (sfIntRect){ 0, (10 * PLAYER_HEIGHT) + 16, 32, 48 };
	player->animationPlayer[SWORD_DOWN] = CreateAnimation(player->sprite, 4, 9, sfTrue, sfFalse, firstFrame);

	firstFrame = (sfIntRect){ 0, (11 * PLAYER_HEIGHT) + 32, 32, 48 };
	player->animationPlayer[AXE_UP] = CreateAnimation(player->sprite, 10, 15, sfTrue, sfFalse, firstFrame);

	firstFrame = (sfIntRect){ 0, (12 * PLAYER_HEIGHT) + 48, 32, 48 };
	player->animationPlayer[AXE_DOWN] = CreateAnimation(player->sprite, 10, 12, sfTrue, sfFalse, firstFrame);

	SetAnimation(IDLE);
}

void SetAnimation(PlayerState _state)
{
	player->lastState = player->currentState;
	player->currentAnimation = &player->animationPlayer[_state];
	player->currentAnimation->timer = 0.f;
	player->currentAnimation->isPlaying = sfTrue;
	player->currentAnimation->currentFrame = 0;
	player->currentState = _state;
}

void UpdatePlayer(sfRenderWindow* _renderWindow, float _dt)
{
	ApplyPhysic(_dt);
	MovePlayer(_renderWindow, _dt);
	CheckCollisionPlayerPlatforms(_dt);
	CheckCollisionPlayerSpike(_dt);
	CollisionPlayerTrigger();
	CheckCollisionPlayerAttackMob(_dt);
	CheckCollisionPlayerMob(_dt);
	CollisionPlayerDeathZone();

	CheckPlayerHP();
	UpdateAnimation(player->currentAnimation, _dt);
}


void CheckCollisionPlayerAttackMob(float _dt)
{
	if (sfKeyboard_isKeyPressed(sfKeyF))
	{
		player->data.timerInvincible = -2.f;
		player->action.isInvincible = sfTrue;
	}


	for (unsigned i = 0; i < GetMobCount(); i++)
	{
		if (mob[i].act != IS_DEATH)
		{
			if (mob[i].act == IS_ATTACK && mob[i].data.timerAttack > mob[i].data.timerAttackLimit)
			{
				StateMobMachine(ATTACK_MOB, i);

				if (mob[i].currentMobAnimation->currentFrame == mob[i].frameAttackSound)
				{
					sfSound_setPlayingOffset(mob[i].soundMob.soundAttack, sfSeconds(0.5f));
					sfSound_play(mob[i].soundMob.soundAttack);
				}

			}
			else if ((!mob[i].currentMobAnimation->isPlaying && mob[i].act == IS_ATTACK) || mob[i].act == IS_TAKE_HIT)
			{
				mob[i].data.timerAttack = 0;
				mob[i].act = IS_IDLE;
			}
		}
	}

	if (!player->action.isInvincible && player->data.timerInvincible > 0)
	{

		sfFloatRect hitPlayer = sfSprite_getGlobalBounds(player->sprite);
		sfFloatRect hitMob = { 0 };
		sfFloatRect intersection;
		for (unsigned i = 0; i < GetMobCount(); i++)
		{
			hitMob = sfRectangleShape_getGlobalBounds(mob[i].shape.rect);

			if (player->data.health > 0)
			{

				if (mob[i].currentState == ATTACK_MOB && player->data.timerInvincible > TIMER_INVINCIBLE)
				{
					if (mob[i].currentMobAnimation->currentFrame == mob[i].frameAttackSound && !player->action.damageEnable && sfFloatRect_intersects(&hitMob, &hitPlayer, &intersection))
					{
						player->data.timerInvincible = 0;
						player->action.damageEnable = sfTrue;

						player->data.health -= mob[i].damage + rand() % mob[i].damage;



					}
					else //if (player->data.timerInvincible )//!mob[i].currentMobAnimation->isPlaying)
					{
						player->action.damageEnable = sfFalse;
					}
				}



			}

		}


		//----MOB ATTACK INFLUENCED PLAYER----//


		PlayerSide playerSide = NOTHING_PLAYER;

		float mobCenterX = 0;
		float playerCenterX = hitPlayer.left + (hitPlayer.width / 2);

		for (unsigned i = 0; i < mobCount; i++)
		{
			hitMob = sfRectangleShape_getGlobalBounds(mob[i].shape.attackRect);
			mobCenterX = hitMob.left + (hitMob.width / 2);


			if (sfFloatRect_intersects(&hitMob, &hitPlayer, &intersection) && player->data.knockBackTimer <= 0)
			{
				if (mob[i].currentState == ATTACK_MOB && mob[i].currentMobAnimation->currentFrame == mob[i].frameAttackSound)
				{
					player->data.knockBackTimer += 0.4f;

					if (playerCenterX < mobCenterX)
					{
						playerSide = LEFT_PLAYER;
					}
					else if (playerCenterX > mobCenterX)
					{
						playerSide = WIDTH_PLAYER;
					}

				}

			}

		}


		if (playerSide != NOTHING_PLAYER)
		{
			player->action.isGrounded = sfFalse;
			StateMachine(FALL);
			player->data.velocity.y = -300.f;

			if (playerSide == LEFT_PLAYER)
			{
				player->data.velocity.x = -500.f;
			}
			else if (playerSide == WIDTH_PLAYER)
			{
				player->data.velocity.x = 500.f;
			}
		}
	}
}

void CheckCollisionPlayerMob(float _dt)
{

	player->data.timerInvincible += _dt;

	sfRectangleShape_setPosition(player->shape.rectCollisionPlayerMob, sfSprite_getPosition(player->sprite));
	player->shape.hitRectCollisionPlayerMob = sfRectangleShape_getGlobalBounds(player->shape.rectCollisionPlayerMob);

	player->data.timerPlayerMob += _dt;

	sfFloatRect hitPlayer = player->shape.hitRectCollisionPlayerMob;
	sfFloatRect hitMob;
	sfFloatRect intersection;
	float playerCenterX = hitPlayer.left + (hitPlayer.width * 0.5f);
	float mobCenterX;

	for (unsigned i = 0; i < GetMobCount(); i++)
	{
		hitMob = mob[i].shape.collisionMob;
		mobCenterX = hitMob.left + (hitMob.width * 0.5f);

		sfBool attackAxe = player->currentState == AXE || player->currentState == AXE_UP || player->currentState == AXE_DOWN;
		sfBool attackSword = player->currentState == SWORD || player->currentState == SWORD_UP || player->currentState == SWORD_DOWN;

		if (!attackAxe && !attackSword && !player->action.isInvincible && mob[i].currentState != DEATH)
		{
			if (sfFloatRect_intersects(&hitMob, &hitPlayer, &intersection))
			{
				player->data.timerPlayerMob = 0;
				player->data.knockBackTimer += 0.2f;
				player->action.isSlideJumping = sfFalse;
				player->action.isTouchingWall = sfFalse;
				player->action.isInvincible = sfTrue;
				player->data.health -= 15;

				if (playerCenterX > (mobCenterX)+PLAYER_MOB_MARGE)
				{
					player->side = LEFT_PLAYER;
				}
				else if (playerCenterX < (mobCenterX)-PLAYER_MOB_MARGE)
				{
					player->side = WIDTH_PLAYER;
				}

			}
		}

	}

	if (player->side != NOTHING_PLAYER)
	{
		player->action.isGrounded = sfFalse;
		player->data.velocity.y = -PLAYER_MOB_VELOCITY_Y;

		if (player->side == LEFT_PLAYER)
		{
			sfSprite_setScale(player->sprite, (sfVector2f) { GAME_SCALE, GAME_SCALE });
			player->data.velocity.x = PLAYER_MOB_VELOCITY_X;

		}
		else if (player->side == WIDTH_PLAYER)
		{
			sfSprite_setScale(player->sprite, (sfVector2f) { -GAME_SCALE, GAME_SCALE });
			player->data.velocity.x = -PLAYER_MOB_VELOCITY_X;
		}
		player->side = NOTHING_PLAYER;
	}

	if (player->data.timerInvincible > TIMER_INVINCIBLE)
	{
		player->action.isInvincible = sfFalse;
	}

}


void ApplyPhysic(float _dt)
{
	if (!player->action.isGrounded)
	{
		if (player->action.isDashing && player->currentState == DASH_GROUND)
		{
			player->data.velocity.y = 0;
			return;
		}



		player->data.velocity.y += GRAVITY * _dt;

		if (player->data.velocity.y > MAX_FALL_SPEED)
		{
			player->data.velocity.y = MAX_FALL_SPEED;
		}
	}
	else
	{
		player->data.velocity.y = 50.f;
	}
}
void createCollisionSideAttack()
{
	float width = ATTACK_HITBOX_WIDTH * GAME_SCALE;
	float height = ATTACK_HITBOX_HEIGHT * GAME_SCALE;

	sfRectangleShape_setSize(player->shape.collisionAttackShape, (sfVector2f) { width, height });
	sfRectangleShape_setFillColor(player->shape.collisionAttackShape, sfColor_fromRGBA(255, 0, 0, 150));

	sfFloatRect p = player->shape.collisionPlayerRect;

	float y = p.top + (p.height / 2.f) - (height / 2.f);

	if (player->data.lastDirection == -1) // gauche
	{
		sfRectangleShape_setPosition(player->shape.collisionAttackShape, (sfVector2f) { p.left - width, y });
	}
	else // droite
	{
		sfRectangleShape_setPosition(player->shape.collisionAttackShape, (sfVector2f) { p.left + p.width, y });
	}

	player->shape.collisionAttackRect = sfRectangleShape_getGlobalBounds(player->shape.collisionAttackShape);
}
void createCollisionUpAttack()
{
	sfRectangleShape_setSize(player->shape.collisionAttackShape, (sfVector2f) { player->shape.collisionPlayerRect.width, player->shape.collisionPlayerRect.height + 20 });
	sfRectangleShape_setFillColor(player->shape.collisionAttackShape, sfColor_fromRGBA(0, 255, 0, 150));
	player->shape.collisionAttackRect = sfRectangleShape_getGlobalBounds(player->shape.collisionAttackShape);
	if (player->data.lastDirection == 1)
	{
		sfRectangleShape_setPosition(player->shape.collisionAttackShape, (sfVector2f) { player->shape.collisionPlayerRect.left + 10, player->shape.collisionPlayerRect.top - 40 });
	}
	else if (player->data.lastDirection == -1)
	{
		sfRectangleShape_setPosition(player->shape.collisionAttackShape, (sfVector2f) { player->shape.collisionPlayerRect.left - 10, player->shape.collisionPlayerRect.top - 40 });

	}
}

void createCollisionDownAttack()
{
	sfFloatRect p = player->shape.collisionPlayerRect;

	float width = player->shape.collisionPlayerRect.width + 5 * GAME_SCALE;
	float height = player->shape.collisionPlayerRect.height;

	sfRectangleShape_setSize(player->shape.collisionAttackShape, (sfVector2f) { width, height });

	sfVector2f pos = { p.left + (p.width * 0.5f) - (width * 0.5f) ,p.top };

	sfRectangleShape_setPosition(player->shape.collisionAttackShape, pos);

	player->shape.collisionAttackRect = sfRectangleShape_getGlobalBounds(player->shape.collisionAttackShape);
}

void ApplyHorizontalInput(sfRenderWindow* _renderWindow, sfBool movingLeft, sfBool movingRight)
{
	if (sfRenderWindow_hasFocus(_renderWindow))
	{
		if (!player->action.isDashing)
		{
			if (movingRight)
			{
				player->data.velocity.x = player->data.speed;
				player->data.lastDirection = 1;
				player->action.isMoving = sfTrue;
				sfSprite_setScale(player->sprite, (sfVector2f) { GAME_SCALE, GAME_SCALE });
			}
			else if (movingLeft)
			{
				player->data.velocity.x = -player->data.speed;
				player->data.lastDirection = -1;
				player->action.isMoving = sfTrue;
				sfSprite_setScale(player->sprite, (sfVector2f) { -GAME_SCALE, GAME_SCALE });
			}

			else
			{
				player->data.velocity.x = 0;
				player->action.isMoving = sfFalse;
			}
		}
	}

}


void HandleAttackInput(sfRenderWindow* _renderWindow, float _dt, sfBool movingLeft, sfBool movingRight)
{
	player->data.attackCooldownTimer += _dt;
	player->data.timeAttackSide += _dt;

	if (sfMouse_isButtonPressed(sfMouseLeft) &&
		player->data.attackCooldownTimer >= ATTACK_AXE_COOLDOWN &&
		player->action.isGrounded)
	{

		sfVector2i mousePos = sfMouse_getPositionRenderWindow(_renderWindow);
		sfBool attackTriggered = sfFalse;

		if (mousePos.y > SCREEN_HEIGHT / 2 + 200)
		{
			player->data.velocity.x = 0;
			StateMachine(AXE_DOWN);
			attackTriggered = sfTrue;
		}
		else if (mousePos.y < SCREEN_HEIGHT / 2 - 200)
		{
			player->data.velocity.x = 0;
			StateMachine(AXE_UP);
			attackTriggered = sfTrue;
		}
		else
		{
			player->data.velocity.x = 0;
			sfSound_setPitch(player->sound.axeSound, RandomFloat(0.8f, 1.2f));
			sfSound_play(player->sound.axeSound);
			StateMachine(AXE);
			attackTriggered = sfTrue;
		}


		if (attackTriggered)
		{
			player->action.isAttacking = sfTrue;
			player->data.attackCooldownTimer = 0.f;
			return;
		}
	}

	if (sfMouse_isButtonPressed(sfMouseRight) &&
		player->data.attackCooldownTimer >= ATTACK_SWORD_COOLDOWN)
	{
		sfVector2i mousePos = sfMouse_getPositionRenderWindow(_renderWindow);
		sfBool attackTriggered = sfFalse;

		if (!player->action.isGrounded && mousePos.y > SCREEN_HEIGHT / 2 + 200 && player->data.velocity.y > 0)
		{
			float playerBottom = player->shape.collisionPlayerRect.top + player->shape.collisionPlayerRect.height;
			sfBool farEnough = sfFalse;

			for (unsigned i = 0; i < GetCollisionTabSize(); i++)
			{
				sfFloatRect platform = GetMapCollision(i);
				if (player->data.position.x > platform.left &&
					player->data.position.x < platform.left + platform.width &&
					platform.top > playerBottom)
				{
					if (platform.top - playerBottom > 150.f)
					{
						farEnough = sfTrue;
					}
					break;
				}
			}

			if (farEnough)
			{
				StateMachine(SWORD_DOWN);
				attackTriggered = sfTrue;
			}
		}
		else if (player->action.isGrounded)
		{
			if (mousePos.y < SCREEN_HEIGHT / 2 - 200)
			{
				StateMachine(SWORD_UP);
				attackTriggered = sfTrue;
			}
			else
			{
				sfSound_setPitch(player->sound.swordSound, RandomFloat(0.9f, 1.2f));
				sfSound_play(player->sound.swordSound);
				StateMachine(SWORD);
				attackTriggered = sfTrue;
			}
		}

		if (attackTriggered)
		{
			player->action.isAttacking = sfTrue;
			player->data.attackCooldownTimer = 0.f;
			return;
		}
	}

	if (player->currentState == SWORD_DOWN)
	{
		if (player->action.isGrounded)
		{
			player->action.isAttacking = sfFalse;
			StateMachine(player->action.isMoving ? RUN : IDLE);
		}
	}
}

sfBool HandleAttackState(sfRenderWindow* _renderWindow, sfBool movingLeft, sfBool movingRight)
{
	if (!player->action.isAttacking || player->data.timeAttackSide < TIMER_ATTACK_SIDE)
		return sfFalse;

	if (player->shape.collisionAttackShape != NULL)
	{
		sfFloatRect pRect = player->shape.collisionPlayerRect;
		sfVector2f attackPos = { 0 };

		if (player->currentState == SWORD_DOWN)
		{
			sfVector2f sizeDown = { player->shape.collisionPlayerRect.width + 20, player->shape.collisionPlayerRect.height + 20 };
			sfRectangleShape_setSize(player->shape.collisionAttackShape, sizeDown);

			attackPos.x = pRect.left - 5;
			attackPos.y = pRect.top - 5;
		}
		else if (player->currentState == SWORD_UP)
		{
			if (player->currentAnimation->currentFrame >= 1)
			{
				sfVector2f sizeUp = { player->shape.collisionPlayerRect.width - 40, player->shape.collisionPlayerRect.height };
				sfRectangleShape_setSize(player->shape.collisionAttackShape, sizeUp);

				attackPos.x = player->shape.collisionPlayerRect.left + player->shape.collisionPlayerRect.width - 20;
				attackPos.y = player->shape.collisionPlayerRect.top;
			}
			else
			{
				sfRectangleShape_setSize(player->shape.collisionAttackShape, (sfVector2f) { 0.f, 0.f });
			}
		}
		else if (player->currentState == SWORD)
		{
			if (player->currentAnimation->currentFrame >= 2)
			{
				sfVector2f sizeSide = { 60, 20 };
				sfRectangleShape_setSize(player->shape.collisionAttackShape, sizeSide);

				attackPos.x = player->shape.collisionPlayerRect.left + player->shape.collisionPlayerRect.width;
				attackPos.y = player->shape.collisionPlayerRect.top + player->shape.collisionPlayerRect.height / 2 - 14;
			}
			else
			{
				sfRectangleShape_setSize(player->shape.collisionAttackShape, (sfVector2f) { 0.f, 0.f });
			}

		}

		else if (player->currentState == AXE)
		{
			if (player->currentAnimation->currentFrame >= 5)
			{
				sfVector2f size = { player->shape.collisionPlayerRect.width + 20.f, player->shape.collisionPlayerRect.height };
				sfRectangleShape_setSize(player->shape.collisionAttackShape, size);
				attackPos.x = player->shape.collisionPlayerRect.left - 10.f;
				attackPos.y = player->shape.collisionPlayerRect.top;
			}
			else
			{
				sfRectangleShape_setSize(player->shape.collisionAttackShape, (sfVector2f) { 0.f, 0.f });
			}
		}
		else if (player->currentState == AXE_UP)
		{
			if (player->currentAnimation->currentFrame >= 4)
			{
				sfVector2f size = { player->shape.collisionPlayerRect.width + 20.f, player->shape.collisionPlayerRect.height + 20.f };
				sfRectangleShape_setSize(player->shape.collisionAttackShape, size);
				attackPos.x = player->shape.collisionPlayerRect.left - 10.f;
				attackPos.y = player->shape.collisionPlayerRect.top - 20.f;
			}
			else
			{
				sfRectangleShape_setSize(player->shape.collisionAttackShape, (sfVector2f) { 0.f, 0.f });
			}
		}
		else if (player->currentState == AXE_DOWN)
		{
			if (player->currentAnimation->currentFrame >= 5)
			{
				sfVector2f size = { player->shape.collisionPlayerRect.width + 30.f, player->shape.collisionPlayerRect.height + 20.f };
				sfRectangleShape_setSize(player->shape.collisionAttackShape, size);
				attackPos.x = player->shape.collisionPlayerRect.left - 15.f;
				attackPos.y = player->shape.collisionPlayerRect.top + 20.f;
			}
			else
			{
				sfRectangleShape_setSize(player->shape.collisionAttackShape, (sfVector2f) { 0.f, 0.f });
			}
		}
		if (player->data.lastDirection == 1)
		{
			switch (player->currentState)
			{
			case SWORD:
				attackPos.x = player->shape.collisionPlayerRect.left + player->shape.collisionPlayerRect.width;
				attackPos.y = player->shape.collisionPlayerRect.top + player->shape.collisionPlayerRect.height / 2 - 14;
				break;

			case SWORD_DOWN:
				attackPos.x = pRect.left - 5;
				attackPos.y = pRect.top - 5;
				break;

			case SWORD_UP:
				attackPos.x = pRect.left + pRect.width - 20;
				attackPos.y = pRect.top;
				break;

			default:
				break;
			}
		}
		else if (player->data.lastDirection == -1)
		{
			switch (player->currentState)
			{
			case SWORD:
				attackPos.x = player->shape.collisionPlayerRect.left - 60;
				attackPos.y = player->shape.collisionPlayerRect.top + player->shape.collisionPlayerRect.height / 2 - 14;
				break;

			case SWORD_DOWN:
				attackPos.x = pRect.left - 5;
				attackPos.y = pRect.top - 5;
				break;

			case SWORD_UP:
				attackPos.x = pRect.left - 20;
				attackPos.y = pRect.top;
				break;
			default:
				break;
			}
		}
		sfRectangleShape_setPosition(player->shape.collisionAttackShape, attackPos);
		player->shape.collisionAttackRect = sfRectangleShape_getGlobalBounds(player->shape.collisionAttackShape);
	}

	if (player->currentState == AXE)
	{
		player->data.velocity.x = 300.f * player->data.lastDirection;
	}
	else if (player->currentState == SWORD || player->currentState == SWORD_UP || player->currentState == SWORD_DOWN)
	{
		float horizontalInput = 0.f;
		if (movingRight && player->data.lastDirection == 1)
		{
			horizontalInput = player->data.speed;
		}
		else if (movingLeft && player->data.lastDirection == -1)
		{
			horizontalInput = -player->data.speed;
		}

		player->data.velocity.x = (player->currentState == SWORD_UP) ? 0.f : horizontalInput;
		player->action.isMoving = (horizontalInput != 0.f);
	}

	if (player->currentAnimation->currentFrame >= player->currentAnimation->frameCount - 1)
	{
		player->action.isAttacking = sfFalse;
		StateMachine(player->action.isGrounded ? (player->action.isMoving ? RUN : IDLE) : FALL);
	}

	return sfTrue;
}
void HandleWallJumping(float _dt, sfBool movingLeft, sfBool movingRight)
{
	float sign = (player->data.wallJumpVelocityX > 0) ? 1.f : -1.f;
	player->data.wallJumpVelocityX -= sign * WALL_JUMP_FRICTION * _dt;

	if (sign > 0 && player->data.wallJumpVelocityX < MIN_WALL_JUMP_SPEED)
	{
		player->data.wallJumpVelocityX = MIN_WALL_JUMP_SPEED;
	}
	if (sign < 0 && player->data.wallJumpVelocityX > -MIN_WALL_JUMP_SPEED)
	{
		player->data.wallJumpVelocityX = -MIN_WALL_JUMP_SPEED;
	}

	float inputVelocity = movingRight ? player->data.speed : (movingLeft ? -player->data.speed : 0);
	player->data.velocity.x = player->data.wallJumpVelocityX + inputVelocity * 0.3f;

	if (player->action.isGrounded)
	{
		player->action.isWallJumping = sfFalse;
	}
}

void HandleSlideJumping(float _dt, sfBool movingLeft, sfBool movingRight)
{
	float sign = (player->data.slideVelocityX > 0) ? 1.f : -1.f;
	player->data.slideVelocityX -= sign * SLIDE_FRICTION * _dt;

	if (sign > 0 && player->data.slideVelocityX < MIN_SLIDE_JUMP_SPEED)
	{
		player->data.slideVelocityX = MIN_SLIDE_JUMP_SPEED;
	}
	if (sign < 0 && player->data.slideVelocityX > -MIN_SLIDE_JUMP_SPEED)
	{
		player->data.slideVelocityX = -MIN_SLIDE_JUMP_SPEED;
	}

	float inputVelocity = movingRight ? player->data.speed : (movingLeft ? -player->data.speed : 0);
	player->data.velocity.x = player->data.slideVelocityX + inputVelocity * 0.4f;

	if (player->action.isGrounded)
	{
		player->action.isSlideJumping = sfFalse;
	}
}

void HandleSliding(float _dt, sfBool movingLeft, sfBool movingRight, sfBool slideKey)
{
	player->data.slideTimer -= _dt;

	float sign = (player->data.lastDirection > 0) ? 1.f : -1.f;
	player->data.slideVelocityX -= sign * SLIDE_FRICTION * _dt;

	if (sign > 0 && player->data.slideVelocityX < 0)
	{
		player->data.slideVelocityX = 0;
	}
	if (sign < 0 && player->data.slideVelocityX > 0)
	{
		player->data.slideVelocityX = 0;
	}

	float inputVelocity = 0;
	if (movingRight)
	{
		inputVelocity = player->data.speed;
		player->data.lastDirection = 1;
		sfSprite_setScale(player->sprite, (sfVector2f) { GAME_SCALE, GAME_SCALE });
	}
	else if (movingLeft)
	{
		inputVelocity = -player->data.speed;
		player->data.lastDirection = -1;
		sfSprite_setScale(player->sprite, (sfVector2f) { -GAME_SCALE, GAME_SCALE });
	}

	player->data.velocity.x = player->data.slideVelocityX + inputVelocity * 0.5f;

	if (player->data.slideTimer <= 0 || !slideKey)
	{
		player->action.isSliding = sfFalse;
		player->data.slideVelocityX = 0;
		player->data.slideCooldownTimer = SLIDE_COOLDOWN;
		StateMachine(player->action.isMoving ? RUN : IDLE);
	}
}

void HandleGroundMovement(sfRenderWindow* _renderWindow, sfBool movingLeft, sfBool movingRight, sfBool slideKey)
{

	ApplyHorizontalInput(_renderWindow, movingLeft, movingRight);

	if (slideKey && player->action.isGrounded && !player->action.isSliding && player->data.slideCooldownTimer <= 0.f)
	{
		player->action.isSliding = sfTrue;
		player->data.slideTimer = SLIDE_DURATION;
		player->data.slideVelocityX = player->data.lastDirection * SLIDE_SPEED;
		StateMachine(SLIDE);
	}
}

static void HandleDoubleJump(float _dt, sfBool _spaceTouching)
{
	if (_spaceTouching && !player->action.isGrounded && player->data.doubleJumpUnlocked)
	{
		StateMachine(D_JUMP);
		player->data.velocity.y = -550.f;
		player->action.jumpOne = sfFalse;
		player->action.jumpTwo = sfTrue;
	}
	else if (player->action.jumpTwo && player->data.velocity.y > 0)
	{
		StateMachine(FALL);
		player->action.jumpTwo = sfFalse;
	}

}

static void HandleJump(float _dt, sfBool movingLeft, sfBool movingRight, sfBool jumpKey)
{
	static sfBool jumpPressed = sfFalse;

	if (jumpKey && !jumpPressed && !player->action.isDashing)
	{
		jumpPressed = sfTrue;
		player->data.jumpStartPosition = player->data.position.y;

		if (player->action.isGrounded)
		{
			player->action.justWallJumped = sfFalse;

			if (player->action.isSliding)
			{
				player->action.isSlideJumping = sfTrue;
				player->data.slideVelocityX = player->data.velocity.x;
				player->action.isSliding = sfFalse;
				player->data.slideCooldownTimer = SLIDE_COOLDOWN;
			}

			player->data.velocity.y = -JUMP_FORCE;
			player->action.isGrounded = sfFalse;
			if (!player->action.jumpOne && !player->action.jumpTwo && player->currentState != D_JUMP)
			{
				StateMachine(JUMP);
			}

			player->action.jumpOne = sfTrue;
		}
		else
		{
			float dx = player->data.velocity.x * _dt;
			if (CheckCollisionPlayerPlatformsX(dx) && !player->action.isDashing)
			{
				player->data.currentWallTouched = player->action.isTouchingRightWall ? 1.f : -1.f;

				float wallJumpHX = 550.f;

				if (player->action.isTouchingRightWall && movingRight)
				{
					player->action.isWallJumping = sfTrue;
					player->action.justWallJumped = sfTrue;
					player->data.wallJumpVelocityX = -wallJumpHX;
					player->data.lastDirection = -1;
					player->data.velocity.y = -JUMP_FORCE * 0.75f;
					sfSprite_setScale(player->sprite, (sfVector2f) { -GAME_SCALE, GAME_SCALE });
				}
				else if (player->action.isTouchingLeftWall && movingLeft)
				{
					player->action.isWallJumping = sfTrue;
					player->action.justWallJumped = sfTrue;
					player->data.wallJumpVelocityX = wallJumpHX;
					player->data.lastDirection = 1;
					player->data.velocity.y = -JUMP_FORCE * 0.75f;
					sfSprite_setScale(player->sprite, (sfVector2f) { GAME_SCALE, GAME_SCALE });
				}

				player->action.isGrounded = sfFalse;
				player->action.isSlideJumping = sfFalse;
				StateMachine(WALL_JUMP);
			}
			else if (player->action.jumpOne)
			{
				HandleDoubleJump(_dt, jumpKey);
			}
		}



	}

	if (!jumpKey)
	{
		jumpPressed = sfFalse;

	}
}

static void HandleDash(float _dt, sfBool _dashHorizontal, sfBool _dashUp, sfBool _dashDiagonal)
{
	player->data.timerDash += _dt;

	sfBool dashEnable = _dashDiagonal || _dashUp || _dashHorizontal;
	float playerScale = sfSprite_getScale(player->sprite).x;
	PlayerState state = player->currentState;

	if (dashEnable && player->data.timerDash > TIMER_DASH && !player->action.isDashing && player->data.dashUnlocked && !player->action.isAttacking)
	{
		sfBool validDash = sfFalse;
		PlayerState state = player->currentState;
		float playerScale = sfSprite_getScale(player->sprite).x;

		if (_dashDiagonal && player->data.diagonalDashUnlocked)
		{
			player->data.dashVelocityX = (playerScale < 0) ? -550.f : 550.f;
			player->data.dashVelocityY = -550.f;
			state = DASH_DIAGONAL;
			validDash = sfTrue;
		}
		else if (_dashUp && player->data.upDashUnlocked)
		{
			player->data.dashVelocityX = 0.f;
			player->data.dashVelocityY = -DASH_Y;
			state = DASH_UP;
			validDash = sfTrue;
		}
		else if (_dashHorizontal && player->data.horizontalDashUnlocked)
		{
			player->data.dashVelocityX = (playerScale < 0) ? -950.f : 950.f;
			player->data.dashVelocityY = 0.f;
			state = DASH_GROUND;
			validDash = sfTrue;
		}

		if (validDash)
		{
			player->action.isDashing = sfTrue;
			player->data.timerDash = 0.f;
			player->data.velocity.x = 0.f;
			player->data.velocity.y = 0.f;
			player->action.isWallJumping = sfFalse;
			player->action.isSlideJumping = sfFalse;
			StateMachine(state);
		}
	}

	if (player->action.isDashing)
	{
		float friction = 1.f;

		float decay = expf(-friction * _dt);
		player->data.dashVelocityX *= decay;
		player->data.dashVelocityY *= decay;

		if (fabsf(player->data.dashVelocityX) < 10.f)
		{
			player->data.dashVelocityX = 0.f;
		}
		if (fabsf(player->data.dashVelocityY) < 10.f)
		{
			player->data.dashVelocityY = 0.f;
		}

		player->data.velocity.x = player->data.dashVelocityX;
		if (_dashUp || _dashDiagonal)
		{
			player->data.velocity.y = player->data.dashVelocityY;
		}

		if (!player->currentAnimation->isPlaying)
		{
			player->data.knockBackTimer = 0.f;
			player->action.isDashing = sfFalse;
			player->data.dashVelocityX = 0.f;
			player->data.dashVelocityY = 0.f;
		}
	}
}



void HandleAirAnimation(float _dt, sfBool movingLeft, sfBool movingRight)
{
	float dx = player->data.velocity.x * _dt;
	CheckCollisionPlayerPlatformsX(dx);

	if (player->data.velocity.y < 0 && !player->action.isDashing)
	{
		if (movingLeft && player->action.isTouchingLeftWall && player->currentState == JUMP)
		{
			float fallen = player->data.position.y - player->data.jumpStartPosition;
			if (-fallen > MIN_WALL_GRIP_DISTANCE)
			{
				player->data.velocity.y = 0;
				StateMachine(WALL_GRIP_FALL);
			}
		}
		else if (movingRight && player->action.isTouchingRightWall && player->currentState == JUMP)
		{
			float fallen = player->data.position.y - player->data.jumpStartPosition;
			if (-fallen > MIN_WALL_GRIP_DISTANCE)
			{
				player->data.velocity.y = 0;
				StateMachine(WALL_GRIP_FALL);
			}
		}

		if (player->currentState != JUMP && player->currentState != WALL_JUMP && player->currentState != D_JUMP)
		{
			StateMachine(JUMP);
		}
	}
	else
	{
		if (!player->action.isDashing)
		{
			if (movingLeft && player->action.isTouchingLeftWall)
			{
				player->data.velocity.y -= 600.f * _dt;
				if (player->data.velocity.y >= MAX_GRIP_WALL_SPEED)
					player->data.velocity.y = MAX_GRIP_WALL_SPEED;
				StateMachine(WALL_GRIP_FALL);
			}
			else if (movingRight && player->action.isTouchingRightWall)
			{
				player->data.velocity.y -= 600.f * _dt;
				if (player->data.velocity.y >= MAX_GRIP_WALL_SPEED)
					player->data.velocity.y = MAX_GRIP_WALL_SPEED;
				StateMachine(WALL_GRIP_FALL);
			}
			else if (!player->action.isDashing)
			{
				StateMachine(FALL);
			}
		}
	}
}

void HandleAnimationState(float _dt, sfBool movingLeft, sfBool movingRight)
{
	if (player->action.isGrounded && !player->action.isSliding && !player->action.isDashing)
	{
		if (player->currentState != TURN)
			StateMachine(player->action.isMoving ? RUN : IDLE);

		if (player->currentState == TURN &&
			player->currentAnimation->currentFrame >= player->currentAnimation->frameCount - 1)
			StateMachine(player->action.isMoving ? RUN : IDLE);
	}
	else if (!player->action.isGrounded)
	{
		HandleAirAnimation(_dt, movingLeft, movingRight);
	}
}


void MovePlayer(sfRenderWindow* _renderWindow, float _dt)
{
	sfBool movingLeft = sfKeyboard_isKeyPressed(sfKeyQ);
	sfBool movingRight = sfKeyboard_isKeyPressed(sfKeyD);
	sfBool slideKey = sfKeyboard_isKeyPressed(sfKeyLControl) || sfKeyboard_isKeyPressed(sfKeyRControl);
	sfBool jumpKey = sfKeyboard_isKeyPressed(sfKeySpace);
	sfBool dashGround = sfKeyboard_isKeyPressed(sfKeyLShift) && (movingLeft || movingRight);
	sfBool dashUp = sfKeyboard_isKeyPressed(sfKeyZ) && sfKeyboard_isKeyPressed(sfKeyLShift);
	sfBool dashDiagonal = dashUp && (movingLeft || movingRight);

	HandleDash(_dt, dashGround, dashUp, dashDiagonal);

	if (player->data.slideCooldownTimer > 0.f)
	{
		player->data.slideCooldownTimer -= _dt;

	}
	if (sfRenderWindow_hasFocus(_renderWindow))
	{
		HandleAttackInput(_renderWindow, _dt, movingLeft, movingRight);
	}

	if (HandleAttackState(_renderWindow, movingLeft, movingRight))
	{
		return;

	}

	if (player->action.isWallJumping)
		HandleWallJumping(_dt, movingLeft, movingRight);
	else if (player->action.isSlideJumping)
	{
		HandleSlideJumping(_dt, movingLeft, movingRight);

	}
	else if (player->action.isSliding)
	{
		HandleSliding(_dt, movingLeft, movingRight, slideKey);

	}
	else
	{
		HandleGroundMovement(_renderWindow, movingLeft, movingRight, slideKey);
	}


	//HandleDash(_dt, dashGround, dashUp, dashDiagonal);
	HandleJump(_dt, movingLeft, movingRight, jumpKey);



	HandleAnimationState(_dt, movingLeft, movingRight);
}


void CollisionPlayerPlatformsX(float _dx)
{
	sfBool sKey = sfKeyboard_isKeyPressed(sfKeyS);
	player->action.isTouchingLeftWall = sfFalse;
	player->action.isTouchingRightWall = sfFalse;

	float playerHalfWidth = (PLAYER_HITBOX_WIDTH * GAME_SCALE) / 2.f;
	float playerWidth, playerHeight;
	GetPlayerHitboxSize(&playerWidth, &playerHeight);

	sfFloatRect hitbox = { player->data.position.x - playerHalfWidth + _dx, player->data.position.y - playerHeight, playerWidth, playerHeight };

	for (unsigned i = 0; i < GetCollisionTabSize(); i++)
	{
		sfFloatRect platform = GetMapCollision(i);
		if (sfFloatRect_intersects(&hitbox, &platform, NULL))
		{
			if (player->data.velocity.x > 0)
			{
				hitbox.left = platform.left - hitbox.width;
			}
			else if (player->data.velocity.x < 0)
			{
				hitbox.left = platform.left + platform.width;
			}

			if (!player->action.justWallJumped)
			{
				player->data.velocity.x = 0;
				player->data.position.x = hitbox.left + hitbox.width / 2.f;
			}

			sfSprite_setPosition(player->sprite, player->data.position);
			sfRectangleShape_setPosition(player->shape.collisionPlayerShape, (sfVector2f) { hitbox.left, hitbox.top });
			player->shape.collisionPlayerRect = sfRectangleShape_getGlobalBounds(player->shape.collisionPlayerShape);
			player->shape.playerRect = sfSprite_getGlobalBounds(player->sprite);
			return;
		}
	}


	player->action.justWallJumped = sfFalse;
	player->data.position.x += _dx;
	sfSprite_setPosition(player->sprite, player->data.position);
	sfRectangleShape_setPosition(player->shape.collisionPlayerShape, (sfVector2f) { player->data.position.x - playerHalfWidth, player->data.position.y - playerHeight });
	player->shape.collisionPlayerRect = sfRectangleShape_getGlobalBounds(player->shape.collisionPlayerShape);
	player->shape.playerRect = sfSprite_getGlobalBounds(player->sprite);
}
void CollisionPlayerPlatformsY(float _dy)
{
	sfBool sKey = sfKeyboard_isKeyPressed(sfKeyS);
	float playerHalfWidth = (PLAYER_HITBOX_WIDTH * GAME_SCALE) / 2.f;
	float playerWidth, playerHeight;
	GetPlayerHitboxSize(&playerWidth, &playerHeight);

	float previousBottom = player->data.position.y;

	sfFloatRect hitbox = { player->data.position.x - playerHalfWidth, player->data.position.y - playerHeight + _dy,playerWidth,playerHeight };
	player->action.isGrounded = sfFalse;

	for (unsigned i = 0; i < GetCollisionTabSize(); i++)
	{
		sfFloatRect platform = GetMapCollision(i);
		if (sfFloatRect_intersects(&hitbox, &platform, NULL))
		{
			if (player->data.velocity.y > 0)
			{
				hitbox.top = platform.top - hitbox.height;
				player->action.isGrounded = sfTrue;

				player->action.justWallJumped = sfFalse;
			}
			else if (player->data.velocity.y < 0)
			{
				hitbox.top = platform.top + platform.height;
			}

			player->data.velocity.y = 0;
			player->data.position.y = hitbox.top + hitbox.height;
			sfSprite_setPosition(player->sprite, player->data.position);
			sfRectangleShape_setPosition(player->shape.collisionPlayerShape, (sfVector2f) { hitbox.left, hitbox.top });
			player->shape.collisionPlayerRect = sfRectangleShape_getGlobalBounds(player->shape.collisionPlayerShape);
			player->shape.playerRect = sfSprite_getGlobalBounds(player->sprite);
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
		sfBool isFalling = player->data.velocity.y > 0;

		if (isFalling && wasAbove && !sKey)
		{
			hitbox.top = semi.top - hitbox.height;
			player->action.isGrounded = sfTrue;

			player->action.justWallJumped = sfFalse;


			player->data.velocity.y = 0;
			player->data.position.y = hitbox.top + hitbox.height;
			sfSprite_setPosition(player->sprite, player->data.position);
			sfRectangleShape_setPosition(player->shape.collisionPlayerShape, (sfVector2f) { hitbox.left, hitbox.top });
			player->shape.collisionPlayerRect = sfRectangleShape_getGlobalBounds(player->shape.collisionPlayerShape);
			player->shape.playerRect = sfSprite_getGlobalBounds(player->sprite);
			return;
		}
	}

	player->data.position.y += _dy;
	sfSprite_setPosition(player->sprite, player->data.position);
	sfRectangleShape_setPosition(player->shape.collisionPlayerShape, (sfVector2f) { player->data.position.x - playerHalfWidth, player->data.position.y - playerHeight });
	player->shape.collisionPlayerRect = sfRectangleShape_getGlobalBounds(player->shape.collisionPlayerShape);
	player->shape.playerRect = sfSprite_getGlobalBounds(player->sprite);
}
void CheckCollisionPlayerPlatforms(float _dt)
{
	float dx = player->data.velocity.x * _dt;
	CollisionPlayerPlatformsX(dx);

	float dy = player->data.velocity.y * _dt;
	CollisionPlayerPlatformsY(dy);

	sfRectangleShape_setPosition(player->shape.collisionPlayerShape, (sfVector2f) { player->data.position.x - (PLAYER_HITBOX_WIDTH * GAME_SCALE) / 2.f, player->data.position.y - (PLAYER_HITBOX_HEIGHT * GAME_SCALE) });
	player->shape.collisionPlayerRect = sfRectangleShape_getGlobalBounds(player->shape.collisionPlayerShape);
	player->shape.playerRect = sfSprite_getGlobalBounds(player->sprite);
}
sfBool CheckCollisionPlayerPlatformsX(float _dx)
{
	player->action.isTouchingLeftWall = sfFalse;
	player->action.isTouchingRightWall = sfFalse;

	float playerHalfWidth = (PLAYER_HITBOX_WIDTH * GAME_SCALE) / 2.f;
	float playerWidth, playerHeight;
	GetPlayerHitboxSize(&playerWidth, &playerHeight);

	sfFloatRect hitbox = { player->data.position.x - playerHalfWidth + _dx, player->data.position.y - playerHeight, playerWidth, playerHeight };

	for (unsigned i = 0; i < GetCollisionTabSize(); i++)
	{
		sfFloatRect platform = GetMapCollision(i);
		if (sfFloatRect_intersects(&hitbox, &platform, NULL))
		{
			float playerCenterX = player->data.position.x;
			float platformCenterX = platform.left + platform.width * 0.5f;

			if (playerCenterX < platformCenterX)
			{
				player->action.isTouchingRightWall = sfTrue;
			}
			else
			{
				player->action.isTouchingLeftWall = sfTrue;
			}

			return sfTrue;
		}
	}
	return sfFalse;
}


void CheckCollisionPlayerSpike(float _dt)
{
	player->data.timerSpikeWidth += _dt;
	player->data.timerSpikeHeight += _dt;
	sfFloatRect hitPlayer = player->shape.hitRectCollisionPlayerMob;
	sfFloatRect hitSpike = { 0 };
	sfFloatRect intersection = { 0 };
	float playerCenterX = hitPlayer.left + (hitPlayer.width / 2);


	for (unsigned i = 0; i < GetSpikeTabSize(); i++)
	{
		hitSpike = GetSpikeTab(i);
		float spikeCenterX = hitSpike.left + (hitSpike.width / 2);
		if (sfFloatRect_intersects(&hitSpike, &hitPlayer, &intersection) && player->data.timerSpikeWidth > TIMER_SPIKE)
		{
			player->data.knockBackTimer += 0.15f;
			if (playerCenterX < spikeCenterX && player->data.position.y >(hitSpike.top + (hitSpike.height / 2)))
			{
				player->spikeSide = WIDTH;
			}
			else if (playerCenterX > spikeCenterX && player->data.position.y > (hitSpike.top + (hitSpike.height / 2)))
			{

				player->spikeSide = LEFT;
			}
		}
	}

	if (player->spikeSide != NOTHING)
	{
		player->action.isGrounded = sfFalse;

		StateMachine(FALL);
		if (player->spikeSide == LEFT)
		{
			player->data.velocity.y = -SPIKE_VELOCITY;
			player->data.velocity.x = SPIKE_VELOCITY;
			sfSprite_setScale(player->sprite, (sfVector2f) { GAME_SCALE, GAME_SCALE });
		}
		else if (player->spikeSide == WIDTH)
		{
			player->data.velocity.y = -SPIKE_VELOCITY;
			player->data.velocity.x = -SPIKE_VELOCITY;
			sfSprite_setScale(player->sprite, (sfVector2f) { -GAME_SCALE, GAME_SCALE });
		}

		player->data.timerSpikeHeight = 0.f;
		player->data.timerSpikeWidth = 0.f;
		player->data.health -= 20;
		player->spikeSide = NOTHING;
	}


	float playerCenterY = hitPlayer.top + (hitPlayer.height / 2);
	float spikeCenterY = 0;


	for (unsigned i = 0; i < GetSpikeTabSize(); i++)
	{
		hitSpike = GetSpikeTab(i);
		spikeCenterY = hitSpike.top + (hitSpike.height / 2);


		if (sfFloatRect_intersects(&hitSpike, &hitPlayer, &intersection) && player->data.timerSpikeHeight > TIMER_SPIKE)
		{
			player->data.knockBackTimer += _dt;
			if (playerCenterY < spikeCenterY && (hitPlayer.left < (hitSpike.left + hitSpike.width)) && (hitPlayer.left + hitPlayer.width) > hitSpike.left)
			{
				player->spikeSide = TOP;
			}
			else if (playerCenterY > spikeCenterY && (hitPlayer.left < (hitSpike.left + hitSpike.width)) && (hitPlayer.left + hitPlayer.width) > hitSpike.left)
			{
				player->spikeSide = HEIGHT;
			}
		}
	}

	if (player->spikeSide != NOTHING)
	{
		player->action.isTouchingLeftWall = sfFalse;
		player->action.isTouchingRightWall = sfFalse;
		player->action.isTouchingWall = sfFalse;


		if (player->spikeSide == TOP)
		{
			player->data.velocity.y = -SPIKE_VELOCITY;
		}
		else if (player->spikeSide == HEIGHT)
		{
			player->data.velocity.y = SPIKE_VELOCITY;
		}


		player->data.timerSpikeWidth = 0.f;
		player->data.timerSpikeHeight = 0.f;
		player->data.health -= 20;
		player->spikeSide = NOTHING;
		player->action.isMoving = sfTrue;

	}



}

void CheckPlayerHP(void)
{
	if (player->data.health <= 0)
	{
		ChangeLevel(player->data.level);
		player->data.health = player->data.maxHealth;
	}
}


void BasePlayer()
{

	Player* temp = calloc(1, sizeof(Player));

	if (!temp)
	{
		return;
	}
	player = temp;

	player->data = (Stats){ 0 };
	player->action = (Action){ 0 };
	player->texture = sfTexture_createFromFile("Assets/Sprites/Game/Player/playerUpD.png", NULL);

	CreateSprite(player->texture, &player->sprite, ORIGIN_CENTER_X, GetPlayerSpawn());
	sfSprite_setScale(player->sprite, (sfVector2f) { GAME_SCALE, GAME_SCALE });

	player->action.isTransitioning = sfTrue;

	player->shape.collisionPlayerShape = sfRectangleShape_create();
	sfRectangleShape_setSize(player->shape.collisionPlayerShape, (sfVector2f) { PLAYER_HITBOX_WIDTH* GAME_SCALE, PLAYER_HITBOX_HEIGHT* GAME_SCALE });

	sfVector2f spritePos = sfSprite_getPosition(player->sprite);
	sfRectangleShape_setPosition(player->shape.collisionPlayerShape, (sfVector2f) { spritePos.x - (PLAYER_HITBOX_WIDTH * GAME_SCALE) / 2.f, spritePos.y - (PLAYER_HITBOX_HEIGHT * GAME_SCALE) });

	sfRectangleShape_setOutlineColor(player->shape.collisionPlayerShape, sfRed);
	sfRectangleShape_setFillColor(player->shape.collisionPlayerShape, sfColor_fromRGBA(255, 255, 255, 50));

	player->lastState = IDLE;


	player->data.position = sfSprite_getPosition(player->sprite);
	player->shape.collisionPlayerRect = sfRectangleShape_getGlobalBounds(player->shape.collisionPlayerShape);
	player->shape.playerRect = sfSprite_getGlobalBounds(player->sprite);

	player->shape.rectCollisionPlayerMob = sfRectangleShape_create();
	sfRectangleShape_setSize(player->shape.rectCollisionPlayerMob, (sfVector2f) { PLAYER_COLLISION_WIDTH, PLAYER_COLLISION_HEIGHT });
	sfRectangleShape_setOrigin(player->shape.rectCollisionPlayerMob, (sfVector2f) { PLAYER_COLLISION_WIDTH / 2, PLAYER_COLLISION_HEIGHT });
	sfRectangleShape_setScale(player->shape.rectCollisionPlayerMob, (sfVector2f) { GAME_SCALE, GAME_SCALE });


	snprintf(player->data.level, sizeof(player->data.level), "Level_04");
	//	printf("player level: %s\n", player->data.level);
	player->data.attackCooldownTimer = 0.5f;


	player->data.maxHealth = 200;
	player->data.health = player->data.maxHealth;




	player->data.lastDirection = 1;
	player->data.speed = 350.f;

	player->data.doubleJumpUnlocked = sfFalse;
	player->data.dashUnlocked = sfFalse;
	player->data.upDashUnlocked = sfFalse;
	player->data.diagonalDashUnlocked = sfFalse;
	player->data.horizontalDashUnlocked = sfFalse;

	player->spikeSide = NOTHING;
	player->side = NOTHING_PLAYER;

	player->sound.swordSound = sfSound_create();
	player->sound.buffer = sfSoundBuffer_createFromFile("Assets/Audio/Sounds/Sword_Attack_Sound.wav");
	sfSound_setBuffer(player->sound.swordSound, player->sound.buffer);

	player->sound.axeSound = sfSound_create();
	player->sound.buffer = sfSoundBuffer_createFromFile("Assets/Audio/Sounds/Axe_Attack_Sound.wav");
	sfSound_setBuffer(player->sound.axeSound, player->sound.buffer);

	player->data.keyNumber = 0;

	player->shape.collisionAttackShape = sfRectangleShape_create();

	sfRectangleShape_setSize(player->shape.collisionAttackShape, (sfVector2f) { ATTACK_HITBOX_WIDTH* GAME_SCALE, ATTACK_HITBOX_HEIGHT* GAME_SCALE });

	sfRectangleShape_setFillColor(player->shape.collisionAttackShape, sfColor_fromRGBA(255, 0, 0, 150));

	player->action.isAttacking = sfFalse;

}

void SetSavedStat(PlayerSaveData* save)
{

	player->data.health = save->health;
	player->data.doubleJumpUnlocked = save->doubleJumpUnlocked;
	player->data.canWallJump = save->canWallJump;

	player->data.dashUnlocked = save->dashUnlocked;
	player->data.upDashUnlocked = save->upDashUnlocked;
	player->data.diagonalDashUnlocked = save->diagonalDashUnlocked;
	player->data.horizontalDashUnlocked = save->horizontalDashUnlocked;


	snprintf(player->data.level, sizeof(player->data.level), "%s", save->level);
	//printf("buffer: %s\n", player->data.level);
}

void CollisionPlayerTrigger()
{
	sfBool keyIsPressed = sfKeyboard_isKeyPressed(sfKeyE);

	for (unsigned i = 0; i < GetTriggerTabSize(); i++)
	{
		sfFloatRect trigger = {
			GetMapTrigger(i).left,
			GetMapTrigger(i).top,
			GetMapTrigger(i).width,
			GetMapTrigger(i).height
		};

		if (sfFloatRect_intersects(&player->shape.collisionPlayerRect, &trigger, NULL))
		{
			if (keyIsPressed && !keyWasPressed && player->data.keyNumber >= GetMobCount())
			{
				player->data.velocity.x = 0;
				player->data.velocity.y = 0;
				snprintf(player->data.level, sizeof(player->data.level), "%s", GetMapTrigger(i).name);
				player->action.isTransitioning = sfTrue;
				ChangeLevel(player->data.level);
			}
		}
	}

	keyWasPressed = keyIsPressed;
}

void CollisionPlayerDeathZone()
{
	for (unsigned i = 0; i < GetDeathZoneTabSize(); i++)
	{
		sfFloatRect deathZone = GetDeathZoneTab(i);

		if (sfFloatRect_intersects(&player->shape.collisionPlayerRect, &deathZone, NULL))
		{
			player->data.health = 0;
		}
	}
}

void DrawPlayer(sfRenderWindow* _renderWindow)
{

	sfBool drawHitbox = sfFalse;

	if (sfKeyboard_isKeyPressed(sfKeyX))
	{
		drawHitbox = sfTrue;
	}

	if (drawHitbox)
	{
		sfRenderWindow_drawRectangleShape(_renderWindow, player->shape.rectCollisionPlayerMob, NULL);
		sfRenderWindow_drawRectangleShape(_renderWindow, player->shape.collisionPlayerShape, NULL);
		//if (player->action.isAttacking)
		{
			sfRenderWindow_drawRectangleShape(_renderWindow, player->shape.collisionAttackShape, NULL);
		}
	}
	sfRenderWindow_drawSprite(_renderWindow, player->sprite, NULL);

}
void CleanUpPlayer(void)
{
	sfSprite_destroy(player->sprite);
	sfTexture_destroy(player->texture);
	sfRectangleShape_destroy(player->shape.collisionPlayerShape);
}

void StateMachine(PlayerState _state)
{
	if (player->currentState == _state)
	{
		return;
	}
	SetAnimation(_state);
}

float RandomFloat(float min, float max)
{
	return min + (float)rand() / (float)RAND_MAX * (max - min);
}