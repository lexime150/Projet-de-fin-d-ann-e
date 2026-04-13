#ifndef COLLECTIBLES_H
#define COLLECTIBLES_H

#include "Common.h"
#include "Player.h"
#include "Mob.h"
#include "Map.h"


typedef enum ItemType
{
	ITEM_HEALTH,
	ITEM_KEY,
	ITEM_COUNT
}ItemType;

typedef struct Items
{
	sfSprite* itemSprite;
	sfTexture* itemTexture;

	sfVector2f itemPosition;
	sfVector2f velocity;
	ItemType type;
	sfBool isGrounded;
}Items;

void Loaditem(void);
void Additem(ItemType _itemType, float _x, float _y);
void Updateitem( float _dt);
void Drawitem(sfRenderWindow* _renderWindow);
void Cleanupitem(void);

#endif // !COLLECTIBLES_H
