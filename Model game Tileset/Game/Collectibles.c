#include "Collectibles.h"

Items* item;
unsigned itemCount;
Player* player;
sfTexture* healthTexture;
sfTexture* keyTexture;
void VacuumEffect(void);
void Loaditem(void)
{
	healthTexture = sfTexture_createFromFile("Assets/Sprites/Game/Collectibles/heart.png", NULL);
	keyTexture = sfTexture_createFromFile("Assets/Sprites/Game/Collectibles/Keys.png", NULL);
	if (!healthTexture)
	{
		fprintf(stderr, "TEXTURE LOAD FAILURE\n");
		return;
	}
	item = NULL;
	itemCount = 0;
}

void Additem(ItemType _itemType, float _x, float _y)
{
	Items* temp = realloc(item, (itemCount + 1) * sizeof(Items));
	if (!temp)
	{
		fprintf(stderr, "REALLOC FAILURE\n");
		return;
	}
	item = temp;

	Items newitem = { 0 };
	newitem.isGrounded = sfFalse;

	int direction = (rand() % 2 == 0) ? -1 : 1;
	newitem.velocity.x = (float)direction * (rand() % (120 - 80 + 1) + 80);
	newitem.velocity.y = -200.f;


	newitem.itemSprite = sfSprite_create();
	switch (_itemType)
	{
	case ITEM_HEALTH:
		sfSprite_setTexture(newitem.itemSprite, healthTexture, sfTrue);
		newitem.type = ITEM_HEALTH;

		sfSprite_setScale(newitem.itemSprite, (sfVector2f) { GAME_SCALE * 1.2f, GAME_SCALE * 1.2f });
		break;
	case ITEM_KEY:
		sfSprite_setTexture(newitem.itemSprite, keyTexture, sfTrue);
		unsigned randMult = rand() % 3;
		sfIntRect keyRect = { 16 * randMult,0,16,16 };
		sfSprite_setTextureRect(newitem.itemSprite, keyRect);
		newitem.type = ITEM_KEY;
		sfSprite_setScale(newitem.itemSprite, (sfVector2f) { GAME_SCALE, GAME_SCALE });
		break;
	default:
		break;
	}


	sfFloatRect localBounds = sfSprite_getLocalBounds(newitem.itemSprite);
	sfSprite_setOrigin(newitem.itemSprite, (sfVector2f) { localBounds.width / 2.f, localBounds.height });

	sfSprite_setPosition(newitem.itemSprite, (sfVector2f) { _x, _y });
	newitem.itemPosition = sfSprite_getPosition(newitem.itemSprite);

	item[itemCount] = newitem;
	itemCount++;
}

void ApplyPhysicsitem(unsigned i, float _dt)
{
	if (!item[i].isGrounded)
	{
		item[i].velocity.y += GRAVITY * _dt;
		float maxFall = GRAVITY * 100.f * _dt;
		if (item[i].velocity.y > maxFall)
		{
			item[i].velocity.y = maxFall;

		}
	}
}


