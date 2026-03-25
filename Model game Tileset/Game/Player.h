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
	TURN = RUN,
	JUMP,
	FALL = JUMP,
	D_JUMP = FALL,
	DASH_ALL_DIR,
	SLIDE,
	CLIMB_WALL,
	LADDER = CLIMB_WALL,
	WALL_GRIP_FALL,
	WALL_JUMP = WALL_GRIP_FALL,
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

	Animation animationPlayer[STATE_NUMBER];
	Animation* currentAnimation;

	sfBool isGrounded;


	sfRectangleShape* collisionShape;
	sfFloatRect collisionRect;
} Player;

void LoadPlayer(void);
void UpdatePlayer(float _dt);
void DrawPlayer(sfRenderWindow* _renderWindow);
void CleanUpPlayer(void);

#endif // !PLAYER_H
