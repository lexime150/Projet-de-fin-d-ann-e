#include "Flying mob.h"
#include "Map.h"
#include "Player.h"

FlyingMob* flyMob = { NULL };
Player* player;
unsigned flyMobCount = 0;

void SetFlyingMobAnimation(FlyingMobState _state, _I);
void StateFlyingMobAnimation(FlyingMobState _state, _I);
void StateFlyMob(float _dt, _I);
void LoadFlyMobAnimation(_I);
void AddFlyMob(sfVector2f _pos);
void LoadFlyMob(void)
{
	for (unsigned i = 0; i < GetFlyMobSpawnTabSize(); i++)
	{
		//AddFlyMob(GetFlyMobSpawn(i));
	}
}

void AddFlyMob(sfVector2f _pos)
{
	FlyingMob* temp = realloc(flyMob, (flyMobCount + 1) * sizeof(FlyingMob));
	if (temp == NULL)
	{
		fprintf(stderr, "realloc failure\n");
		return;
	}
	flyMob = temp;
	temp = NULL;

	FlyingMob newMob = { 0 };

	newMob.sprite = CreateSprite("Assets/Sprites/Game/Mob/Flying mob.png", _pos);
	sfSprite_setTextureRect(newMob.sprite, (sfIntRect) { 0, 0, FLY_MOB_SIZE, FLY_MOB_SIZE });
	sfSprite_setOrigin(newMob.sprite, (sfVector2f) { FLY_MOB_SIZE / 2.f, FLY_MOB_SIZE / 2.f });
	sfSprite_setScale(newMob.sprite, (sfVector2f) { GAME_SCALE, GAME_SCALE });



	newMob.limitedZone = CreateCircle(600.f, _pos, sfTransparent, sfColor_fromRGB(125, 147, 12));
	newMob.moveSteps = 0;
	newMob.lastAttack = 0;
	newMob.health = 150;

	flyMob[flyMobCount] = newMob;
	LoadFlyMobAnimation(flyMobCount);

	flyMobCount++;
}

void LoadFlyMobAnimation(_I)
{
	sfIntRect firstFrame = { 0, 0, FLY_MOB_SIZE, FLY_MOB_SIZE };
	flyMob[_i].animation[FLIGHT] = CreateAnimation(flyMob[_i].sprite, 8, 11, sfTrue, sfTrue, firstFrame);

	firstFrame.top += FLY_MOB_SIZE;
	flyMob[_i].animation[HURT_FLY_MOB] = CreateAnimation(flyMob[_i].sprite, 4, 7, sfTrue, sfFalse, firstFrame);

	firstFrame.top += FLY_MOB_SIZE;
	flyMob[_i].animation[DEATH_FLY_MOB] = CreateAnimation(flyMob[_i].sprite, 2, 7, sfTrue, sfFalse, firstFrame);

	firstFrame.top += FLY_MOB_SIZE;
	flyMob[_i].animation[ATTACK_1_FLY_MOB] = CreateAnimation(flyMob[_i].sprite, 8, 24, sfTrue, sfFalse, firstFrame);

	firstFrame.top += FLY_MOB_SIZE;
	flyMob[_i].animation[ATTACK_2_FLY_MOB] = CreateAnimation(flyMob[_i].sprite, 8, 13, sfTrue, sfFalse, firstFrame);

	firstFrame.top += FLY_MOB_SIZE;
	flyMob[_i].animation[ATTACK_PROJECTILES] = CreateAnimation(flyMob[_i].sprite, 6, 9, sfTrue, sfFalse, firstFrame);

	firstFrame.top += FLY_MOB_SIZE; firstFrame.width = 48; firstFrame.height = 48;

	SetFlyingMobAnimation(FLIGHT, _i);

}

void UpdateFlyMob(float _dt)
{
	for (unsigned i = 0; i < flyMobCount; i++)
	{
		if (flyMob[i].sprite != NULL)
		{
			StateFlyMob(_dt, i);

			UpdateAnimation(flyMob[i].currentAnimation, _dt);

		}
	}

}

void DrawFlyMob(sfRenderWindow* _renderWindow)
{
	for (unsigned i = 0; i < flyMobCount; i++)
	{
		if (flyMob[i].sprite != NULL)
		{

			sfRenderWindow_drawSprite(_renderWindow, flyMob[i].sprite, NULL);
			sfRenderWindow_drawCircleShape(_renderWindow, flyMob[i].limitedZone, NULL);
		}
	}
}


void CleanupFlyMob(void)
{
	for (unsigned i = 0; i < flyMobCount; i++)
	{
		sfSprite_destroy(flyMob[i].sprite);
	}

	free(flyMob);
	flyMob = (FlyingMob*){ NULL };
	flyMobCount = 0;
}

void SetFlyingMobAnimation(FlyingMobState _state, _I)
{
	flyMob[_i].lastState = flyMob[_i].currentState;
	flyMob[_i].currentState = _state;
	flyMob[_i].currentAnimation = &flyMob[_i].animation[_state];
	flyMob[_i].currentAnimation->isPlaying = sfTrue;
	flyMob[_i].currentAnimation->currentFrame = 0;
	flyMob[_i].currentAnimation->timer = 0;
}

void StateFlyingMobAnimation(FlyingMobState _state, _I)
{
	if (flyMob[_i].currentState != _state)
	{
		SetFlyingMobAnimation(_state, _i);
	}
}

