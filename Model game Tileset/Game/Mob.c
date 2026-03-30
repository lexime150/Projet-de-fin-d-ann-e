#include "Mob.h"
#include "Map.h"
#include "Player.h"

Mob mushroom = { 0 };
sfTexture* textureMushroom;
Player player;

void SetAnimationMushroom(MobState _state);
void StateMobMachine(MobState _state);
void CheckCollisionMobPlat(Mob _mob);
void CheckDistanceMobPlayer(Mob _mob, float _dt);


void LoadMob(void)
{
	 textureMushroom = sfTexture_createFromFile("Assets/Sprites/Champignon du Mordhor.png", NULL);

	
	 mushroom.sprite = sfSprite_create();
	 sfSprite_setTexture(mushroom.sprite, textureMushroom, sfTrue);
	 sfSprite_setScale(mushroom.sprite, (sfVector2f){GAME_SCALE, GAME_SCALE});
	 sfSprite_setOrigin(mushroom.sprite, (sfVector2f) { MUSHROOM_SIZE / 2, MUSHROOM_SIZE});
	 sfSprite_setPosition(mushroom.sprite, (sfVector2f){600, -300});

	 mushroom.speed = 0;
	 
	 LoadMobAnimation();
}

void LoadMobAnimation(void)
{
	sfIntRect firstFrame = {0, 0, MUSHROOM_SIZE, MUSHROOM_SIZE };
	mushroom.mobAnimation[IDLE_MOB] = CreateAnimation(mushroom.sprite, 4, 7, sfTrue, sfTrue, firstFrame);

	firstFrame.top += MUSHROOM_SIZE;
	mushroom.mobAnimation[RUN_MOB] = CreateAnimation(mushroom.sprite, 8, 10, sfTrue, sfTrue, firstFrame);

	firstFrame.top += MUSHROOM_SIZE;
	mushroom.mobAnimation[ATTACK_MOB] = CreateAnimation(mushroom.sprite, 8, 10, sfTrue, sfFalse, firstFrame);

	firstFrame.top += MUSHROOM_SIZE;
	mushroom.mobAnimation[TAKE_IT] = CreateAnimation(mushroom.sprite, 3, 9, sfTrue, sfFalse, firstFrame);

	firstFrame.top += MUSHROOM_SIZE;
	mushroom.mobAnimation[DEATH] = CreateAnimation(mushroom.sprite, 4, 7, sfTrue, sfFalse, firstFrame);

	SetAnimationMushroom(IDLE_MOB);

}

void SetAnimationMushroom(MobState _state)
{
	mushroom.lastState = mushroom.currentState;
	mushroom.currentState = _state;
	mushroom.currentMobAnimation = &mushroom.mobAnimation[_state];
	mushroom.currentMobAnimation->currentFrame = 0;
	mushroom.currentMobAnimation->isPlaying = sfTrue;
	mushroom.currentMobAnimation->timer = 0.f;

}

void StateMobMachine(MobState _state)
{
	if (mushroom.currentState == _state)
	{
		return;
	}
	else
	{
		SetAnimationMushroom(_state);
	}
}


void UpdateMob(sfRenderWindow* _renderWindow, float _dt)
{
	CheckCollisionMobPlat(mushroom);
	CheckDistanceMobPlayer(mushroom, _dt);
	sfSprite_move(mushroom.sprite, (sfVector2f) { mushroom.speed* _dt, 0 });
	UpdateAnimation(mushroom.currentMobAnimation, _dt);
}

void CheckCollisionMobPlat(Mob _mob)
{
	sfFloatRect hitMob = sfSprite_getGlobalBounds(mushroom.sprite);

	for (int i = 0; i < GetCollisionTabSize(); i++)
	{
		sfFloatRect hitPlat = GetMapCollision(0);

		if (hitMob.top + hitMob.height > hitPlat.top)
		{
			sfSprite_setPosition(mushroom.sprite, (sfVector2f) { sfSprite_getPosition(mushroom.sprite).x, hitPlat.top });

		}

		

	}
}

void CheckDistanceMobPlayer(Mob _mob, float _dt)
{
	sfVector2f posPlayer = sfSprite_getPosition(player.sprite);
	sfVector2f posMob = sfSprite_getPosition(mushroom.sprite);

	float distX = posPlayer.x - posMob.x;

	if (distX < 400.f && distX > -400.f)
	{
		StateMobMachine(RUN_MOB);

		if (distX < 0)
		{
			sfSprite_setScale(mushroom.sprite, (sfVector2f){-GAME_SCALE, GAME_SCALE});
			mushroom.speed = -100.f;
		}
		else
		{
			sfSprite_setScale(mushroom.sprite, (sfVector2f) { GAME_SCALE, GAME_SCALE });
			mushroom.speed = 100.f;
		}
	}
	else
	{
		mushroom.speed = 0;
		StateMobMachine(IDLE_MOB);
	}
}

float GetDistPlatMob(unsigned _index)
{
	return sqrtf(GetDistPlatMobX(_index) * GetDistPlatMobX(_index) - GetDistPlatMobY(_index) * GetDistPlatMobY(_index));
}

float GetDistPlatMobX(unsigned _index)
{
	float posMob = sfSprite_getPosition(mushroom.sprite).x;
	float posPlat = GetMapCollision(_index).left;

	return posMob - posPlat;
}

float GetDistPlatMobY(_index)
{
	float posMob = sfSprite_getPosition(mushroom.sprite).y;
	float posPlat = GetMapCollision(_index).top;

	return posMob - posPlat;
}


void DrawMob(sfRenderWindow* _renderWindow)
{
	sfRenderWindow_drawSprite(_renderWindow, mushroom.sprite, NULL);
}

void CleanupMob(void)
{
}
