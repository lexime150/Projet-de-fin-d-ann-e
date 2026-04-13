#include "Collectibles.h"

Items* item;
unsigned itemCount;

sfTexture* healthTexture;
sfTexture* keyTexture;

void Loaditem(void)
{
	healthTexture = sfTexture_createFromFile("Assets/Sprites/Collectibles/heart.png", NULL);
	keyTexture = sfTexture_createFromFile("Assets/Sprites/Collectibles/Keys.png", NULL);
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
	newitem.velocity.x = direction * (rand() % (120 - 80 + 1) + 80);
	newitem.velocity.y = -200.f;
	

	newitem.itemSprite = sfSprite_create();
	switch (_itemType)
	{
	case ITEM_HEALTH:
		sfSprite_setTexture(newitem.itemSprite, healthTexture, sfTrue);
		newitem.type = ITEM_HEALTH;
		sfSprite_setScale(newitem.itemSprite, (sfVector2f) { GAME_SCALE*1.2, GAME_SCALE*1.2 });
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
	sfFloatRect hitbox = { pos.x + _dx, pos.y, bounds.width, bounds.height };

	for (unsigned j = 0; j < GetCollisionTabSize(); j++)
	{
		sfFloatRect platform = GetMapCollision(j);
		if (!sfFloatRect_intersects(&hitbox, &platform, NULL))
			continue;

		if (item[i].velocity.x > 0.f)
			sfSprite_setPosition(item[i].itemSprite, (sfVector2f) { platform.left - bounds.width, pos.y });
		else if (item[i].velocity.x < 0.f)
			sfSprite_setPosition(item[i].itemSprite, (sfVector2f) { platform.left + platform.width, pos.y });

		item[i].velocity.x = 0.f;
		return sfTrue;
	}

	// Vérifie les semi-solides aussi
	for (unsigned j = 0; j < GetSemiSolidCollisionTabSize(); j++)
	{
		sfFloatRect semi = GetSemiSolidCollisionTab(j);
		if (!sfFloatRect_intersects(&hitbox, &semi, NULL))
			continue;

		if (item[i].velocity.x > 0.f)
			sfSprite_setPosition(item[i].itemSprite, (sfVector2f) { semi.left - bounds.width, pos.y });
		else if (item[i].velocity.x < 0.f)
			sfSprite_setPosition(item[i].itemSprite, (sfVector2f) { semi.left + semi.width, pos.y });

		item[i].velocity.x = 0.f;
		return sfTrue;
	}

	// Si au sol, vérifie qu'il y a du sol devant avant de bouger
	if (item[i].isGrounded)
	{
		sfFloatRect groundCheck = { pos.x + _dx, pos.y + bounds.height + 1.f, bounds.width, 2.f };
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
				groundFound = sfTrue;
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

	float previousBottom = pos.y + bounds.height;

	sfFloatRect hitbox = { pos.x, pos.y + _dy, bounds.width, bounds.height };
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
			hitbox.top = platform.top - hitbox.height;
			item[i].isGrounded = sfTrue;
		}
		else if (item[i].velocity.y < 0.f)
		{
			hitbox.top = platform.top + platform.height;
		}

		item[i].velocity.y = 0.f;
		sfSprite_setPosition(item[i].itemSprite, (sfVector2f) { hitbox.left, hitbox.top });
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
			hitbox.top = semi.top - hitbox.height;
			item[i].isGrounded = sfTrue;
			item[i].velocity.y = 0.f;
			sfSprite_setPosition(item[i].itemSprite, (sfVector2f) { hitbox.left, hitbox.top });
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
		ApplyPhysicsitem(i, _dt);

		if (item[i].isGrounded)
		{
			item[i].velocity.x *= 0.85f;
			if (fabsf(item[i].velocity.x) < 1.f)
			{
				item[i].velocity.x = 0.f;

			}
		}

		float dx = item[i].velocity.x * _dt;
		CollisionitemX(i, dx);

		float dy = item[i].velocity.y * _dt;
		CollisionitemY(i, dy);
	}
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
	for (unsigned i = 0; i < itemCount; i++)
	{

		sfSprite_destroy(item[i].itemSprite);
	}

	free(item);
	item = NULL;
	itemCount = 0;

	sfTexture_destroy(healthTexture);
	healthTexture = NULL;
}