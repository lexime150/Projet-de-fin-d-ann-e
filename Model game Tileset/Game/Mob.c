#include "Mob.h"
#include "Map.h"
#include "Player.h"

Mob* mob = { 0 };

sfTexture* texture[MOB_NUMBER];
Player player;
unsigned mobCount;




void SetAnimationMob(MobState _state, unsigned _i);
void StateMobMachine(MobState _state, unsigned _i);
void CheckCollisionMobPlat(float _dt, unsigned _i);
void MoveMob(float _dt, unsigned _i);
void AttackMob(float _dt, unsigned _i);

void CheckVelocityY(float _dt, unsigned _i);

void AddMob(TypeMob _type, float _x, float _y);

float GetDistancePlayerMob(Player* _player, unsigned _i);

void LoadMob(void)
{
	texture[MUSHROOM] = sfTexture_createFromFile("Assets/Sprites/Champignon du Mordhor.png", NULL);
	texture[SKELETON] = sfTexture_createFromFile("Assets/Sprites/skeleton.png", NULL);


	mob = malloc(sizeof(Mob));
	if (!mob)
	{
		fprintf(stderr, "MALLOC FAILURE");
		return;
	}



	AddMob(MUSHROOM, 600, 125);
	AddMob(MUSHROOM, 800, 125);

}

void AddMob(TypeMob _type, float _x, float _y)
{
	Mob* temp = realloc(mob, (mobCount + 1) * sizeof(Mob));
	if (!temp)
	{
		fprintf(stderr, "realloc failure\n");
		return;
	}

	mob = temp;
	temp = NULL;

	Mob newMob = { 0 };

	newMob.mobType = _type;

	newMob.sprite = sfSprite_create();

	newMob.rect = sfRectangleShape_create();


	switch (newMob.mobType)
	{
	case MUSHROOM:
		sfSprite_setTexture(newMob.sprite, texture[MUSHROOM], sfTrue);
		sfSprite_setOrigin(newMob.sprite, (sfVector2f) { MUSHROOM_SIZE / 2, MUSHROOM_SIZE });
		sfRectangleShape_setSize(newMob.rect, (sfVector2f) { HITBOX_MUSHROOM_WIDTH, HITBOX_MUSHROOM_HEIGHT });
		sfRectangleShape_setOrigin(newMob.rect, (sfVector2f) { HITBOX_MUSHROOM_WIDTH / 2, HITBOX_MUSHROOM_HEIGHT });
		break;
	case SKELETON:
		sfSprite_setTexture(newMob.sprite, texture[SKELETON], sfTrue);
		sfSprite_setOrigin(newMob.sprite, (sfVector2f) { HITBOX_SKELETON / 2, HITBOX_SKELETON });
		sfRectangleShape_setSize(newMob.rect, (sfVector2f) { HITBOX_SKELETON, HITBOX_SKELETON });
		sfRectangleShape_setOrigin(newMob.rect, (sfVector2f) { HITBOX_SKELETON / 2, HITBOX_SKELETON });
		sfSprite_setTextureRect(newMob.sprite, (sfIntRect) { 0, 0, 32, 32 });
		break;
	default:
		break;
	}

	sfSprite_setScale(newMob.sprite, (sfVector2f) { GAME_SCALE, GAME_SCALE });
	sfRectangleShape_setScale(newMob.rect, (sfVector2f) { GAME_SCALE, GAME_SCALE });
	sfSprite_setPosition(newMob.sprite, (sfVector2f) { _x, _y });
	sfRectangleShape_setPosition(newMob.rect, sfSprite_getPosition(newMob.sprite));

	mob[mobCount] = newMob;
	mobCount++;

	LoadMobAnimation();
}



