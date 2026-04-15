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
#define TIMER_TAKE_HIT_MUSHROOM 1.f

#define MUSHROOM_DAMAGE 38

#define COLLISION_MUSHROOM_WIDTH 1.5f
#define COLLISION_MUSHROOM_HEIGHT 30

#define MUSHROOM_ATTACK_SOUND 5

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

#define TIMER_ATTACK_SKELETON 0.4f
#define TIMER_TAKE_HIT_SKELETON 0.3f

#define SKELETON_KNOCKBACK_


#define SKELETON_DAMAGE 25




#define MOB_ATTACK_PLAYER_VELOCITY_X 450.f
#define MOB_ATTACK_PLAYER_VELOCITY_Y 50.f 

#define SKELETON_ATTACK_SOUND 2


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

typedef enum MobSide
{
	LEFT_MOB,
	TOP_MOB,
	WIDTH_MOB,
	HEIGHT_MOB,
	NOTHING_MOB
}MobSide;


typedef struct Timer
{
	float timerAttack;
	float timerTakeHit;

	float timerAttackLimit;
	float timerTakeHitLimit;

	float timerKnockBack;
	float timerKnockBackLimit;

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

	MobSide side;

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

	sfSoundBuffer* soundBufferTakeHit;
	sfSound* soundTakeHit;

	sfSoundBuffer* soundBufferAttack;
	sfSound* soundAttack;

	sfSoundBuffer* soundBufferDead;
	sfSound* soundDead;

	sfBool isGrounded;

	short frameAttackSound;

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