#ifndef KEY_H
#define KEY_H

#include "common.h"


#define KEY_NUMBER 5
#define KEY_SIZE 16

#define KEY_HEIGHT_MIN 2
#define KEY_HEIGHT_MAX 5

#define KEY_WIDTH_MAX 7

typedef struct Key
{
	sfSprite* keySprite;
	float cooldownAnimation;

}Key;



void LoadKey(void);
void UpdateKey(float _dt);
void DrawKey(sfRenderWindow* _renderWindow);
void CleanupKey(void);


void KeyPosition(sfVector2f _pos, int _keyNumber, int _i);

#endif 