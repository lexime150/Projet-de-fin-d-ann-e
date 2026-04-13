#include "Collectibles.h"

Orbs* orb;
unsigned orbCount;
sfTexture* g_orbTexture;

void LoadOrb(void)
{
	g_orbTexture = sfTexture_createFromFile("Assets/Sprites/heart.png", NULL);
	if (!g_orbTexture)
	{
		fprintf(stderr, "TEXTURE LOAD FAILURE\n");
		return;
	}
	orb = NULL;
	orbCount = 0;
}

void AddOrb(float _x, float _y)
{
	Orbs* temp = realloc(orb, (orbCount + 1) * sizeof(Orbs));
	if (!temp)
	{
		fprintf(stderr, "REALLOC FAILURE\n");
		return;
	}
	orb = temp;

	Orbs newOrb = { 0 };
	newOrb.isGrounded = sfFalse;

	int direction = (rand() % 2 == 0) ? -1 : 1;
	newOrb.velocity.x = direction * 120.f;
	newOrb.velocity.y = -200.f;

	newOrb.orbSprite = sfSprite_create();
	sfSprite_setTexture(newOrb.orbSprite, g_orbTexture, sfTrue);
	sfSprite_setScale(newOrb.orbSprite, (sfVector2f) { GAME_SCALE, GAME_SCALE });
	sfSprite_setPosition(newOrb.orbSprite, (sfVector2f) { _x, _y });

	orb[orbCount] = newOrb;
	orbCount++;
}

void ApplyPhysicsOrb(unsigned i, float _dt)
{
	if (!orb[i].isGrounded)
	{
		orb[i].velocity.y += GRAVITY * _dt;
		float maxFall = GRAVITY * 100.f * _dt;
		if (orb[i].velocity.y > maxFall)
		{
			orb[i].velocity.y = maxFall;

		}
	}
}

sfBool CollisionOrbX(unsigned i, float _dx)
{
	sfFloatRect bounds = sfSprite_getGlobalBounds(orb[i].orbSprite);
	sfVector2f  pos = sfSprite_getPosition(orb[i].orbSprite);

	sfFloatRect hitbox = { pos.x + _dx, pos.y, bounds.width, bounds.height };

	for (unsigned j = 0; j < GetCollisionTabSize(); j++)
	{
		sfFloatRect platform = GetMapCollision(j);
		if (!sfFloatRect_intersects(&hitbox, &platform, NULL))
		{
			continue;

		}

		if (orb[i].velocity.x > 0.f)
		{
			hitbox.left = platform.left - hitbox.width;

		}
		else if (orb[i].velocity.x < 0.f)
		{
			hitbox.left = platform.left + platform.width;

		}

		orb[i].velocity.x = 0.f;
		sfSprite_setPosition(orb[i].orbSprite, (sfVector2f) { hitbox.left, hitbox.top });
		return sfTrue;
	}

	if (orb[i].isGrounded)
	{
		sfFloatRect groundCheck = { hitbox.left,pos.y + bounds.height + 1.f,bounds.width,2.f };

		sfBool groundFound = sfFalse;
		for (unsigned j = 0; j < GetCollisionTabSize(); j++)
		{
			sfFloatRect platform = GetMapCollision(j);
			if (sfFloatRect_intersects(&groundCheck, &platform, NULL))
			{
				groundFound = sfTrue;
				break;
			}
		}
		for (unsigned j = 0; j < GetSemiSolidCollisionTabSize() && !groundFound; j++)
		{
			sfFloatRect semi = GetSemiSolidCollisionTab(j);
			if (sfFloatRect_intersects(&groundCheck, &semi, NULL))
			{

				groundFound = sfTrue;
			}
		}

		if (!groundFound)
		{
			orb[i].velocity.x = 0.f;
			return sfTrue;
		}
	}

	sfSprite_setPosition(orb[i].orbSprite, (sfVector2f) { pos.x + _dx, pos.y });
	return sfFalse;
}

sfBool CollisionOrbY(unsigned i, float _dy)
{
	sfFloatRect bounds = sfSprite_getGlobalBounds(orb[i].orbSprite);
	sfVector2f  pos = sfSprite_getPosition(orb[i].orbSprite);

	float previousBottom = pos.y + bounds.height;

	sfFloatRect hitbox = { pos.x, pos.y + _dy, bounds.width, bounds.height };
	orb[i].isGrounded = sfFalse;

	for (unsigned j = 0; j < GetCollisionTabSize(); j++)
	{
		sfFloatRect platform = GetMapCollision(j);
		if (!sfFloatRect_intersects(&hitbox, &platform, NULL))
		{
			continue;

		}

		if (orb[i].velocity.y > 0.f)
		{
			hitbox.top = platform.top - hitbox.height;
			orb[i].isGrounded = sfTrue;
		}
		else if (orb[i].velocity.y < 0.f)
		{
			hitbox.top = platform.top + platform.height;
		}

		orb[i].velocity.y = 0.f;
		sfSprite_setPosition(orb[i].orbSprite, (sfVector2f) { hitbox.left, hitbox.top });
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
		sfBool isFalling = (orb[i].velocity.y > 0.f);

		if (isFalling && wasAbove)
		{
			hitbox.top = semi.top - hitbox.height;
			orb[i].isGrounded = sfTrue;
			orb[i].velocity.y = 0.f;
			sfSprite_setPosition(orb[i].orbSprite, (sfVector2f) { hitbox.left, hitbox.top });
			return sfTrue;
		}
	}

	sfSprite_setPosition(orb[i].orbSprite, (sfVector2f) { pos.x, pos.y + _dy });
	return sfFalse;
}

void UpdateOrb(float _dt)
{
	for (unsigned i = 0; i < orbCount; i++)
	{
		ApplyPhysicsOrb(i, _dt);

		if (orb[i].isGrounded)
		{
			orb[i].velocity.x *= 0.85f;
			if (fabsf(orb[i].velocity.x) < 1.f)
			{
				orb[i].velocity.x = 0.f;

			}
		}

		float dx = orb[i].velocity.x * _dt;
		CollisionOrbX(i, dx);

		float dy = orb[i].velocity.y * _dt;
		CollisionOrbY(i, dy);
	}
}

void DrawOrb(sfRenderWindow* _renderWindow)
{
	for (unsigned i = 0; i < orbCount; i++)
	{
		sfRenderWindow_drawSprite(_renderWindow, orb[i].orbSprite, NULL);

	}
}

void CleanupOrb(void)
{
	for (unsigned i = 0; i < orbCount; i++)
	{

		sfSprite_destroy(orb[i].orbSprite);
	}

	free(orb);
	orb = NULL;
	orbCount = 0;

	sfTexture_destroy(g_orbTexture);
	g_orbTexture = NULL;
}