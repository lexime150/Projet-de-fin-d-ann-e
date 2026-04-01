#ifndef MOB_H
#define MOB_H

#include "Common.h"


#define MUSHROOM_SIZE 45
#define HITBOX_MUSHROOM_WIDTH 24
#define HITBOX_MUSHROOM_HEIGHT 37

#define DIST_ATTACK 150.f
#define DIST_RUN 400.f

#define HITBOX_SKELETON_HEIGHT 32
#define HITBOX_ATTACK_SKELETON_WIDTH 48

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


	MobState lastState;
	MobState currentState;

	sfFloatRect hitbox;


	sfRectangleShape* rect;
	sfFloatRect hitRect;

	float timerState;
	float timerDamage;

	TypeMob mobType;

}Mob;


void LoadMob(void);
void LoadMobAnimation(unsigned _i);
void UpdateMob(sfRenderWindow* _renderWindow, float _dt);
void DrawMob(sfRenderWindow* _renderWindow);
void CleanupMob(void);

#endif 