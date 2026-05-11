#include "Boss.h"
#include "Map.h"
#include "Player.h"

Boss boss;
Player* player;
float specialCount;

void LoadBossAnimation();
void SetBossAnimation(BossState _state);
void StateBossMachine(BossState _state);
void SetVelocityBoss(float _dt);
float GetDistanceBossPlayerX();
void CheckCollisionBossPlat();
void CheckAttackBossPlayer(float _dt);
float GetDistanceObject(sfVector2f _obj1, sfVector2f _obj2);

void CheckCollisionCirclePlayer(float _dt);


void LoadBoss(void)
{
	if (strcmp(player->data.level, "Level_04") == 0)
	{

		boss.timer = (Timer){ 0 };
		boss.sprite = sfSprite_create();
		boss.isGrounded = sfTrue;
		boss.texture = sfTexture_createFromFile("Assets/Sprites/Game/Boss/Boss.png", NULL);
		sfSprite_setTexture(boss.sprite, boss.texture, sfTrue);
		sfSprite_setTextureRect(boss.sprite, (sfIntRect) { 0, 0, 128, 128 });
		sfSprite_setOrigin(boss.sprite, (sfVector2f) { 64, 128 });
		sfSprite_setScale(boss.sprite, (sfVector2f) { GAME_SCALE, GAME_SCALE });
		sfSprite_setPosition(boss.sprite, GetPlayerSpawn());
		boss.direction = 1;
		boss.lastAttack = 0;
		boss.special = 0;
		boss.specialAttackEnable = sfFalse;
		
		specialCount = 1;
		boss.specialAttackOpacity = 0.f;

		boss.attackShape = CreateRectangle((sfVector2f) { 130.f, 128.f }, (sfVector2f){ 65.f, 128.f }, (sfVector2f){ GAME_SCALE, GAME_SCALE }, sfBlue);
		boss.hurtShape = CreateRectangle((sfVector2f){100.f, 128.f}, (sfVector2f){50.f, 128.f}, (sfVector2f){GAME_SCALE, GAME_SCALE}, sfRed);

		boss.specialAttackShape = CreateCircle(10.f, sfSprite_getPosition(boss.sprite), sfTransparent, sfGreen);

		LoadBossAnimation();
	}

}

void LoadBossAnimation()
{
	if (strcmp(player->data.level, "Level_04") == 0)
	{
		sfIntRect firstFrame = { 0, 0, BOSS_SIZE, BOSS_SIZE };
		boss.animationBoss[IDLE_1] = CreateAnimation(boss.sprite, 7, 11, sfTrue, sfTrue, firstFrame);

		firstFrame.top += BOSS_SIZE;
		boss.animationBoss[IDLE_2] = CreateAnimation(boss.sprite, 5, 8, sfTrue, sfTrue, firstFrame);

		firstFrame.top += BOSS_SIZE;
		boss.animationBoss[WALK] = CreateAnimation(boss.sprite, 13, 11, sfTrue, sfTrue, firstFrame);

		firstFrame.top += BOSS_SIZE;
		boss.animationBoss[RUN_BOSS] = CreateAnimation(boss.sprite, 7, 12, sfTrue, sfTrue, firstFrame);

		firstFrame.top += BOSS_SIZE;
		boss.animationBoss[ATTACK_1] = CreateAnimation(boss.sprite, 16, 20, sfTrue, sfFalse, firstFrame);

		firstFrame.top += BOSS_SIZE;
		boss.animationBoss[ATTACK_2] = CreateAnimation(boss.sprite, 7, 11, sfTrue, sfFalse, firstFrame);

		firstFrame.top += BOSS_SIZE;
		boss.animationBoss[ATTACK_3] = CreateAnimation(boss.sprite, 10, 12, sfTrue, sfFalse, firstFrame);

		firstFrame.top += BOSS_SIZE;
		boss.animationBoss[SPECIAL] = CreateAnimation(boss.sprite, 5, 8, sfTrue, sfFalse, firstFrame);

		firstFrame.top += BOSS_SIZE;
		boss.animationBoss[HURT] = CreateAnimation(boss.sprite, 3, 6, sfTrue, sfFalse, firstFrame);

		firstFrame.left += (BOSS_SIZE * 3);
		boss.animationBoss[DEATH_BOSS] = CreateAnimation(boss.sprite, 3, 4, sfTrue, sfFalse, firstFrame);

		SetBossAnimation(IDLE_2);
	}
}