sfBool CollisionitemX(unsigned i, float _dx)
{
	sfFloatRect bounds = sfSprite_getGlobalBounds(item[i].itemSprite);
	sfVector2f  pos = sfSprite_getPosition(item[i].itemSprite);


	sfFloatRect hitbox = { bounds.left + _dx, bounds.top, bounds.width, bounds.height };

	for (unsigned j = 0; j < GetCollisionTabSize(); j++)
	{
		sfFloatRect platform = GetMapCollision(j);
		if (!sfFloatRect_intersects(&hitbox, &platform, NULL))
		{
			continue;

		}

		if (item[i].velocity.x > 0.f)
		{
			sfSprite_setPosition(item[i].itemSprite, (sfVector2f) { platform.left - bounds.width / 2.f, pos.y });

		}
		else if (item[i].velocity.x < 0.f)
		{
			sfSprite_setPosition(item[i].itemSprite, (sfVector2f) { platform.left + platform.width + bounds.width / 2.f, pos.y });

		}

		item[i].velocity.x = 0.f;
		return sfTrue;
	}

	for (unsigned j = 0; j < GetSemiSolidCollisionTabSize(); j++)
	{
		sfFloatRect semi = GetSemiSolidCollisionTab(j);
		if (!sfFloatRect_intersects(&hitbox, &semi, NULL))
		{
			continue;

		}

		if (item[i].velocity.x > 0.f)
		{
			sfSprite_setPosition(item[i].itemSprite, (sfVector2f) { semi.left - bounds.width / 2.f, pos.y });

		}
		else if (item[i].velocity.x < 0.f)
		{
			sfSprite_setPosition(item[i].itemSprite, (sfVector2f) { semi.left + semi.width + bounds.width / 2.f, pos.y });

		}

		item[i].velocity.x = 0.f;
		return sfTrue;
	}

	if (item[i].isGrounded)
	{
		sfFloatRect groundCheck = { bounds.left + _dx, pos.y + 1.f, bounds.width, 2.f };
		sfBool groundFound = sfFalse;

		for (unsigned j = 0; j < GetCollisionTabSize(); j++)
		{
			sfFloatRect collision = GetMapCollision(j);
			if (sfFloatRect_intersects(&groundCheck, &collision, NULL))
			{
				groundFound = sfTrue;
				break;
			}
		}
		for (unsigned j = 0; j < GetSemiSolidCollisionTabSize() && !groundFound; j++)
		{
			sfFloatRect semiSolidCollision = GetSemiSolidCollisionTab(j);
			if (sfFloatRect_intersects(&groundCheck, &semiSolidCollision, NULL))
			{
				groundFound = sfTrue;

			}
		}

		if (!groundFound)
		{
			item[i].velocity.x = 0.f;
			return sfTrue;
		}
	}

	sfSprite_setPosition(item[i].itemSprite, (sfVector2f) { pos.x + _dx, pos.y });
	return sfFalse;
}

sfBool CollisionitemY(unsigned i, float _dy)
{
	sfFloatRect bounds = sfSprite_getGlobalBounds(item[i].itemSprite);
	sfVector2f  pos = sfSprite_getPosition(item[i].itemSprite);

	float previousBottom = pos.y;

	sfFloatRect hitbox = { bounds.left, bounds.top + _dy, bounds.width, bounds.height };
	item[i].isGrounded = sfFalse;

	for (unsigned j = 0; j < GetCollisionTabSize(); j++)
	{
		sfFloatRect platform = GetMapCollision(j);
		if (!sfFloatRect_intersects(&hitbox, &platform, NULL))
		{
			continue;

		}

		if (item[i].velocity.y > 0.f)
		{
			sfSprite_setPosition(item[i].itemSprite, (sfVector2f) { pos.x, platform.top });
			item[i].isGrounded = sfTrue;
		}
		else if (item[i].velocity.y < 0.f)
		{

			sfSprite_setPosition(item[i].itemSprite, (sfVector2f) { pos.x, platform.top + platform.height + bounds.height });
		}

		item[i].velocity.y = 0.f;
		return sfTrue;
	}

	for (unsigned j = 0; j < GetSemiSolidCollisionTabSize(); j++)
	{
		sfFloatRect semi = GetSemiSolidCollisionTab(j);
		if (!sfFloatRect_intersects(&hitbox, &semi, NULL))
		{
			continue;

		}

		sfBool wasAbove = (previousBottom <= semi.top);
		sfBool isFalling = (item[i].velocity.y > 0.f);

		if (isFalling && wasAbove)
		{
			sfSprite_setPosition(item[i].itemSprite, (sfVector2f) { pos.x, semi.top });
			item[i].isGrounded = sfTrue;
			item[i].velocity.y = 0.f;
			return sfTrue;
		}
	}

	sfSprite_setPosition(item[i].itemSprite, (sfVector2f) { pos.x, pos.y + _dy });
	return sfFalse;
}

