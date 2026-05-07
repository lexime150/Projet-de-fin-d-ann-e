#include "Tool.h"


void CreateSprite(sfTexture* _texture, sfSprite** _sprite, OriginSprite _origin, sfVector2f _pos)
{

	//*_texture = sfTexture_createFromFile(_char, NULL);
	*_sprite = sfSprite_create();

	sfSprite_setTexture(*_sprite, _texture, sfTrue);
	//sfSprite_setTextureRect(*_sprite, _textureRect);
	
	sfVector2f sizeSprite = { sfSprite_getGlobalBounds(*_sprite).width, sfSprite_getGlobalBounds(*_sprite).height};
	switch (_origin)
	{
	case ORIGIN_VANILLA:
		sizeSprite = (sfVector2f){ 0 };
		break;
	case ORIGIN_CENTER:
		sizeSprite.x /= 2;
		sizeSprite.y /= 2;
		break;
	case ORIGIN_CENTER_X:
		sizeSprite.x /= 2;
		break;
	default:
		break;
	}

	sfSprite_setOrigin(*_sprite, sizeSprite);
	sfSprite_setPosition(*_sprite, _pos);

}

sfRectangleShape* CreateRectangle(sfVector2f _size, sfVector2f _origin, sfVector2f _scale, sfColor _color)
{
	sfRectangleShape* rect = sfRectangleShape_create();
	sfRectangleShape_setSize(rect, _size);
	sfRectangleShape_setOrigin(rect, _origin);
	sfRectangleShape_setScale(rect, _scale);
	sfRectangleShape_setFillColor(rect, _color);

	return rect;

}