void SetBossAnimation(BossState _state)
{
	boss.lastState = boss.currentState;
	boss.currentState = _state;
	boss.currentAnimation = &boss.animationBoss[_state];
	boss.currentAnimation->timer = 0;
	boss.currentAnimation->currentFrame = 0;
	boss.currentAnimation->isPlaying = sfTrue;
}

void StateBossMachine(BossState _state)
{
	if (boss.currentState != _state)
	{
		SetBossAnimation(_state);
	}
}

static void UpdateBossInfo()
{
	sfVector2f posPlayer = sfSprite_getPosition(boss.sprite);

	sfRectangleShape_setPosition(boss.attackShape, posPlayer);
	sfRectangleShape_setPosition(boss.hurtShape, posPlayer);


	if (boss.currentState == SPECIAL)
	{
		specialCount += 0.13f;
		boss.specialAttackOpacity += (1.f + rand() % 6);
		sfCircleShape_setScale(boss.specialAttackShape, (sfVector2f) { specialCount* GAME_SCALE, specialCount* GAME_SCALE });
		sfCircleShape_setOutlineColor(boss.specialAttackShape, sfColor_fromRGBA(0.f, 255.f - boss.specialAttackOpacity * 1.5f, boss.specialAttackOpacity * 2.f, 255.f - boss.specialAttackOpacity));
	}
	else
	{
		specialCount = 1.f;
		sfCircleShape_setScale(boss.specialAttackShape, (sfVector2f) { specialCount* GAME_SCALE, specialCount* GAME_SCALE });
	}


}

void UpdateBoss(float _dt)
{
	if (strcmp(player->data.level, "Level_04") == 0)
	{
	

		CheckCollisionBossPlat();
		CheckAttackBossPlayer(_dt);
		SetVelocityBoss(_dt);
		UpdateBossInfo();
		UpdateAnimation(boss.currentAnimation, _dt);
	}
}

void DrawBoss(sfRenderWindow* _renderWindow)
{
	if (boss.sprite != NULL)
	{
		//sfRenderWindow_drawRectangleShape(_renderWindow, boss.attackShape, NULL);
		//sfRenderWindow_drawRectangleShape(_renderWindow, boss.hurtShape, NULL);
		if (boss.currentState == SPECIAL)
		{
			sfRenderWindow_drawCircleShape(_renderWindow, boss.specialAttackShape, NULL);

		}
		else
		{
			specialCount = 1.f;
		}

		sfRenderWindow_drawSprite(_renderWindow, boss.sprite, NULL);
	}
}

void CleanupBoss(void)
{
	sfSprite_destroy(boss.sprite);
	sfRectangleShape_destroy(boss.attackShape);
	sfRectangleShape_destroy(boss.hurtShape);
	boss = (Boss){ NULL };
	

}




