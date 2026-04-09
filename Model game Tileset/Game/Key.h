#ifndef KEY_H
#define KEY_H

#include "common.h"

#define KEY_NUMBER 5
#define KEY_SIZE 16

void LoadKey(void);
void DrawKey(sfRenderWindow* _renderWindow);
void CleanupKey(void);


void KeyPosition(float _x, float _y,  float _textureRectX, int _textureRectY, int _keyNumber);

#endif 