void Updateitem(float _dt)
{
	for (unsigned i = 0; i < itemCount; i++)
	{
		if (item[i].type == ITEM_HEALTH)
		{
			ApplyPhysicsitem(i, _dt);

			if (item[i].isGrounded)
			{
				item[i].velocity.x *= 0.85f;
				if (fabsf(item[i].velocity.x) < 1.f)
					item[i].velocity.x = 0.f;
			}

			float dx = item[i].velocity.x * _dt;
			CollisionitemX(i, dx);

			float dy = item[i].velocity.y * _dt;
			CollisionitemY(i, dy);
		}
		else if (item[i].type == ITEM_KEY)
		{
			sfVector2f pos = sfSprite_getPosition(item[i].itemSprite);
			sfSprite_setPosition(item[i].itemSprite, (sfVector2f) {pos.x + item[i].velocity.x * _dt,pos.y + item[i].velocity.y * _dt});
		}
	}



	if (sfKeyboard_isKeyPressed(sfKeyW))
	{
		Additem(ITEM_KEY, sfSprite_getPosition(player->sprite).x, sfSprite_getPosition(player->sprite).y);
		//itemCount--;
	}

	VacuumEffect();
}

void Drawitem(sfRenderWindow* _renderWindow)
{
	for (unsigned i = 0; i < itemCount; i++)
	{
		sfRenderWindow_drawSprite(_renderWindow, item[i].itemSprite, NULL);

	}
}

void Cleanupitem(void)
{
	sfTexture_destroy(healthTexture);
	sfTexture_destroy(keyTexture);
	healthTexture = NULL;
	keyTexture = NULL;


	for (unsigned i = 0; i < itemCount; i++)
	{
		sfSprite_destroy(item[i].itemSprite);

		item[i] = (Items){ 0 };
	}
	itemCount = 0;
	free(item);
	item = NULL;

}

sfVector2f GetItemDistance(unsigned _index)
{
	sfVector2f distance;


	distance.x = sfSprite_getPosition(item[_index].itemSprite).x - sfSprite_getPosition(player->sprite).x;
	distance.y = sfSprite_getPosition(item[_index].itemSprite).y - sfSprite_getPosition(player->sprite).y;

	return distance;
}

int GetItemCount(void)
{
	return itemCount;
}
void RemoveItem(int index)
{
	sfSprite_destroy(item[index].itemSprite);


	item[index] = item[itemCount - 1];

	itemCount--;


	Items* temp = realloc(item, itemCount * sizeof(Items));
	if (temp || itemCount == 0)
	{
		item = temp;

	}
}
void VacuumEffect(void)
{
	for (int i = 0; i < GetItemCount(); i++)
	{
		sfVector2f itemPos = sfSprite_getPosition(item[i].itemSprite);
		sfVector2f playerPos = sfSprite_getPosition(player->sprite);

		sfVector2f dir;
		dir.x = playerPos.x - itemPos.x;
		dir.y = playerPos.y - itemPos.y;

		float distance = sqrtf(dir.x * dir.x + dir.y * dir.y);

		if (distance > 0.1f)
		{
			dir.x /= distance;
			dir.y /= distance;

			if (item[i].type == ITEM_KEY)
			{
				float strength = 12.55f;
				item[i].velocity.x += dir.x * 2 *strength;
				item[i].velocity.y += dir.y * 2 * strength;
				if (distance < 300.f)
				{
					sfSprite_setPosition(item[i].itemSprite, playerPos);
				}
				if (distance < 125.f)
				{
					player->data.keyNumber++;
					RemoveItem(i);
					i--;
					continue;
				}
			}
			else
			{
				float radius = 200.f;
				if (distance < radius)
				{
					float strength = 25.f;
					item[i].velocity.x += dir.x * strength;
					item[i].velocity.y += dir.y * strength;
				}

				if (distance < 20.f)
				{
					if (player->data.health < player->data.maxHealth)
					{
						player->data.health += HEALTH_NUMBER;
						RemoveItem(i);
						i--;
					}
				}
			}
		}
	}
}