void LoadMobAnimation()
{
	sfIntRect firstFrame = { 0 };
	switch (mob[mobCount - 1].mobType)
	{
	case MUSHROOM:
		firstFrame = (sfIntRect){ 0, 0, MUSHROOM_SIZE, MUSHROOM_SIZE };
		mob[mobCount - 1].mobAnimation[IDLE_MOB] = CreateAnimation(mob[mobCount - 1].sprite, 4, 7, sfTrue, sfTrue, firstFrame);

		firstFrame.top += MUSHROOM_SIZE;
		mob[mobCount - 1].mobAnimation[RUN_MOB] = CreateAnimation(mob[mobCount - 1].sprite, 8, 10, sfTrue, sfTrue, firstFrame);

		firstFrame.top += MUSHROOM_SIZE;
		mob[mobCount - 1].mobAnimation[ATTACK_MOB] = CreateAnimation(mob[mobCount - 1].sprite, 8, 10, sfTrue, sfFalse, firstFrame);

		firstFrame.top += MUSHROOM_SIZE;
		mob[mobCount - 1].mobAnimation[TAKE_IT] = CreateAnimation(mob[mobCount - 1].sprite, 3, 9, sfTrue, sfFalse, firstFrame);

		firstFrame.top += MUSHROOM_SIZE;
		mob[mobCount - 1].mobAnimation[DEATH] = CreateAnimation(mob[mobCount - 1].sprite, 4, 7, sfTrue, sfFalse, firstFrame);
		break;
	case SKELETON:
		firstFrame = (sfIntRect){0, 0, HITBOX_SKELETON, HITBOX_SKELETON};
		mob[mobCount - 1].mobAnimation[IDLE_MOB] = CreateAnimation(mob[mobCount - 1].sprite, 3, 6, sfTrue, sfTrue, firstFrame);


	default:
		break;
	}
	SetAnimationMob(IDLE_MOB, mobCount - 1);



}




void SetAnimationMob(MobState _state, unsigned _i)
{
	mob[_i].lastState = mob[_i].currentState;
	mob[_i].currentState = _state;
	mob[_i].currentMobAnimation = &mob[_i].mobAnimation[_state];
	mob[_i].currentMobAnimation->currentFrame = 0;
	mob[_i].currentMobAnimation->isPlaying = sfTrue;
	mob[_i].currentMobAnimation->timer = 0.f;

}

void StateMobMachine(MobState _state, unsigned _i)
{
	if (mob[_i].currentState == _state)
	{
		return;
	}
	else
	{
		SetAnimationMob(_state, _i);
	}
}

void UpdateMob(sfRenderWindow* _renderWindow, float _dt)
{
	for (unsigned i = 0; i < mobCount; i++)
	{


		mob[i].hitRect = sfRectangleShape_getGlobalBounds(mob[i].rect);
		sfSprite_move(mob[i].sprite, (sfVector2f) { mob[i].velocity.x* _dt, mob[i].velocity.y* _dt });
		sfRectangleShape_setPosition(mob[i].rect, sfSprite_getPosition(mob[i].sprite));



		MoveMob(_dt, i);
		AttackMob(_dt, i);

		CheckVelocityY(_dt, i);
		CheckCollisionMobPlat(_dt, i);

		UpdateAnimation(mob[i].currentMobAnimation, _dt);

	}
}


