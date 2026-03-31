#include "Mob.h"
#include "Map.h"
#include "Player.h"

Mob* mob = { 0 };

sfTexture* texture[MOB_NUMBER];
Player player;
unsigned mobCount;


void SetTypeMob(void);

void SetAnimationMushroom(MobState _state);
void StateMobMachine(MobState _state);
void CheckCollisionMobPlat(float _dt);
void MoveMob(float _dt);
void AttackMob(float _dt);

void CheckVelocityY(float _dt);

void AddMob(TypeMob _type, float _x, float _y);

float GetDistancePlayerMob(Player* _player, Mob* _mob);

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



	AddMob(SKELETON, 600, 125);

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
	switch (mob->mobType)
	{
	case MUSHROOM:
		firstFrame = (sfIntRect){ 0, 0, MUSHROOM_SIZE, MUSHROOM_SIZE };
		mob[mobCount].mobAnimation[IDLE_MOB] = CreateAnimation(mob[mobCount].sprite, 4, 7, sfTrue, sfTrue, firstFrame);

		firstFrame.top += MUSHROOM_SIZE;
		mob[mobCount].mobAnimation[RUN_MOB] = CreateAnimation(mob[mobCount].sprite, 8, 10, sfTrue, sfTrue, firstFrame);

		firstFrame.top += MUSHROOM_SIZE;
		mob[mobCount].mobAnimation[ATTACK_MOB] = CreateAnimation(mob[mobCount].sprite, 8, 10, sfTrue, sfFalse, firstFrame);

		firstFrame.top += MUSHROOM_SIZE;
		mob[mobCount].mobAnimation[TAKE_IT] = CreateAnimation(mob[mobCount].sprite, 3, 9, sfTrue, sfFalse, firstFrame);

		firstFrame.top += MUSHROOM_SIZE;
		mob[mobCount].mobAnimation[DEATH] = CreateAnimation(mob[mobCount].sprite, 4, 7, sfTrue, sfFalse, firstFrame);
		break;
	case SKELETON:
		firstFrame = (sfIntRect){0, 0, HITBOX_SKELETON, HITBOX_SKELETON};
		mob[mobCount].mobAnimation[IDLE_MOB] = CreateAnimation(mob[mobCount].sprite, 3, 6, sfTrue, sfTrue, firstFrame);


	default:
		break;
	}
	SetAnimationMushroom(IDLE_MOB, mobCount);



}




void SetAnimationMushroom(MobState _state, unsigned _i)
{
	mob[_i].lastState = mob->currentState;
	mob[_i].currentState = _state;
	mob[_i].currentMobAnimation = &mob->mobAnimation[_state];
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
		SetAnimationMushroom(_state, _i);
	}
}

void UpdateMob(sfRenderWindow* _renderWindow, float _dt)
{
	for (unsigned i = 0; i < mobCount; i++)
	{


		mob[i].hitRect = sfRectangleShape_getGlobalBounds(mob[i].rect);
		sfSprite_move(mob[i].sprite, (sfVector2f) { mob[i].velocity.x* _dt, mob[i].velocity.y* _dt });
		sfRectangleShape_setPosition(mob[i].rect, sfSprite_getPosition(mob[i].sprite));



		MoveMob(_dt);
		AttackMob(_dt);

		CheckVelocityY(_dt);
		CheckCollisionMobPlat(_dt);

		UpdateAnimation(mob[i].currentMobAnimation, _dt);

	}
}


void CheckCollisionMobPlat(float _dt)
{
	mob->isGroundedMob = sfFalse;


	for (unsigned i = 0; i < GetCollisionTabSize(); i++)
	{
		sfFloatRect hitPlat = GetMapCollision(i);
		mob->hitbox = sfSprite_getGlobalBounds(mob->sprite);

		if (sfFloatRect_intersects(&hitPlat, &mob->hitbox, NULL))
		{
			if (mob->velocity.y > 0)
			{
				mob->isGroundedMob = sfTrue;
				mob->hitbox.top = hitPlat.top - (hitPlat.height);
				mob->velocity.y = 0;
			}
			else if (mob->velocity.y < 0)
			{
				mob[0].hitbox.top = (hitPlat.top + hitPlat.height);
			}

			if ((mob->hitbox.left + mob->hitbox.width) > hitPlat.left && mob->isGroundedMob)
			{
				MoveMob(_dt);
			}
			else if (mob->hitRect.left < hitPlat.left)
			{
				sfSprite_setPosition(mob->sprite, (sfVector2f) { hitPlat.left + (mob->hitRect.width / 2), sfSprite_getPosition(mob->sprite).y });
				mob->velocity.x = 0;

			}
			else if ((mob->hitRect.left + mob->hitRect.width) > (hitPlat.left + hitPlat.width))
			{
				sfSprite_setPosition(mob->sprite, (sfVector2f) { (hitPlat.left + hitPlat.width) - (mob->hitRect.width / 2), sfSprite_getPosition(mob->sprite).y });
				mob->velocity.x = 0;

			}

		}
	}

}

void MoveMob(float _dt)
{
	sfVector2f posPlayer = sfSprite_getPosition(player.sprite);
	sfVector2f posMob = sfSprite_getPosition(mob->sprite);

	float distX = posPlayer.x - posMob.x;
	if ((GetDistancePlayerMob(&player, mob) < 400.f && GetDistancePlayerMob(&player, mob) > -400.f) && (GetDistancePlayerMob(&player, mob) > DIST_ATTACK || GetDistancePlayerMob(&player, mob) < -DIST_ATTACK))
	{
	//	StateMobMachine(RUN_MOB);
		if (distX < 0)
		{
			sfSprite_setScale(mob->sprite, (sfVector2f) { -GAME_SCALE, GAME_SCALE });
			mob->velocity.x = -DIST_ATTACK;
		}
		else
		{
			sfSprite_setScale(mob->sprite, (sfVector2f) { GAME_SCALE, GAME_SCALE });
			mob->velocity.x = DIST_ATTACK;
		}

	}

}

void AttackMob(float _dt)
{
	mob->timerState += _dt;
	if ((GetDistancePlayerMob(&player, mob) < ((player.playerRect.left + player.playerRect.width) - mob->hitRect.left) &&
		GetDistancePlayerMob(&player, mob) > player.collisionPlayerRect.left - (mob->hitRect.left + mob->hitRect.width)) && mob->timerState > 0.85f)
	{
		mob->velocity.x = 0;
		mob->timerState = 0.f;
	//	StateMobMachine(ATTACK_MOB);
	}

	if (!mob->currentMobAnimation->isPlaying)
	{
		StateMobMachine(IDLE_MOB);

	}
}


float GetDistancePlayerMob(Player* _player, Mob* _mob)
{
	float distX = _player->position.x - sfSprite_getPosition(_mob->sprite).x;
	float distY = _player->position.y - sfSprite_getPosition(_mob->sprite).y;

	return sqrtf((distX * distX) + (distY * distY));

}


void DrawMob(sfRenderWindow* _renderWindow)
{

	sfRenderWindow_drawRectangleShape(_renderWindow, mob[0].rect, NULL);
	sfRenderWindow_drawSprite(_renderWindow, mob[0].sprite, NULL);

}

void cleanupmob(void)
{

}


void CheckVelocityY(float _dt)
{
	if (!mob->isGroundedMob)
	{
		mob->velocity.y += GRAVITY * _dt;
		if (mob->velocity.y > GRAVITY * 100.f * _dt)
		{
			mob->velocity.y = GRAVITY * 100.f * _dt;
		}
	}

}
