#ifndef PLAYER_H
#define PLAYER_H

#include "Common.h"
#include "Map.h"

#define PLAYER_WIDTH 32
#define PLAYER_HEIGHT 32
#define JUMP_FORCE 600.f

#define SLIDE_SPEED 600.f

#define SLIDE_FRICTION 800.f

#define SLIDE_DURATION 0.4f
#define SLIDE_COOLDOWN 0.4f

#define MIN_SLIDE_JUMP_SPEED 375.f

#define WALL_JUMP_FRICTION 300.f
#define MIN_WALL_JUMP_SPEED 150.f

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

typedef struct Player
{
	sfSprite* sprite;
	sfTexture* texture;

	float speed;
	sfVector2f velocity;
	sfVector2f position;
	float wallJumpVelocityX;
	sfFloatRect playerRect;

	Animation animationPlayer[17];
	Animation* currentAnimation;


	short lastDirection;

	sfRectangleShape* collisionShape;
	sfFloatRect collisionRect;


	sfBool isGrounded;
	sfBool isMoving;

	sfBool isSliding;
	sfBool isSlideJumping;

	sfBool isTouchingRightWall;
	sfBool isTouchingLeftWall;
	sfBool isTouchingWall;
	sfBool isWallJumping;


	sfBool isAttacking;

	sfBool justWallJumped;

	PlayerState currentState;
	PlayerState lastState;

	float lastWallTouched;
	float currentWallTouched;
	float slideTimer;
	float slideVelocityX;
	float slideCooldownTimer;


}Player;


void LoadPlayer(void);
void UpdatePlayer(float _dt);
void DrawPlayer(sfRenderWindow* _renderWindow);
void CleanUpPlayer(void);

#endif