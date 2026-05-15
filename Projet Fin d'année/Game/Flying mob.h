#ifndef FLYING_MOB
#define FLYING_MOB


#include "Common.h"

#define FLY_MOB_SIZE 150

#define _I unsigned _i


typedef enum FlyingMobState
{
	FLIGHT,
	HURT_FLY_MOB,
	DEATH_FLY_MOB,
	ATTACK_1_FLY_MOB,
	ATTACK_2_FLY_MOB,
	ATTACK_PROJECTILES,
	PROJECTILES,
	STATE_FLYING_MOB_NUMBER

}FlyingMobState;

typedef struct TimerFlyingMob
{
	float timerDamage;
	float timerHurt;
	float knockBackTimer;
	float timerMoveY;

}TimerFlyingMob;

typedef struct Projectile
{
	sfSprite* sprite;
	float speed;
	sfVector2f velocity;
	float timerShoot;
	Animation animation;

	sfBool isTouching;

}Projectile;

typedef struct FlyingMob
{
	sfSprite* sprite;
	sfTexture* texture;

	sfVector2f velocity;
	sfVector2f position;

	TimerFlyingMob timer;

	sfBool isFlying;
	
	Animation animation[STATE_FLYING_MOB_NUMBER];
	Animation* currentAnimation;

	Projectile projectile;

	FlyingMobState currentState;
	FlyingMobState lastState;


}FlyingMob;


void LoadFlyMob(void);
void UpdateFlyMob(float _dt);
void DrawFlyMob(sfRenderWindow* _renderWondow);
void CleanupFlyMob(void);


#endif 