void CheckCollisionMobPlat(float _dt, unsigned _i)
{
	mob[_i].isGroundedMob = sfFalse;


	for (unsigned i = 0; i < GetCollisionTabSize(); i++)
	{
		sfFloatRect hitPlat = GetMapCollision(i);
		mob[_i].hitbox = sfSprite_getGlobalBounds(mob[_i].sprite);

		if (sfFloatRect_intersects(&hitPlat, &mob[_i].hitbox, NULL))
		{
			if (mob[_i].velocity.y > 0)
			{
				mob[_i].isGroundedMob = sfTrue;
				mob[_i].hitbox.top = hitPlat.top - (hitPlat.height);
				mob[_i].velocity.y = 0;
			}
			else if (mob[_i].velocity.y < 0)
			{
				mob[_i].hitbox.top = (hitPlat.top + hitPlat.height);
			}

			if ((mob[_i].hitbox.left + mob[_i].hitbox.width) > hitPlat.left && mob[_i].isGroundedMob)
			{
				//MoveMob(_dt, );
			}
			else if (mob[_i].hitRect.left < hitPlat.left)
			{
				sfSprite_setPosition(mob[_i].sprite, (sfVector2f) { hitPlat.left + (mob[_i].hitRect.width / 2), sfSprite_getPosition(mob[_i].sprite).y });
				mob[_i].velocity.x = 0;

			}
			else if ((mob[_i].hitRect.left + mob[_i].hitRect.width) > (hitPlat.left + hitPlat.width))
			{
				sfSprite_setPosition(mob[_i].sprite, (sfVector2f) { (hitPlat.left + hitPlat.width) - (mob[_i].hitRect.width / 2), sfSprite_getPosition(mob[_i].sprite).y });
				mob[_i].velocity.x = 0;

			}

		}
	}

}

void MoveMob(float _dt, unsigned _i)
{
	sfVector2f posPlayer = sfSprite_getPosition(player.sprite);
	sfVector2f posMob = sfSprite_getPosition(mob[_i].sprite);

	float distX = posPlayer.x - posMob.x;
	if ((GetDistancePlayerMob(&player, _i) < 400.f && GetDistancePlayerMob(&player, _i) > -400.f) && (GetDistancePlayerMob(&player, _i) > DIST_ATTACK || GetDistancePlayerMob(&player, _i) < -DIST_ATTACK))
	{
	//	StateMobMachine(RUN_MOB);
		if (distX < 0)
		{
			sfSprite_setScale(mob[_i].sprite, (sfVector2f) { -GAME_SCALE, GAME_SCALE });
			mob[_i].velocity.x = -DIST_ATTACK;
		}
		else
		{
			sfSprite_setScale(mob[_i].sprite, (sfVector2f) { GAME_SCALE, GAME_SCALE });
			mob[_i].velocity.x = DIST_ATTACK;
		}

	}

}

void AttackMob(float _dt, unsigned _i)
{
	mob[_i].timerState += _dt;
	if ((GetDistancePlayerMob(&player, _i) < ((player.playerRect.left + player.playerRect.width) - mob[_i].hitRect.left) &&
		GetDistancePlayerMob(&player, _i) > player.collisionPlayerRect.left - (mob[_i].hitRect.left + mob[_i].hitRect.width)) && mob[_i].timerState > 0.85f)
	{
		mob[_i].velocity.x = 0;
		mob[_i].timerState = 0.f;
		//StateMobMachine(ATTACK_MOB, _i);
	}

//	if (!mob[_i].currentMobAnimation->isPlaying)
//	{
		///StateMobMachine(IDLE_MOB, _i);

//	}
}


float GetDistancePlayerMob(Player* _player, unsigned _i)
{
	float distX = _player->position.x - sfSprite_getPosition(mob[_i].sprite).x;
	float distY = _player->position.y - sfSprite_getPosition(mob[_i].sprite).y;

	return sqrtf((distX * distX) + (distY * distY));

}


void DrawMob(sfRenderWindow* _renderWindow)
{
	for (int i = 0; i < mobCount; i++)
	{
		sfRenderWindow_drawRectangleShape(_renderWindow, mob[i].rect, NULL);
		sfRenderWindow_drawSprite(_renderWindow, mob[i].sprite, NULL);
	}
}

void cleanupmob(void)
{

}


void CheckVelocityY(float _dt, unsigned _i)
{
	if (!mob[_i].isGroundedMob)
	{
		mob[_i].velocity.y += GRAVITY * _dt;
		if (mob[_i].velocity.y > GRAVITY * 100.f * _dt)
		{
			mob[_i].velocity.y = GRAVITY * 100.f * _dt;
		}
	}

}
