#ifndef PLAYER_H
#define PLAYER_H

#include "Common.h"
#include "Map.h"

#define PLAYER_WIDTH 32
#define PLAYER_HEIGHT 32

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
	sfFloatRect playerRect;

	Animation animationPlayer[17];
	Animation* currentAnimation;

	sfBool isGrounded;
	sfBool isSliding;

	short lastDirection;

	sfRectangleShape* collisionShape;
	sfFloatRect collisionRect;


	sfBool isMoving;

	PlayerState currentState;
	PlayerState lastState;

	sfBool isAttacking;

}Player;


void LoadPlayer(void);
void UpdatePlayer(float _dt);
void DrawPlayer(sfRenderWindow* _renderWindow);
void CleanUpPlayer(void);

#endif // !PLAYER_H
