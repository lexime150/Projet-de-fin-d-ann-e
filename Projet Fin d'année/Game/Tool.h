#ifndef TOOL_H
#define TOOL_H

#include "Common.h"





typedef enum OriginSprite
{
	ORIGIN_VANILLA,
	ORIGIN_CENTER,
	ORIGIN_CENTER_X

}OriginSprite;

void CreateSprite(sfTexture** _texture, char* _char, sfSprite** _sprite, OriginSprite _origin, sfVector2f _pos);
sfRectangleShape* CreateRectangle(sfVector2f _size, sfVector2f _origin, sfVector2f _scale, sfColor _color);

#endif 