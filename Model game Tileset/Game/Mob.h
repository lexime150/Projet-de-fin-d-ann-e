#ifndef MOB_H
#define MOB_H

#include "Common.h"


#define MUSHROOM_SIZE 45
#define HITBOX_MUSHROOM_WIDTH 24
#define HITBOX_MUSHROOM_HEIGHT 37

#define DIST_ATTACK_MUSHROOM 150.f
#define DIST_RUN_MUSHROOM 400.f

#define DIST_RUN_SKELETON 300.f
#define DIST_ATTACK_SKELETON 100.f

#define DIST_RANGE_ENEMY 17.f

#define HITBOX_SKELETON_HEIGHT 32
#define HITBOX_ATTACK_SKELETON_WIDTH 48

#define TIMER_ATTACK 1.f

typedef enum MobState
{
	IDLE_MOB,
	RUN_MOB,
	ATTACK_MOB,
	TAKE_IT,
	DEATH,
	STATE_MOB_NUMBER
}MobState;

typedef enum TypeMob
{
	MUSHROOM,
	SKELETON,
	GOBLIN,
	MOB_NUMBER

}TypeMob;

typedef enum MobAct
{
	IS_IDLE,
	IS_MOVING,
	IS_ATTACK,
	IS_ALIVE,
	IS_TAKE_HIT,
	IS_GROUNDED_MOB
}MobAct;


typedef struct Mob
{
	sfSprite* sprite;

	float speed;
	sfVector2f velocity;
	sfVector2f position;

	Animation mobAnimation[STATE_MOB_NUMBER];
	Animation* currentMobAnimation;

	sfBool isMoving;
	sfBool isAttack;
	sfBool isAlive;
	sfBool isTakeIt;
	sfBool isGroundedMob;

	MobAct act;

	MobState lastState;
	MobState currentState;

	sfFloatRect hitbox;


	sfRectangleShape* rect;
	sfFloatRect hitRect;

	float timerAttack;
	float timerDamage;

	unsigned hp;

	TypeMob mobType;

	float rangeMove;
	float rangeAttack;

}Mob;


void LoadMob(void);
void LoadMobAnimation(unsigned _i);
void UpdateMob(sfRenderWindow* _renderWindow, float _dt);
void DrawMob(sfRenderWindow* _renderWindow);
void CleanupMob(void);

#endif 