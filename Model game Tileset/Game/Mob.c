#include "Mob.h"
#include "Map.h"
#include "Player.h"

Mob* mushroom = { 0 };
Mob* zombie = { 0 };
sfTexture* textureMushroom;
Player player;
unsigned mobCount;

void SetAnimationMushroom(Mob* _mob, MobState _state);
void StateMobMachine(Mob* _mob, MobState _state);
void CheckCollisionMobPlat(Mob* _mob, float _dt);
void MoveMob(Mob* _mob, float _dt);
void AttackMob(Mob* _mob, float _dt);

void CheckVelocityY(Mob* _mob, float _dt);

float GetDistancePlayerMob(Player* _player, Mob* _mob);

void LoadMob(void)
{
	textureMushroom = sfTexture_createFromFile("Assets/Sprites/Champignon du Mordhor.png", NULL);

	mushroom = calloc(2 , sizeof(Mob));
	if (!mushroom)
	{
		fprintf(stderr, "MALLOC FAILURE");
		return;
	}
	mobCount = 2;

	for (int i = 0; i < mobCount; i++)
	{
		mushroom[i].sprite = sfSprite_create();
		sfSprite_setTexture(mushroom[i].sprite, textureMushroom, sfTrue);
		sfSprite_setOrigin(mushroom[i].sprite, (sfVector2f) { (MUSHROOM_SIZE / 2), MUSHROOM_SIZE });
		sfSprite_setScale(mushroom[i].sprite, (sfVector2f) { GAME_SCALE, GAME_SCALE });
		sfSprite_setPosition(mushroom[i].sprite, (sfVector2f) { 600 * (i + 1), 125 });

		mushroom[i].speed = 0;
		mushroom[i].velocity = (sfVector2f){ 0, 0 };
		mushroom[i].isGroundedMob = sfFalse;
		mushroom[i].timerState = 0;

		mushroom[i].rect = sfRectangleShape_create();
		sfRectangleShape_setSize(mushroom[i].rect, (sfVector2f) { HITBOX_MUSHROOM_WIDTH, HITBOX_MUSHROOM_HEIGHT });
		sfRectangleShape_setOrigin(mushroom[i].rect, (sfVector2f) { HITBOX_MUSHROOM_WIDTH / 2, HITBOX_MUSHROOM_HEIGHT });
		sfRectangleShape_setScale(mushroom[i].rect, (sfVector2f) { GAME_SCALE, GAME_SCALE });
		sfRectangleShape_setFillColor(mushroom[i].rect, sfTransparent);
		sfRectangleShape_setOutlineColor(mushroom[i].rect, sfMagenta);
		sfRectangleShape_setOutlineThickness(mushroom[i].rect, 1.f);
		sfRectangleShape_setPosition(mushroom[i].rect, sfSprite_getPosition(mushroom[i].sprite));

		mushroom[i].hitRect = sfRectangleShape_getGlobalBounds(mushroom[i].rect);


		LoadMobAnimation(&mushroom[i]);
	}
}

void LoadMobAnimation(Mob* _mob)
{
	sfIntRect firstFrame = { 0, 0, MUSHROOM_SIZE, MUSHROOM_SIZE };
	_mob->mobAnimation[IDLE_MOB] = CreateAnimation(_mob->sprite, 4, 7, sfTrue, sfTrue, firstFrame);

	firstFrame.top += MUSHROOM_SIZE;
	_mob->mobAnimation[RUN_MOB] = CreateAnimation(_mob->sprite, 8, 10, sfTrue, sfTrue, firstFrame);

	firstFrame.top += MUSHROOM_SIZE;
	_mob->mobAnimation[ATTACK_MOB] = CreateAnimation(_mob->sprite, 8, 10, sfTrue, sfFalse, firstFrame);

	firstFrame.top += MUSHROOM_SIZE;
	_mob->mobAnimation[TAKE_IT] = CreateAnimation(_mob->sprite, 3, 9, sfTrue, sfFalse, firstFrame);

	firstFrame.top += MUSHROOM_SIZE;
	_mob->mobAnimation[DEATH] = CreateAnimation(_mob->sprite, 4, 7, sfTrue, sfFalse, firstFrame);

	SetAnimationMushroom(_mob, IDLE_MOB);

}

void SetAnimationMushroom(Mob* _mob, MobState _state)
{
	_mob->lastState = _mob->currentState;
	_mob->currentState = _state;
	_mob->currentMobAnimation = &_mob->mobAnimation[_state];
	_mob->currentMobAnimation->currentFrame = 0;
	_mob->currentMobAnimation->isPlaying = sfTrue;
	_mob->currentMobAnimation->timer = 0.f;

}

void StateMobMachine(Mob* _mob, MobState _state)
{
	if (_mob->currentState == _state)
	{
		return;
	}
	else
	{
		SetAnimationMushroom(_mob, _state);
	}
}

