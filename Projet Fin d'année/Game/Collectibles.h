#ifndef COLLECTIBLES_H
#define COLLECTIBLES_H

#include "Common.h"
#include "Player.h"
#include "Mob.h"
#include "Map.h"

#define HEALTH_NUMBER 50

typedef enum ItemType
{
	ITEM_HEALTH,
	ITEM_KEY,
	ITEM_ORB_UPGRADE,
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

	float floatTimer;
	int triggerIndex;
	sfBool isCollected;
}Items;

void Loaditem(void);
void Additem(ItemType _itemType, float _x, float _y, int _triggerIndex);
int GetItemIndexByTrigger(int _triggerIndex);
void Updateitem( float _dt);
void Drawitem(sfRenderWindow* _renderWindow);
void Cleanupitem(void);
int GetItemCount(void);
void RemoveItem(int index);
sfVector2f GetItemDistance(unsigned _index);
#endif // !COLLECTIBLES_H
