#include "Mob.h"
#include "Map.h"
#include "Player.h"

Mob* mushroom = { 0 };
sfTexture* textureMushroom;
Player player;

void SetAnimationMushroom(Mob* _mob, MobState _state);
void StateMobMachine(Mob* _mob, MobState _state);
void CheckCollisionMobPlat(Mob* _mob, float _dt);
void CheckDistanceMobPlayer(Mob* _mob, float _dt);

void CheckVelocityY(float _dt);



void LoadMob(void)
{
	 textureMushroom = sfTexture_createFromFile("Assets/Sprites/Champignon du Mordhor.png", NULL);

	 mushroom = malloc(sizeof(Mob));
	 if (!mushroom)
	 {
		 fprintf(stderr, "MALLOC FAILURE");
	 }
	 mushroom[0].sprite = sfSprite_create();
	 sfSprite_setTexture(mushroom[0].sprite, textureMushroom, sfTrue);
	 sfSprite_setOrigin(mushroom[0].sprite, (sfVector2f) { (MUSHROOM_SIZE / 2), MUSHROOM_SIZE});
	 sfSprite_setScale(mushroom[0].sprite, (sfVector2f){GAME_SCALE, GAME_SCALE});
	 sfSprite_setPosition(mushroom[0].sprite, (sfVector2f){600, 125});

	 mushroom[0].speed = 0;
	 mushroom[0].velocity = (sfVector2f){0, 0};
	 mushroom[0].isGroundedMob = sfFalse;
	 
	 mushroom[0].rect = sfRectangleShape_create();
	 sfRectangleShape_setSize(mushroom[0].rect, (sfVector2f){HITBOX_MUSHROOM_WIDTH, HITBOX_MUSHROOM_HEIGHT});
	 sfRectangleShape_setOrigin(mushroom[0].rect, (sfVector2f){HITBOX_MUSHROOM_WIDTH / 2, HITBOX_MUSHROOM_HEIGHT});
	 sfRectangleShape_setScale(mushroom[0].rect, (sfVector2f){GAME_SCALE, GAME_SCALE});
	 sfRectangleShape_setFillColor(mushroom[0].rect, sfTransparent);
	 sfRectangleShape_setOutlineColor(mushroom[0].rect, sfMagenta);
	 sfRectangleShape_setOutlineThickness(mushroom[0].rect, 1.f);
	 sfRectangleShape_setPosition(mushroom[0].rect, sfSprite_getPosition(mushroom[0].sprite));

	 mushroom[0].hitRect = sfRectangleShape_getGlobalBounds(mushroom[0].rect);

	 
	 LoadMobAnimation(&mushroom[0]);
}

void LoadMobAnimation(Mob* _mob)
{
	sfIntRect firstFrame = {0, 0, MUSHROOM_SIZE, MUSHROOM_SIZE };
	_mob->mobAnimation[IDLE_MOB] = CreateAnimation(mushroom[0].sprite, 4, 7, sfTrue, sfTrue, firstFrame);

	firstFrame.top += MUSHROOM_SIZE;
	_mob->mobAnimation[RUN_MOB] = CreateAnimation(mushroom[0].sprite, 8, 10, sfTrue, sfTrue, firstFrame);

	firstFrame.top += MUSHROOM_SIZE;
	_mob->mobAnimation[ATTACK_MOB] = CreateAnimation(mushroom[0].sprite, 8, 10, sfTrue, sfFalse, firstFrame);

	firstFrame.top += MUSHROOM_SIZE;
	_mob->mobAnimation[TAKE_IT] = CreateAnimation(mushroom[0].sprite, 3, 9, sfTrue, sfFalse, firstFrame);

	firstFrame.top += MUSHROOM_SIZE;
	_mob->mobAnimation[DEATH] = CreateAnimation(mushroom[0].sprite, 4, 7, sfTrue, sfFalse, firstFrame);

	SetAnimationMushroom(_mob, IDLE_MOB);

}

void SetAnimationMushroom(Mob* _mob, MobState _state)
{
	_mob->lastState = mushroom[0].currentState;
	_mob->currentState = _state;
	_mob->currentMobAnimation = &mushroom[0].mobAnimation[_state];
	_mob->currentMobAnimation->currentFrame = 0;
	_mob->currentMobAnimation->isPlaying = sfTrue;
	_mob->currentMobAnimation->timer = 0.f;

}

