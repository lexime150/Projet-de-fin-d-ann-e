#include "Mob.h"
#include "Map.h"
#include "Player.h"

Mob mushroom = { 0 };
sfTexture* textureMushroom;
Player player;

void SetAnimationMushroom(MobState _state);
void StateMobMachine(MobState _state);
//void CheckCollisionMobPlat(Mob _mob);
void CheckCollisionMobPlat(float _dt);
void CheckDistanceMobPlayer(Mob _mob, float _dt);

void CheckVelocityY(float _dt);


void LoadMob(void)
{
	 textureMushroom = sfTexture_createFromFile("Assets/Sprites/Champignon du Mordhor.png", NULL);

	
	 mushroom.sprite = sfSprite_create();
	 sfSprite_setTexture(mushroom.sprite, textureMushroom, sfTrue);
	 sfSprite_setScale(mushroom.sprite, (sfVector2f){GAME_SCALE, GAME_SCALE});
	 sfSprite_setOrigin(mushroom.sprite, (sfVector2f) { (MUSHROOM_SIZE / 2), MUSHROOM_SIZE});
	 sfSprite_setPosition(mushroom.sprite, (sfVector2f){600, -300});

	 mushroom.speed = 0;
	 mushroom.velocity = (sfVector2f){0, 0};
	 mushroom.isGroundedMob = sfFalse;
	 
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
	CheckVelocityY(_dt);
	CheckCollisionMobPlat(_dt);
	
	mushroom.hitbox = sfSprite_getGlobalBounds(mushroom.sprite);

	sfSprite_move(mushroom.sprite, (sfVector2f) { mushroom.velocity.x * _dt, mushroom.velocity.y * _dt });





	UpdateAnimation(mushroom.currentMobAnimation, _dt);
}


void CheckCollisionMobPlat(float _dt)
{
	mushroom.isGroundedMob = sfFalse;

	for (int i = 0; i < GetCollisionTabSize(); i++)
	{
		sfFloatRect hitPlat = GetMapCollision(i);
		
		if (sfFloatRect_intersects(&hitPlat, &mushroom.hitbox, NULL))
		{
			if (mushroom.velocity.y > 0)
			{
				mushroom.isGroundedMob = sfTrue;
				mushroom.hitbox.top = hitPlat.top - hitPlat.height;
				mushroom.velocity.y = 0;
			}
			else if (mushroom.velocity.y < 0)
			{
				mushroom.hitbox.top = (hitPlat.top + hitPlat.height);
			}

			if ((mushroom.hitbox.left + mushroom.hitbox.width) > hitPlat.left && mushroom.isGroundedMob)
			{
				CheckDistanceMobPlayer(mushroom, _dt);
			}	
			else if (mushroom.hitbox.left < hitPlat.left)
			{
				sfSprite_setPosition(mushroom.sprite, (sfVector2f){hitPlat.left + (mushroom.hitbox.width / 2), sfSprite_getPosition(mushroom.sprite).y});
			}
			else if ((mushroom.hitbox.left + mushroom.hitbox.width) > (hitPlat.left + hitPlat.width))
			{
				sfSprite_setPosition(mushroom.sprite, (sfVector2f){(hitPlat.left + hitPlat.width) - (mushroom.hitbox.width / 2), sfSprite_getPosition(mushroom.sprite).y});
			}


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
			mushroom.velocity.x = -100.f;
		}
		else
		{
			sfSprite_setScale(mushroom.sprite, (sfVector2f) { GAME_SCALE, GAME_SCALE });
			mushroom.velocity.x = 100.f;
		}
	}
	else
	{
		mushroom.velocity.x = 0;
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


void CheckVelocityY(float _dt)
{
	if (!mushroom.isGroundedMob)
	{
		mushroom.velocity.y += GRAVITY * _dt;
		if (mushroom.velocity.y > GRAVITY * 100.f * _dt)
		{
			mushroom.velocity.y = GRAVITY * 100.f * _dt;
		}
	}


}
