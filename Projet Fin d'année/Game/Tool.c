#include "Tool.h"


sfSprite* CreateSprite(char* _texture,  sfVector2f _pos)
{

	sfSprite* sprite = sfSprite_create();
	sfTexture* texture = sfTexture_createFromFile(_texture, NULL);
	sfSprite_setTexture(sprite, texture, sfTrue);
	sfSprite_setPosition(sprite, _pos);
	return sprite;
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

sfCircleShape* CreateCircle(float _radius, sfVector2f _pos, sfColor _fillColor, sfColor _outlineColor)
{
	sfCircleShape* circle = sfCircleShape_create();
	sfCircleShape_setRadius(circle, _radius);
	sfCircleShape_setOrigin(circle, (sfVector2f) { _radius, _radius });
	sfCircleShape_setFillColor(circle, _fillColor);
	sfCircleShape_setOutlineThickness(circle, 0.4f);
	sfCircleShape_setOutlineColor(circle, _outlineColor);
	sfCircleShape_setPosition(circle, _pos);

	return circle;
}