void SetVelocityBoss(float _dt)
{
	//------Move-------//

	boss.timer.timerAttack += _dt;

	if (boss.timer.knockBackTimer > 0.f)
	{
		boss.timer.knockBackTimer -= _dt;
		return;
	}

	BossState state = IDLE_1;

	if (boss.isGrounded)
	{
		float distX = GetDistanceBossPlayerX();



		if ((fabs(distX) < 500.f) && (fabs(distX) > 200.f))
		{
			if (distX > 0)
			{
				boss.direction = 1;
				boss.velocity.x = 125.f;
			}
			else
			{
				boss.direction = -1;
				boss.velocity.x = -125.f;
			}
			state = WALK;
		}
		//---RUN---//
		else if (fabs(distX) < 200.f && fabs(distX) > 100.f)
		{

			if (distX > 0)
			{
				boss.direction = 1;
				boss.velocity.x = 150.f;
			}
			else
			{
				boss.direction = -1;
				boss.velocity.x = -150.f;
			}

			state = WALK;

		}
		//---ATTACK---//
		else if ((fabs(distX) < 100.f) && (boss.timer.timerAttack > 2.f) && (boss.currentState != ATTACK_1 && boss.currentState != ATTACK_2 && boss.currentState != ATTACK_3 && boss.currentState != SPECIAL)) //&& boss.special < SPECIAL_NUMBER)
		{
			boss.timer.timerAttack = 0;

			if (distX > 0)
			{
				boss.direction = 1;
			}
			else
			{
				boss.direction = -1;
			}
			boss.velocity.x = 0;
			boss.timer.knockBackTimer += 1.f;
			
			if (boss.lastAttack == 0)
			{
				boss.lastAttack = ATTACK_1;
				state = ATTACK_1;
				boss.special++;
			}
			else if(boss.special < SPECIAL_NUMBER)
			{
				if (boss.lastAttack == ATTACK_1)
				{
					boss.lastAttack = ATTACK_2;
					state = ATTACK_2;
				}
				else if (boss.lastAttack == ATTACK_2)
				{
					boss.lastAttack = ATTACK_3;
					state = ATTACK_3;
				}
				else
				{
					boss.lastAttack = ATTACK_1;
					state = ATTACK_1;
				}
				boss.special++;
			}
			else if (boss.special >= SPECIAL_NUMBER)
			{	
				if (boss.currentState != SPECIAL)
				{
					state = IDLE_1;
					boss.special = 0;
				}


				printf("%d\n", boss.special);
				state = SPECIAL;
				sfCircleShape_setPosition(boss.specialAttackShape, (sfVector2f) { sfSprite_getPosition(boss.sprite).x + 25.f, sfSprite_getPosition(boss.sprite).y - 150.f});
				

			
			}
		}
		//---IDLE---//
		else
		{
			state = IDLE_2;
			boss.velocity.x = 0;
		}
	}
	else
	{
		boss.velocity.y = GRAVITY;
	}
	sfSprite_setScale(boss.sprite, (sfVector2f) { boss.direction * GAME_SCALE, GAME_SCALE });
	sfSprite_move(boss.sprite, (sfVector2f) { boss.velocity.x * _dt, boss.velocity.y * _dt });
	StateBossMachine(state);


}


void CheckCollisionBossPlat()
{
	unsigned short tabSize = GetCollisionTabSize();

	//---Plat---//

	//--X--//
	sfFloatRect hitPlat = { 0 };
	sfFloatRect hitBoss = sfSprite_getGlobalBounds(boss.sprite);
	sfVector2f posBoss = sfSprite_getPosition(boss.sprite);
	sfFloatRect intersection = { 0 };


	for (int i = 0; i < tabSize; i++)
	{
		hitPlat = GetMapCollision(i);

		if (sfFloatRect_intersects(&hitPlat, &hitBoss, &intersection))
		{
			if (intersection.width > intersection.height)
			{
				if ((posBoss.y) > (hitPlat.top))
				{
					boss.isGrounded = sfTrue;
					posBoss.y -= intersection.height;
				}
				else if (hitBoss.top < (hitPlat.top + hitPlat.height))
				{
					boss.isGrounded = sfFalse;
					posBoss.y += intersection.height;
				}
			}
			sfSprite_setPosition(boss.sprite, posBoss);
			break;
		}
	}

	posBoss = sfSprite_getPosition(boss.sprite);
	hitBoss = sfSprite_getGlobalBounds(boss.sprite);

	for (int i = 0; i < tabSize; i++)
	{
		hitPlat = GetMapCollision(i);

		if (sfFloatRect_intersects(&hitPlat, &hitBoss, &intersection))
		{
			if (boss.isGrounded)
			{
				if (posBoss.x < hitPlat.left)
				{
					posBoss.x -= intersection.width;
				}
				else
				{
					posBoss.x += intersection.width;
				}

				sfSprite_setPosition(boss.sprite, posBoss);
				break;
			}
		}
	}
}

