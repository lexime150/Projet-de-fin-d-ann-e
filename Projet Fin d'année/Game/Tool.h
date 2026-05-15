#ifndef TOOL_H
#define TOOL_H

#include "Common.h"





typedef enum OriginSprite
{
	ORIGIN_VANILLA,
	ORIGIN_CENTER,
	ORIGIN_CENTER_X

}OriginSprite;

sfSprite* CreateSprite(char* _texture, sfVector2f _pos);
sfRectangleShape* CreateRectangle(sfVector2f _size, sfVector2f _origin, sfVector2f _scale, sfColor _color);
sfCircleShape* CreateCircle(float _radius, sfVector2f _pos, sfColor _fillColor, sfColor _outlineColor);


float GetDistanceObject(sfVector2f _obj1, sfVector2f _obj2);
sfVector2f GetDistanceObjectVector(sfVector2f _obj1, sfVector2f _obj2);

#endif 