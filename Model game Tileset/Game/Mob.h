#ifndef MOB_H
#define MOB_H

#include "Common.h"

//------Mushroom

//Sprite

#define MUSHROOM_SIZE_SPRITE 45

//Rectangle

#define HITBOX_MUSHROOM_WIDTH 24
#define HITBOX_MUSHROOM_HEIGHT 37
#define HITBOX_MUSHROOM_ATTACK_WIDTH 30

//Range

#define DIST_ATTACK_MUSHROOM 150.f
#define DIST_RUN_MUSHROOM 400.f

//Timer

#define TIMER_ATTACK_MUSHROOM 1.6f
#define TIMER_TAKE_HIT_MUSHROOM 0.75f

#define MUSHROOM_DEGATS 38

#define COLLISION_MUSHROOM_WIDTH 1.5f
#define COLLISION_MUSHROOM_HEIGHT 30

//-------Skeleton

//Range

#define DIST_RUN_SKELETON 300.f
#define DIST_ATTACK_SKELETON 100.f

//Hitbox

#define HITBOX_SKELETON_WIDTH 32
#define HITBOX_SKELETON_HEIGHT 28
#define HITBOX_ATTACK_STATE_SKELETON_WIDTH 48
#define HITBOX_ATTACK_SKELETON 48

#define COLLISION_SKELETON_WIDTH 1.5f
#define COLLISION_SKELETON_HEIGHT 28


//Timer

#define TIMER_ATTACK_SKELETON 1.1f
#define TIMER_TAKE_HIT_SKELETON 1.f

#define SKELETON_DEGATS 25


typedef enum MobState
{
	IDLE_MOB,
	RUN_MOB,
	ATTACK_MOB,
	TAKE_HIT,
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
	IS_DEATH,
	IS_TAKE_HIT,
	IS_GROUNDED_MOB
}MobAct;

typedef struct Timer
{
	float timerAttack;
	float timerTakeHit;

	float timerAttackLimit;
	float timerTakeHitLimit;

}Timer;


typedef struct Mob
{
	sfSprite* sprite;

	float speed;
	sfVector2f velocity;
	sfVector2f position;
	

	Animation mobAnimation[STATE_MOB_NUMBER];
	Animation* currentMobAnimation;

	MobAct act;

	MobState lastState;
	MobState currentState;

	sfFloatRect hitbox;
	sfFloatRect hitRect;
	sfFloatRect hitAttack;
	sfFloatRect collisionMob;

	Timer timer;

	sfRectangleShape* rect;
	sfRectangleShape* collisionRect;
	sfRectangleShape* attackRect;

	int hp;

	TypeMob mobType;

	float rangeMove;
	float rangeAttack;
	unsigned damage;

	sfBool justDied;
	sfBool isGrounded;

}Mob;


void LoadMob(void);
void LoadMobAnimation(unsigned _i);
void UpdateMob(sfRenderWindow* _renderWindow, float _dt);
void DrawMob(sfRenderWindow* _renderWindow);
void CleanupMob(void);

unsigned GetMobCount(void);

float GetDistancePlayerMobY(unsigned _i);
float GetDistancePlayerMobX(unsigned _i);
float GetDistancePlayerMobVector(unsigned _i);

void StateMobMachine(MobState _state, unsigned _i);


#endif 