#include "Boss.h"
#include "Map.h"
#include "Player.h"

Boss boss;
Player* player;

void LoadBossAnimation();
void SetBossAnimation(BossState _state);
void StateBossMachine(BossState _state);
void SetVelocityBoss(float _dt);
float GetDistanceBossPlayerX();
void CheckCollisionBossPlat();
void CheckAttackBossPlayer();


void LoadBoss(void)
{
	if (strcmp(player->data.level, "Level_05") == 0)
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

		boss.attackShape = CreateRectangle((sfVector2f) { 130.f, 128.f }, (sfVector2f){ 65.f, 128.f }, (sfVector2f){ GAME_SCALE, GAME_SCALE }, sfBlue);
		boss.hurtShape = CreateRectangle((sfVector2f){100.f, 128.f}, (sfVector2f){50.f, 128.f}, (sfVector2f){GAME_SCALE, GAME_SCALE}, sfRed);

		LoadBossAnimation();
	}

}

void LoadBossAnimation()
{
	if (strcmp(player->data.level, "Level_05") == 0)
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

}

void UpdateBoss(float _dt)
{
	if (strcmp(player->data.level, "Level_05") == 0)
	{
		CheckCollisionBossPlat();
		SetVelocityBoss(_dt);
		UpdateBossInfo();
		UpdateAnimation(boss.currentAnimation, _dt);
	}
}

void DrawBoss(sfRenderWindow* _renderWindow)
{
	if (boss.sprite != NULL)
	{
		sfRenderWindow_drawRectangleShape(_renderWindow, boss.attackShape, NULL);
		sfRenderWindow_drawRectangleShape(_renderWindow, boss.hurtShape, NULL);
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

			state = RUN_BOSS;

		}
		//---ATTACK---//
		else if ((fabs(distX) < 100.f) && (boss.timer.timerAttack > 2.f) && (boss.currentState != ATTACK_1 && boss.currentState != ATTACK_2 && boss.currentState != ATTACK_3))
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
			}
			else
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
		//printf("llo");
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




float GetDistanceBossPlayerX()
{
	float distX = sfSprite_getPosition(player->sprite).x - sfSprite_getPosition(boss.sprite).x;

	return distX;
}

void CheckAttackBossPlayer()
{
	sfFloatRect hitBoss = sfRectangleShape_getGlobalBounds(boss.attackShape);
	sfFloatRect hitPlayer = sfRectangleShape_getGlobalBounds(player->shape.collisionPlayerShape);

	if (sfFloatRect_intersects(&hitBoss, &hitPlayer, NULL))
	{
		if (boss.currentState == ATTACK_1)
		{
			if (boss.currentAnimation->currentFrame == 10)
			{
				player->action.isGrounded;


			}

		}


	}


}