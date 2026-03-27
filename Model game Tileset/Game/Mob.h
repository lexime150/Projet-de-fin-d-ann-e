#ifndef MOB_H
#define MOB_H

#include "Common.h"

#define MUSHROOM_SIZE 150


typedef enum MobState
{
	IDLE_MOB,
	RUN_MOB,
	ATTACKÁMOB,
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
	
	MobState lastState;
	MobState currentState;

}Mob;


void LoadMob(void);
void LoadMobAnimation(void);
void UpdateMob(sfRenderWindow* _renderWindow, float _dt);
void DrawMob(sfRenderWindow* _renderWindow);
void CleanupMob(void);

#endif 