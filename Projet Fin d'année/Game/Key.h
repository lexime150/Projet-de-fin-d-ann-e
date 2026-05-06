#ifndef KEY_H
#define KEY_H

#include "common.h"
#include "Map.h"

#define KEY_NUMBER 6
#define KEY_SIZE 16

#define KEY_HEIGHT_MIN 2
#define KEY_HEIGHT_MAX 5

#define KEY_WIDTH_MAX 7

#define EXTRAS_KEY_SIZE (sfVector2f){32.f, 16.f}
#define EXTRAS_KEY_NUMBER 5

typedef enum ExtraKey
{
    CONTROL_L = 8,
    SHIFT = 4,
    SPACE = 10,

}ExtraKey;

typedef struct Key
{
    sfSprite* keySprite;
    float cooldownAnimation;
    sfBool isKeyPressed;
}Key;

void LoadKey(void);
void UpdateKey(float _dt);
void DrawKey(sfRenderWindow* _renderWindow);
void CleanupKey(void);


void KeyPosition(sfVector2f _pos, int _keyNumber, sfKeyCode _i);
void ExtraKeyPosition(sfVector2f _pos, int _keyNumber, ExtraKey _i);

#endif