void CheckCollisionCirclePlayer(float _dt)
{
	boss.timer.specialAttackTimer += _dt;

	sfVector2f posPlayer = sfSprite_getPosition(player->sprite);
	sfVector2f posCircle = sfCircleShape_getPosition(boss.specialAttackShape);
	float circleSize = sfCircleShape_getRadius(boss.specialAttackShape) * sfCircleShape_getScale(boss.specialAttackShape).x;

	float dist = fabs(GetDistanceObject(posPlayer, posCircle));

	if (dist < circleSize && boss.timer.specialAttackTimer > 1.f)
	{
		boss.specialAttackEnable = sfTrue;
		boss.timer.specialAttackTimer = 0;

		StateMachine(FALL);
		//player->data.knockBackTimer += 0.75f;
		player->action.isGrounded = sfFalse;
		player->data.velocity.y = -650.f;
		PlayerDamage(40);
		if (posPlayer.x > posCircle.x)
		{
			player->data.velocity.x = 850.f;
		}
		else if(posPlayer.x < posCircle.x)
		{
			player->data.velocity.x = -850.f;
		}


	}
	else if ((boss.timer.specialAttackTimer > 1.f) && (boss.currentState != SPECIAL))
	{
		boss.specialAttackEnable = sfFalse;
	}
}




float GetDistanceBossPlayerX()
{
	float distX = sfSprite_getPosition(player->sprite).x - sfSprite_getPosition(boss.sprite).x;

	return distX;
}

void CheckAttackBossPlayer(float _dt)
{
	sfFloatRect hitBoss = sfRectangleShape_getGlobalBounds(boss.attackShape);
	sfFloatRect hitPlayer = sfRectangleShape_getGlobalBounds(player->shape.collisionPlayerShape);

	if (sfFloatRect_intersects(&hitBoss, &hitPlayer, NULL))
	{
		float posPlayerX = sfSprite_getPosition(player->sprite).x;
		float posBossX = sfSprite_getPosition(boss.sprite).x;
		float scale = GAME_SCALE;
		int hp = 0;
		if ((boss.special < SPECIAL_NUMBER) || (boss.currentState != SPECIAL))
		{
			if (boss.currentState == ATTACK_1)
			{
				if (boss.currentAnimation->currentFrame == 10)
				{

					player->action.isGrounded = sfFalse;
					player->data.velocity.x = 250.f;
					player->data.velocity.y = -1000.f;
					//PlayerDamage(35 + rand() % 35);
					hp = 16;
					player->data.knockBackTimer += 0.4f;
					StateMachine(FALL);
				}

			}
			else if (boss.currentState == ATTACK_2)
			{
				if (boss.currentAnimation->currentFrame == 6)
				{
					player->action.isGrounded = sfFalse;
					player->data.velocity.y = -200.f;
					player->data.velocity.x = 750.f;

					if (posPlayerX < posBossX)
					{
						scale *= -1.f;
						player->data.velocity.x *= -1.f;
					}

					player->data.knockBackTimer += 0.06f;
					hp = 21;
					sfSprite_setScale(player->sprite, (sfVector2f) { scale, GAME_SCALE });
					StateMachine(FALL);
				}
			}
			else if (boss.currentState == ATTACK_3)
			{
				if (boss.currentAnimation->currentFrame == 7)
				{
					player->action.isGrounded = sfFalse;
					player->data.velocity.y = -350.f;
					player->data.velocity.x = 500.f;

					if (posPlayerX < posBossX)
					{
						scale *= -1.f;
						player->data.velocity.x *= -1.f;
					}

					player->data.knockBackTimer += 0.3f;
					hp = 25;
					sfSprite_setScale(player->sprite, (sfVector2f) { scale, GAME_SCALE });
					StateMachine(FALL);
				}
			}
		}

		if (hp > 0)
		{
			PlayerDamage(hp + (rand() % hp));
		}
	}

	CheckCollisionCirclePlayer(_dt);

}


float GetDistanceObject(sfVector2f _obj1, sfVector2f _obj2)
{
	float distX = _obj1.x - _obj2.x;
	float distY = _obj1.y - _obj2.y;

	return sqrtf((distX * distX) + (distY * distY));
}