void StateFlyMob(float _dt, _I)
{
	//flyMob[_i].projectile.timerShoot += _dt;
	flyMob[_i].timer.timerMoveY += _dt;
	flyMob[_i].timer.timerHurt += _dt;

	if (flyMob[_i].timer.knockBackTimer > 0)
	{
		flyMob[_i].timer.knockBackTimer -= _dt;
		return;

	}

	if (flyMob[_i].currentState != DEATH_FLY_MOB)
	{
		sfVector2f posPlayer = player->data.position;
		sfVector2f posFlyMob = sfSprite_getPosition(flyMob[_i].sprite);
		sfVector2f posCircle = sfCircleShape_getPosition(flyMob[_i].limitedZone);
		sfVector2f dist = GetDistanceObjectVector(posPlayer, posFlyMob);
		float distFlyMobLimited = GetDistanceObject(posFlyMob, posCircle);
		float distPlayerLimitedZone = GetDistanceObject(posPlayer, posCircle);
		float scale = GAME_SCALE;
		flyMob[_i].velocity = (sfVector2f){ 0 };


		if ((distFlyMobLimited > (sfCircleShape_getRadius(flyMob[_i].limitedZone) - 80.f)))
		{
			flyMob[_i].limitedZoneEnable = sfTrue;
		}

		if (posPlayer.x < posFlyMob.x)
		{
			scale = -GAME_SCALE;
			//flyMob[_i].velocity.x = -200.f;
		}
		else
		{
			scale = GAME_SCALE;
		}

		if ((distFlyMobLimited < (sfCircleShape_getRadius(flyMob[_i].limitedZone))) && (distPlayerLimitedZone < sfCircleShape_getRadius(flyMob[_i].limitedZone)) && !flyMob[_i].limitedZoneEnable)
		{
			if (dist.x > 100.f)
			{
				StateFlyingMobAnimation(FLIGHT, _i);
				flyMob[_i].velocity.x = 200.f;

				if (scale < 0)
				{
					flyMob[_i].velocity.x = -200.f;
				}


				if (dist.y > 35.f && (((posPlayer.y - 80.f) > posFlyMob.y) || (posPlayer.y + 25.f) < posFlyMob.y))
				{
					if (posPlayer.y > posFlyMob.y)
					{
						flyMob[_i].velocity.y = 200.f;

					}
					else
					{
						flyMob[_i].velocity.y = -200.f;
					}
				}
			}
			else
			{
				
				sfFloatRect hitPlayer = sfSprite_getGlobalBounds(player->sprite);
				sfFloatRect hitFlyMob = sfSprite_getGlobalBounds(flyMob[_i].sprite);

				if (sfFloatRect_intersects(&hitPlayer, &hitFlyMob, NULL))
				{
					flyMob[_i].timer.knockBackTimer += 0.5f;
					sfBool attackEnable = flyMob[_i].currentAnimation->currentFrame == 4;
					if (flyMob[_i].lastAttack == 0)
					{
						StateFlyingMobAnimation(ATTACK_1_FLY_MOB, _i);
						PlayerDamage(32);
						flyMob[_i].lastAttack = ATTACK_1_FLY_MOB;
					}

					if (flyMob[_i].lastAttack == ATTACK_1_FLY_MOB)
					{
						StateFlyingMobAnimation(ATTACK_2_FLY_MOB, _i);
						PlayerDamage(43);
						flyMob[_i].lastAttack = ATTACK_2_FLY_MOB;
					}
					else if (flyMob[_i].lastAttack == ATTACK_2_FLY_MOB)
					{
						StateFlyingMobAnimation(ATTACK_1_FLY_MOB, _i);
						PlayerDamage(32);
						flyMob[_i].lastAttack = ATTACK_1_FLY_MOB;
					}
				}
			}
		}
		else if (flyMob[_i].limitedZoneEnable)
		{
			if (distFlyMobLimited > 20.f)
			{
				if (posFlyMob.x < posCircle.x)
				{
					flyMob[_i].velocity.x = 250.f;
					scale = GAME_SCALE;
				}
				else
				{
					flyMob[_i].velocity.x = -250.f;
					scale = -GAME_SCALE;
				}

				if (posFlyMob.y < posCircle.y)
				{
					printf("TOP\n");
					flyMob[_i].velocity.y = 250.f;
				}
				else
				{
					printf("HEIGHT\n");
					flyMob[_i].velocity.y = -250.f;
				}
			}
			else
			{
				flyMob[_i].limitedZoneEnable = sfFalse;
			}
		}
		
		sfFloatRect hitFlyMob = sfSprite_getGlobalBounds(flyMob[_i].sprite);
		sfFloatRect hitPlayer = sfRectangleShape_getGlobalBounds(player->shape.collisionAttackShape);

		if (sfFloatRect_intersects(&hitFlyMob, &hitPlayer, NULL) && flyMob[_i].timer.timerHurt > 1.f)
		{
			flyMob[_i].timer.timerHurt = 0;
			flyMob[_i].health -= 35;
			flyMob[_i].timer.knockBackTimer += 0.5f;
			StateFlyingMobAnimation(HURT_FLY_MOB, _i);
		}


		if (flyMob[_i].health <= 0)
		{
			StateFlyingMobAnimation(DEATH_FLY_MOB, _i);
			flyMob[_i].velocity.x = 0; flyMob[_i].velocity.y = 325.f;
		}

		sfSprite_setScale(flyMob[_i].sprite, (sfVector2f) { scale, GAME_SCALE });

	}

	sfSprite_move(flyMob[_i].sprite, (sfVector2f) { flyMob[_i].velocity.x* _dt, flyMob[_i].velocity.y* _dt });

}