void UpdateMob(sfRenderWindow* _renderWindow, float _dt)
{
	for (int i = 0; i < mobCount; i++)
	{

#pragma region function indi
		mushroom[i].hitRect = sfRectangleShape_getGlobalBounds(mushroom[i].rect);
		sfSprite_move(mushroom[i].sprite, (sfVector2f) { mushroom[i].velocity.x* _dt, mushroom[i].velocity.y* _dt });
		sfRectangleShape_setPosition(mushroom[i].rect, sfSprite_getPosition(mushroom[i].sprite));


#pragma endregion

		MoveMob(&mushroom[i], _dt);
		AttackMob(&mushroom[i], _dt);

		CheckVelocityY(&mushroom[i], _dt);
		CheckCollisionMobPlat(&mushroom[i], _dt);

		UpdateAnimation(mushroom[i].currentMobAnimation, _dt);

	}
}


void CheckCollisionMobPlat(Mob* _mob, float _dt)
{
	_mob->isGroundedMob = sfFalse;
	//for (int x = 0; x < mobCount; x++)
//	{
		for (unsigned i = 0; i < GetCollisionTabSize(); i++)
		{
			sfFloatRect hitPlat = GetMapCollision(i);
			_mob->hitbox = sfSprite_getGlobalBounds(_mob->sprite);

			if (sfFloatRect_intersects(&hitPlat, &_mob->hitbox, NULL))
			{
				if (_mob->velocity.y > 0)
				{
					_mob->isGroundedMob = sfTrue;
					_mob->hitbox.top = hitPlat.top - (hitPlat.height);
					_mob->velocity.y = 0;
				}
				else if (_mob->velocity.y < 0)
				{
					mushroom[0].hitbox.top = (hitPlat.top + hitPlat.height);
				}

				if ((_mob->hitbox.left + _mob->hitbox.width) > hitPlat.left && _mob->isGroundedMob)
				{
					MoveMob(_mob, _dt);
				}
				else if (_mob->hitRect.left < hitPlat.left)
				{
					sfSprite_setPosition(_mob->sprite, (sfVector2f) { hitPlat.left + (_mob->hitRect.width / 2), sfSprite_getPosition(_mob->sprite).y });
					_mob->velocity.x = 0;

				}
				else if ((_mob->hitRect.left + _mob->hitRect.width) > (hitPlat.left + hitPlat.width))
				{
					sfSprite_setPosition(_mob->sprite, (sfVector2f) { (hitPlat.left + hitPlat.width) - (_mob->hitRect.width / 2), sfSprite_getPosition(_mob->sprite).y });
					_mob->velocity.x = 0;

				}

			}
		}
//	}
}

void MoveMob(Mob* _mob, float _dt)
{
	sfVector2f posPlayer = sfSprite_getPosition(player.sprite);
	sfVector2f posMob = sfSprite_getPosition(_mob->sprite);

	float distX = posPlayer.x - posMob.x;
	if ((GetDistancePlayerMob(&player, _mob) < 400.f && GetDistancePlayerMob(&player, _mob) > -400.f) && (GetDistancePlayerMob(&player, _mob) > DIST_ATTACK || GetDistancePlayerMob(&player, _mob) < -DIST_ATTACK))
	{
		StateMobMachine(_mob, RUN_MOB);
		if (distX < 0)
		{
			sfSprite_setScale(_mob->sprite, (sfVector2f) { -GAME_SCALE, GAME_SCALE });
			_mob->velocity.x = -DIST_ATTACK;
		}
		else
		{
			sfSprite_setScale(_mob->sprite, (sfVector2f) { GAME_SCALE, GAME_SCALE });
			_mob->velocity.x = DIST_ATTACK;
		}

	}

}

void AttackMob(Mob* _mob, float _dt)
{
	_mob->timerState += _dt;
	if ((GetDistancePlayerMob(&player, _mob) < ((player.playerRect.left + player.playerRect.width) - _mob->hitRect.left) &&
		GetDistancePlayerMob(&player, _mob) > player.collisionRect.left - (_mob->hitRect.left + _mob->hitRect.width)) && _mob->timerState > 0.85f)
	{
		_mob->velocity.x = 0;
		_mob->timerState = 0.f;
		StateMobMachine(_mob, ATTACK_MOB);
	}
   
	if (!_mob->currentMobAnimation->isPlaying)
	{
		StateMobMachine(_mob, IDLE_MOB);

	}
}


float GetDistancePlayerMob(Player* _player, Mob* _mob)
{
	float distX = _player->position.x - sfSprite_getPosition(_mob->sprite).x;
	float distY = _player->position.y - sfSprite_getPosition(_mob->sprite).y;

	return sqrtf((distX * distX) - (distY * distY));

}


void DrawMob(sfRenderWindow* _renderWindow)
{
	for (int i = 0; i < mobCount; i++)
	{
		sfRenderWindow_drawRectangleShape(_renderWindow, mushroom[i].rect, NULL);
		sfRenderWindow_drawSprite(_renderWindow, mushroom[i].sprite, NULL);
	}
}

void CleanupMob(void)
{
	for (int i = 0; i < mobCount; i++)
	{
		sfSprite_destroy(mushroom[i].sprite);
		free(mushroom);
		mushroom[i] = (Mob){ NULL };
	}
}


void CheckVelocityY(Mob* _mob, float _dt)
{
	if (!_mob->isGroundedMob)
	{
		_mob->velocity.y += GRAVITY * _dt;
		if (_mob->velocity.y > GRAVITY * 100.f * _dt)
		{
			_mob->velocity.y = GRAVITY * 100.f * _dt;
		}
	}

}