void StateMobMachine(Mob* _mob, MobState _state)
{
	if (mushroom[0].currentState == _state)
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

#pragma region function indi
	mushroom[0].hitRect = sfRectangleShape_getGlobalBounds(mushroom[0].rect);
	sfSprite_move(mushroom[0].sprite, (sfVector2f) { mushroom[0].velocity.x* _dt, mushroom[0].velocity.y* _dt });
	sfRectangleShape_setPosition(mushroom[0].rect, sfSprite_getPosition(mushroom[0].sprite));
#pragma endregion

	CheckVelocityY(_dt);
	CheckCollisionMobPlat(&mushroom[0], _dt);

	UpdateAnimation(mushroom[0].currentMobAnimation, _dt);
}


void CheckCollisionMobPlat(Mob* _mob, float _dt)
{
	mushroom[0].isGroundedMob = sfFalse;

	for (unsigned i = 0; i < GetCollisionTabSize(); i++)
	{
		sfFloatRect hitPlat = GetMapCollision(i);
		mushroom[0].hitbox = sfSprite_getGlobalBounds(mushroom[0].sprite);
		
		if (sfFloatRect_intersects(&hitPlat, &mushroom[0].hitbox, NULL))
		{
			if (mushroom[0].velocity.y > 0)
			{
				mushroom[0].isGroundedMob = sfTrue;
				mushroom[0].hitbox.top = hitPlat.top - (hitPlat.height);
				mushroom[0].velocity.y = 0;
			}
			else if (mushroom[0].velocity.y < 0)
			{
				mushroom[0].hitbox.top = (hitPlat.top + hitPlat.height);
			}

			if ((mushroom[0].hitbox.left + mushroom[0].hitbox.width) > hitPlat.left && mushroom[0].isGroundedMob)
			{
				CheckDistanceMobPlayer(_mob, _dt);
			}	
			else if (mushroom[0].hitRect.left < hitPlat.left)
			{
				sfSprite_setPosition(mushroom[0].sprite, (sfVector2f){hitPlat.left + (mushroom[0].hitRect.width / 2), sfSprite_getPosition(mushroom[0].sprite).y});
				mushroom[0].velocity.x = 0;
				
			}
			else if ((mushroom[0].hitRect.left + mushroom[0].hitRect.width) > (hitPlat.left + hitPlat.width))
			{
				sfSprite_setPosition(mushroom[0].sprite, (sfVector2f){(hitPlat.left + hitPlat.width) - (mushroom[0].hitRect.width / 2), sfSprite_getPosition(mushroom[0].sprite).y});
				mushroom[0].velocity.x = 0;
				
			}

		}
	}

}

void CheckDistanceMobPlayer(Mob* _mob, float _dt)
{
	sfVector2f posPlayer = sfSprite_getPosition(player.sprite);
	sfVector2f posMob = sfSprite_getPosition(mushroom[0].sprite);

	float distX = posPlayer.x - posMob.x;
	if (distX < 400.f && distX > -400.f)
	{
		if (distX > 100.f || distX < -100.f)
		{
			StateMobMachine(_mob, RUN_MOB);

			if (distX < 0)
			{
				sfSprite_setScale(mushroom[0].sprite, (sfVector2f) { -GAME_SCALE, GAME_SCALE });
				mushroom[0].velocity.x = -100.f;
			}
			else
			{
				sfSprite_setScale(mushroom[0].sprite, (sfVector2f) { GAME_SCALE, GAME_SCALE });
				mushroom[0].velocity.x = 100.f;
			}
		}
		else //if(distX < 100.f && distX > -100.f)
		{
			StateMobMachine(_mob, ATTACK_MOB);
			mushroom[0].velocity.x = 0;
		}
	}
	else if (distX < 100.f && distX > -100.f)
	{
		StateMobMachine(_mob, ATTACK_MOB);

	}
	else
	{
		mushroom[0].velocity.x = 0;
		StateMobMachine(_mob, IDLE_MOB);
	}
}


void DrawMob(sfRenderWindow* _renderWindow)
{
	//sfRenderWindow_drawRectangleShape(_renderWindow, mushroom.rect, NULL);
	sfRenderWindow_drawSprite(_renderWindow, mushroom[0].sprite, NULL);
}

void CleanupMob(void)
{
	sfSprite_destroy(mushroom[0].sprite);
	mushroom[0] = (Mob){NULL};
}


void CheckVelocityY(float _dt)
{
	if (!mushroom[0].isGroundedMob)
	{
		mushroom[0].velocity.y += GRAVITY * _dt;
		if (mushroom[0].velocity.y > GRAVITY * 100.f * _dt)
		{
			mushroom[0].velocity.y = GRAVITY * 100.f * _dt;
		}
	}

}
