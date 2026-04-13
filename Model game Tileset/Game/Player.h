#ifndef PLAYER_H
#define PLAYER_H

#include "Common.h"
#include "Map.h"
#include "Game.h"

#define PLAYER_WIDTH 32
#define PLAYER_HEIGHT 32
#define JUMP_FORCE 620.f

#define SLIDE_SPEED 600.f

#define SLIDE_FRICTION 800.f

#define SLIDE_DURATION 0.6f
#define SLIDE_COOLDOWN 2.2f

#define MIN_SLIDE_JUMP_SPEED 375.f

#define WALL_JUMP_FRICTION 300.f
#define MIN_WALL_JUMP_SPEED 150.f

#define MIN_WALL_GRIP_DISTANCE 200.f

#define MAX_GRIP_WALL_SPEED 600.f


#define SWORD_DEGATS 66
#define AXE_DEGATS 90


#define PLAYER_HITBOX_WIDTH  20
#define PLAYER_HITBOX_HEIGHT 28

#define ATTACK_HITBOX_WIDTH 22
#define ATTACK_HITBOX_HEIGHT 15

#define ATTACK_SWORD_COOLDOWN 0.8f
#define ATTACK_AXE_COOLDOWN 1.2f


#define TIMER_TAKE_IT 0.52f
#define TIMER_SPIKE 0.3f
#define	TIMER_PLAYER_MOB 0.5f
#define TIMER_INVINCIBLE 2.f

#define SPIKE_VELOCITY 500.f

#define PLAYER_MOB_VELOCITY_X 350.f
#define PLAYER_MOB_VELOCITY_Y 120.f

#define PLAYER_MOB_MARGE 2.f

#define PLAYER_COLLISION_WIDTH 4.f
#define PLAYER_COLLISION_HEIGHT 32.f

typedef enum SpikeSide
{
	LEFT,
	TOP,
	WIDTH,
	HEIGHT,
	NOTHING

}SpikeSide;

typedef enum PlayerSide
{
	LEFT_PLAYER,
	TOP_PLAYER,
	WIDTH_PLAYER,
	HEIGHT_PLAYER,
	NOTHING_PLAYER
}PlayerSide;


typedef enum PlayerState
{
	IDLE,
	RUN,
	TURN,
	JUMP,
	FALL,
	D_JUMP,
	DASH_GROUND,
	DASH_UP,
	DASH_DIAGONAL,
	SLIDE,
	CLIMB_WALL,
	LADDER,
	WALL_GRIP_FALL,
	WALL_JUMP,
	AXE,
	SWORD,
	DASH,
	STATE_NUMBER

}PlayerState;

typedef struct Action
{
	sfBool isGrounded;
	sfBool isMoving;

	sfBool isSliding;
	sfBool isSlideJumping;

	sfBool isTouchingRightWall;
	sfBool isTouchingLeftWall;
	sfBool isTouchingWall;
	sfBool isWallJumping;

	sfBool justWallJumped;

	sfBool isTransitioning;
	sfBool isAttacking;
	sfBool degatsEnable;

	sfBool isInvincible;
	
}Action;

typedef struct Sound
{
	sfSoundBuffer* buffer;
	sfSound* swordSound;
	sfSound* axeSound;
}Sound;

typedef struct Stats
{
	float speed;
	float health;
	float maxHealth;
	char level[20];
	unsigned keyNumber;


	sfBool canWallJump;
	sfBool canDoubleJump;

	sfVector2f velocity;
	sfVector2f position;

	float wallJumpVelocityX;
	
	float slideTimer;
	float slideVelocityX;
	float slideCooldownTimer;

	short lastDirection;
	float lastWallTouched;
	float currentWallTouched;

	float attackCooldownTimer;

	float jumpStartPosition;
	float timerTakeIt;

	float timerSpikeWidth;
	float timerSpikeHeight;
	float knockBackTimer;

	float timerPlayerMob;

	float timerAttack;

	float timerInvincible;

}Stats;

typedef struct Shape
{
	sfFloatRect playerRect;
	sfRectangleShape* collisionPlayerShape;
	sfFloatRect collisionPlayerRect;

	sfRectangleShape* collisionAttackShape;
	sfFloatRect collisionAttackRect;

	sfRectangleShape* rectCollisionPlayerMob;
	sfFloatRect hitRectCollisionPlayerMob;

}Shape;

typedef struct Player
{
	sfSprite* sprite;
	sfTexture* texture;

	Action action;
	Stats data;
	Shape shape;
	Sound sound;

	PlayerState currentState;
	PlayerState lastState;

	Animation animationPlayer[17];
	Animation* currentAnimation;

	SpikeSide spikeSide;
	PlayerSide side;

}Player;


void LoadPlayer(PlayerSaveData* save);
void UpdatePlayer(float _dt);
void SetSavedStat(PlayerSaveData* save);
void DrawPlayer(sfRenderWindow* _renderWindow);
void CleanUpPlayer(void);

void StateMachine(PlayerState _state);

#endif