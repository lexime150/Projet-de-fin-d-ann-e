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
void CheckFlyMobLimitedZone(float _dt, _I);
void Projectiles(float _dt, _I);

void LoadFlyMob(void)
{
	for (unsigned i = 0; i < GetFlyMobSpawnTabSize(); i++)
	{
		AddFlyMob(GetFlyMobSpawn(i));
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

	newMob.projectile.timerShoot = 0;
	newMob.projectile.sprite = CreateSprite("Assets/Sprites/Game/Mob/Flying mob.png", _pos);
	sfSprite_setScale(newMob.projectile.sprite, (sfVector2f){GAME_SCALE, GAME_SCALE});

	newMob.limitedZone = CreateCircle(600.f, _pos, sfTransparent, sfColor_fromRGB(125, 147, 12));
	newMob.moveSteps = 0;

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
	flyMob[_i].animation[DEATH_FLY_MOB] = CreateAnimation(flyMob[_i].sprite, 4, 7, sfTrue, sfFalse, firstFrame);

	firstFrame.top += FLY_MOB_SIZE;
	flyMob[_i].animation[ATTACK_1_FLY_MOB] = CreateAnimation(flyMob[_i].sprite, 8, 11, sfTrue, sfFalse, firstFrame);

	firstFrame.top += FLY_MOB_SIZE;
	flyMob[_i].animation[ATTACK_2_FLY_MOB] = CreateAnimation(flyMob[_i].sprite, 8, 13, sfTrue, sfFalse, firstFrame);

	firstFrame.top += FLY_MOB_SIZE;
	flyMob[_i].animation[ATTACK_PROJECTILES] = CreateAnimation(flyMob[_i].sprite, 6, 9, sfTrue, sfFalse, firstFrame);

	firstFrame.top += FLY_MOB_SIZE; firstFrame.width = 48; firstFrame.height = 48;
	flyMob[_i].projectile.animation = CreateAnimation(flyMob[_i].projectile.sprite, 8, 10, sfTrue, sfTrue, firstFrame);


	SetFlyingMobAnimation(FLIGHT, _i);

}

void UpdateFlyMob(float _dt)
{
	for (unsigned i = 0; i < flyMobCount; i++)
	{
		if (flyMob[i].sprite != NULL)
		{
			CheckFlyMobLimitedZone(_dt, i);
			StateFlyMob(_dt, i);

			Projectiles(_dt, i);
			UpdateAnimation(flyMob[i].currentAnimation, _dt);

			if (flyMob[i].projectile.isShooting)
			{
				UpdateAnimation(&flyMob[i].projectile.animation, _dt);
			}
		}
	}

}

void DrawFlyMob(sfRenderWindow* _renderWindow)
{
	for (unsigned i = 0; i < flyMobCount; i++)
	{
		if (flyMob[i].sprite != NULL)
		{
			if (flyMob[i].projectile.isShooting)
			{
				sfRenderWindow_drawSprite(_renderWindow, flyMob[i].projectile.sprite, NULL);
			}

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
	flyMob[_i].projectile.timerShoot += _dt;
	flyMob[_i].timer.timerMoveY += _dt;

	if (flyMob[_i].timer.knockBackTimer > 0)
	{
		flyMob[_i].timer.knockBackTimer -= _dt;
		return;

	}
	if (flyMob[_i].limitedZoneEnable)
	{
		return;
	}

	if (flyMob[_i].currentState != DEATH_FLY_MOB)
	{
		sfVector2f posPlayer = player->data.position;
		sfVector2f posFlyMob = sfSprite_getPosition(flyMob[_i].sprite);
		sfVector2f dist = GetDistanceObjectVector(posPlayer, posFlyMob);

		if (dist.x < 700.f && dist.x > 250.f)
		{
			if (posPlayer.x < posFlyMob.x)
			{
				sfSprite_setScale(flyMob[_i].sprite, (sfVector2f) { -GAME_SCALE, GAME_SCALE });
				flyMob[_i].velocity.x = -200.f;
			}
			else
			{
				sfSprite_setScale(flyMob[_i].sprite, (sfVector2f) { GAME_SCALE, GAME_SCALE });
				flyMob[_i].velocity.x = 200.f;
			}
		}
		else if (dist.x < 250.f && dist.y < 650.f)
		{
			if (flyMob[_i].projectile.timerShoot > TIMER_SHOOT && !flyMob[_i].projectile.isShooting)
			{
				StateFlyingMobAnimation(ATTACK_PROJECTILES, _i);
				flyMob[_i].projectile.isShooting = sfTrue;
				flyMob[_i].velocity.x = 0;
				flyMob[_i].projectile.timerShoot = 0;
			}
			else if(!flyMob[_i].projectile.animation.isPlaying && flyMob[_i].currentState == ATTACK_PROJECTILES)
			{
				StateFlyingMobAnimation(FLIGHT, _i);
				flyMob[_i].projectile.isShooting = sfFalse;
			}
			
		}

		if (!flyMob[_i].limitedZoneEnable)
		{
			sfSprite_setPosition(flyMob[_i].sprite, (sfVector2f) { posFlyMob.x, posFlyMob.y + sinf(flyMob[_i].timer.timerMoveY * 4.f) });
			sfSprite_move(flyMob[_i].sprite, (sfVector2f) { flyMob[_i].velocity.x* _dt, flyMob[_i].velocity.y* _dt });
		}
	}
}

void CheckFlyMobLimitedZone(float _dt, _I)
{
	sfVector2f posMob = sfSprite_getPosition(flyMob[_i].sprite);
	sfVector2f posCircle = sfCircleShape_getPosition(flyMob[_i].limitedZone);
	float dist = GetDistanceObject(posMob, posCircle);

	if (dist > sfCircleShape_getRadius(flyMob[_i].limitedZone))
	{
		flyMob[_i].limitedZoneEnable = sfTrue;
	}

	if (flyMob[_i].limitedZoneEnable)
	{
		// Vecteur vers le centre du cercle
		sfVector2f dir = { posCircle.x - posMob.x, posCircle.y - posMob.y };
		float speed = 150.f; // pixels/sec, ajuste à ton goût

		// Normalise dir
		float len = sqrtf(dir.x * dir.x + dir.y * dir.y);
		if (len > 1.f)
		{
			dir.x = (dir.x / len) * speed * _dt;
			dir.y = (dir.y / len) * speed * _dt;
			sfSprite_move(flyMob[_i].sprite, dir);
		}

		// Désactive une fois rentré dans la zone
		if (dist <= sfCircleShape_getRadius(flyMob[_i].limitedZone))
		{
			flyMob[_i].limitedZoneEnable = sfFalse;
		}
	}
}

void Projectiles(float _dt, _I)
{
	if (flyMob[_i].projectile.isShooting) //&& !flyMob[_i].projectile.isTouching)
	{
		sfVector2f posProjectile = sfSprite_getPosition(flyMob[_i].projectile.sprite);
		sfVector2f posPlayer = sfSprite_getPosition(player->sprite);
		float scale = sfSprite_getScale(flyMob[_i].sprite).x;

		flyMob[_i].projectile.velocity.y = 235.f;
		float angle = atan2f(posProjectile.y - posPlayer.y, posProjectile.x - posPlayer.x);

		sfSprite_setRotation(flyMob[_i].sprite, angle);
		if (scale < 0)
		{
			flyMob[_i].projectile.velocity.x = -480.f;
		}
		else
		{
			flyMob[_i].projectile.velocity.x = 480.f;
		}

		sfSprite_move(flyMob[_i].projectile.sprite, (sfVector2f){flyMob[_i].projectile.velocity.x * _dt, flyMob[_i].projectile.velocity.y * _dt });

		sfFloatRect hitPlayer = sfSprite_getGlobalBounds(player->sprite);
		sfFloatRect hitProjectile = sfSprite_getGlobalBounds(flyMob[_i].projectile.sprite);

		if (sfFloatRect_intersects(&hitPlayer, &hitProjectile, NULL))
		{
			flyMob[_i].projectile.isShooting = sfFalse;
			PlayerDamage(25);
		}
	}
	
	if(!flyMob[_i].projectile.isShooting || flyMob[_i].projectile.timerShoot > TIMER_SHOOT)
	{
		flyMob[_i].projectile.velocity = (sfVector2f){ 0 };
		sfSprite_setPosition(flyMob[_i].projectile.sprite, sfSprite_getPosition(flyMob[_i].sprite));
		sfSprite_setRotation(flyMob[_i].sprite, 0);

		flyMob[_i].projectile.animation.currentFrame = 0;
		flyMob[_i].projectile.animation.timer = 0;
	}
}