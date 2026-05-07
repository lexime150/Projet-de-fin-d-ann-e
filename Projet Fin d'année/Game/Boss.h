#ifndef BOSS_H
#define BOSS_H

#include "Common.h"
#include "Animation.h"

#define BOSS_SIZE 128

typedef enum BossState
{
	IDLE_1,
	IDLE_2,
	WALK,
	RUN_BOSS,
	ATTACK_1,
	ATTACK_2,
	ATTACK_3,
	SPECIAL,
	HURT,
	DEATH_BOSS,
	ANIM_NUMBER
}BossState;

typedef struct Timer
{
	float knockBackTimer;
	float timerAttack;
	float timerHurt;

}Timer;

typedef enum BossSide
{
	LEFT_BOSS,
	WIDTH_BOSS,
	TOP_BOSS,
	HEIGHT_BOSS,
	NOTHING_BOSS
}BossSide;

typedef struct Boss
{
	sfSprite* sprite;
	sfTexture* texture;

	BossState lastState;
	BossState currentState;

	Animation animationBoss[ANIM_NUMBER];
	Animation* currentAnimation;

	sfVector2f velocity;

	sfVector2f distWalk;
	sfVector2f distRun;
	int direction;
	sfBool isGrounded;

	Timer timer;

	BossState lastAttack;

	sfRectangleShape* attackShape;
	sfRectangleShape* hurtShape;


}Boss;

void LoadBoss(void);
void UpdateBoss(float _dt);
void DrawBoss(sfRenderWindow* _renderWindow);
void CleanupBoss(void);


#endif 