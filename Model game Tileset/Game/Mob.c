#include "Mob.h"
#include "Map.h"
#include "Player.h"

Mob* mob = { 0 };

sfTexture* texture[MOB_NUMBER];
Player player;
unsigned mobCount;

void Swap(Mob* _mob, int _i, int _j);
void BubbleSort(Mob* _mob, int _i);
float GetDistancePlayerMobY(unsigned _i);
float GetDistancePlayerMobX(Player* _player, unsigned _i);
float GetDistancePlayerMobVector(unsigned _i);

void UpdateMobInfo(float _dt, unsigned _i);
void SetAnimationMob(MobState _state, unsigned _i);
void StateMobMachine(MobState _state, unsigned _i);
void CheckCollisionMobEntities(float _dt, unsigned _i);

void StateMob(float _dt, unsigned _i);

void AddMob(TypeMob _type, float _x, float _y);
void SetBubbleSort(void);
void DeleteMob(unsigned* _i);
void SetVelocity(unsigned _i, float _dt);



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




	AddMob(SKELETON, GetEnemySpawn(0).x, GetEnemySpawn(0).y);
	AddMob(SKELETON, GetEnemySpawn(1).x, GetEnemySpawn(1).y);

	GetDistancePlayerMobVector(0);
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

	newMob.speed = 200.f;

	switch (newMob.mobType)
	{
	case MUSHROOM:
		sfSprite_setTexture(newMob.sprite, texture[MUSHROOM], sfTrue);
		sfSprite_setOrigin(newMob.sprite, (sfVector2f) { MUSHROOM_SIZE / 2, MUSHROOM_SIZE });
		sfRectangleShape_setSize(newMob.rect, (sfVector2f) { HITBOX_MUSHROOM_WIDTH, HITBOX_MUSHROOM_HEIGHT });
		sfRectangleShape_setOrigin(newMob.rect, (sfVector2f) { HITBOX_MUSHROOM_WIDTH / 2, HITBOX_MUSHROOM_HEIGHT });
		newMob.rangeMove = DIST_RUN_MUSHROOM;
		newMob.rangeAttack = DIST_ATTACK_MUSHROOM;
		newMob.timer.timerAttackLimit = TIMER_ATTACK_MUSHROOM;
		newMob.timer.timerTakeHitLimit = TIMER_TAKE_HIT_MUSHROOM;
		break;
	case SKELETON:
		sfSprite_setTexture(newMob.sprite, texture[SKELETON], sfTrue);
		sfSprite_setOrigin(newMob.sprite, (sfVector2f) { HITBOX_SKELETON_HEIGHT / 2, HITBOX_SKELETON_HEIGHT });
		sfRectangleShape_setSize(newMob.rect, (sfVector2f) { HITBOX_SKELETON_HEIGHT, HITBOX_SKELETON_HEIGHT });
		sfRectangleShape_setOrigin(newMob.rect, (sfVector2f) { HITBOX_SKELETON_HEIGHT / 2, HITBOX_SKELETON_HEIGHT });
		sfSprite_setTextureRect(newMob.sprite, (sfIntRect) { 0, 0, 32, 32 });
		newMob.rangeMove = DIST_RUN_MUSHROOM;
		newMob.rangeAttack = DIST_ATTACK_SKELETON;
		newMob.timer.timerAttackLimit = TIMER_ATTACK_SKELETON;
		newMob.timer.timerTakeHitLimit = TIMER_TAKE_HIT_SKELETON;
		break;
	default:
		break;
	}

	sfSprite_setScale(newMob.sprite, (sfVector2f) { GAME_SCALE, GAME_SCALE });
	sfRectangleShape_setScale(newMob.rect, (sfVector2f) { GAME_SCALE, GAME_SCALE });
	sfSprite_setPosition(newMob.sprite, (sfVector2f) { _x, _y });
	sfRectangleShape_setPosition(newMob.rect, sfSprite_getPosition(newMob.sprite));
	newMob.act = IS_IDLE;

	mob[mobCount] = newMob;
	mobCount++;

	LoadMobAnimation(mobCount - 1);
}

void DeleteMob(unsigned* _i)
{
	if (sfKeyboard_isKeyPressed(sfKeyT))
	{
		sfSprite_destroy(mob[*_i].sprite);
		sfRectangleShape_destroy(mob[*_i].rect);
		mob[*_i] = (Mob){ NULL };
		SetBubbleSort();
		mobCount--;
		Mob* temp = realloc(mob, mobCount * sizeof(Mob));
		if (!temp && mobCount > 0)
		{
			fprintf(stderr, "Realloc Failure\n");
			return;
		}

		mob = temp;
		_i--;
		for (unsigned x = 0; x < mobCount; x++)
		{
			SetAnimationMob(mob[x].currentState, x);
		}
	}
}


void LoadMobAnimation(unsigned _i)
{
	sfIntRect firstFrame = { 0 };
	switch (mob[_i].mobType)
	{
	case MUSHROOM:
		firstFrame = (sfIntRect){ 0, 0, MUSHROOM_SIZE, MUSHROOM_SIZE };
		mob[_i].mobAnimation[IDLE_MOB] = CreateAnimation(mob[_i].sprite, 4, 7, sfTrue, sfTrue, firstFrame);

		firstFrame.top += MUSHROOM_SIZE;
		mob[_i].mobAnimation[RUN_MOB] = CreateAnimation(mob[_i].sprite, 8, 10, sfTrue, sfTrue, firstFrame);

		firstFrame.top += MUSHROOM_SIZE;
		mob[_i].mobAnimation[ATTACK_MOB] = CreateAnimation(mob[_i].sprite, 8, 10, sfTrue, sfFalse, firstFrame);

		firstFrame.top += MUSHROOM_SIZE;
		mob[_i].mobAnimation[TAKE_HIT] = CreateAnimation(mob[_i].sprite, 3, 9, sfTrue, sfFalse, firstFrame);

		firstFrame.top += MUSHROOM_SIZE;
		mob[_i].mobAnimation[DEATH] = CreateAnimation(mob[_i].sprite, 4, 7, sfTrue, sfFalse, firstFrame);
		break;
	case SKELETON:
		firstFrame = (sfIntRect){ 0, 0, HITBOX_SKELETON_HEIGHT, HITBOX_SKELETON_HEIGHT };
		mob[_i].mobAnimation[IDLE_MOB] = CreateAnimation(mob[_i].sprite, 3, 6, sfTrue, sfTrue, firstFrame);

		firstFrame.top += HITBOX_SKELETON_HEIGHT;
		mob[_i].mobAnimation[RUN_MOB] = CreateAnimation(mob[_i].sprite, 4, 11, sfTrue, sfTrue, firstFrame);

		firstFrame.top += HITBOX_SKELETON_HEIGHT; firstFrame.width = HITBOX_ATTACK_SKELETON_WIDTH;
		mob[_i].mobAnimation[ATTACK_MOB] = CreateAnimation(mob[_i].sprite, 6, 10, sfTrue, sfFalse, firstFrame);

		firstFrame.top += HITBOX_SKELETON_HEIGHT; firstFrame.width = HITBOX_SKELETON_HEIGHT;
		mob[_i].mobAnimation[TAKE_HIT] = CreateAnimation(mob[_i].sprite, 4, 9, sfTrue, sfFalse, firstFrame);

		firstFrame.top += HITBOX_SKELETON_HEIGHT;
		mob[_i].mobAnimation[DEATH] = CreateAnimation(mob[_i].sprite, 4, 6, sfTrue, sfFalse, firstFrame);
		break;
	default:
		break;
	}

	for (unsigned i = 0; i < mobCount; i++)
	{
		SetAnimationMob(IDLE_MOB, i);
	}


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

void Swap(Mob* _mob, int _i, int _j)
{
	Mob temp = mob[_i];
	mob[_i] = mob[_j];
	mob[_j] = temp;
}

void BubbleSort(Mob* _mob, int _i)
{
	for (int i = 0; i < _i - 1; i++)
	{
		for (int j = 0; j < _i - i - 1; j++)
		{
			if (mob[j].sprite == NULL && mob[j + 1].sprite != NULL)
			{
				Swap(mob, j, j + 1);
			}

		}
	}
}

void SetBubbleSort()
{
	int i = mobCount;

	BubbleSort(mob, i);
}

void UpdateMobInfo(float _dt, unsigned _i)
{
	sfRectangleShape_setPosition(mob[_i].rect, sfSprite_getPosition(mob[_i].sprite));
	mob[_i].hitRect = sfRectangleShape_getGlobalBounds(mob[_i].rect);
	mob[_i].position = sfSprite_getPosition(mob[_i].sprite);

	CheckCollisionMobEntities(_dt, _i);
}

void UpdateMob(sfRenderWindow* _renderWindow, float _dt)
{
	for (unsigned i = 0; i < mobCount; i++)
	{

		UpdateMobInfo(_dt, i);
		StateMob(_dt, i);
		UpdateAnimation(mob[i].currentMobAnimation, _dt);
		DeleteMob(&i);
	}
}


void CheckCollisionMobEntities(float _dt, unsigned _i)
{
	//---------MOB PLAT----------//


	sfFloatRect hitMob = { 0 };
	sfFloatRect hitPlat = { 0 };
	sfFloatRect intersection = { 0 };
	sfVector2f posMob = sfSprite_getPosition(mob[_i].sprite);

	for (unsigned i = 0; i < GetCollisionTabSize(); i++)
	{
		hitPlat = GetMapCollision(i);
		hitMob = mob[_i].hitRect;

		if (sfFloatRect_intersects(&hitPlat, &hitMob, &intersection))
		{
			if (intersection.width < intersection.height)
			{
				if (mob[_i].velocity.x < 0.f)
				{
					posMob.x += intersection.width;

				}
				else if (mob[_i].velocity.x > 0.f)
				{

					posMob.x -= intersection.width;

				}
				StateMobMachine(IDLE_MOB, _i);

			}
			else if (hitMob.left < hitPlat.left)
			{

				posMob.x = hitPlat.left + (hitMob.width / 2);
				StateMobMachine(IDLE_MOB, _i);
			}
			else if ((hitMob.left + hitMob.width) > (hitPlat.left + hitPlat.width))
			{
				StateMobMachine(IDLE_MOB, _i);

				posMob.x = (hitPlat.left + hitPlat.width) - (hitMob.width / 2);
			}


			sfSprite_setPosition(mob[_i].sprite, posMob);
		}


	}


	for (unsigned i = 0; i < GetCollisionTabSize(); i++)
	{
		hitPlat = GetMapCollision(i);
		hitMob = mob[_i].hitRect;


		if (sfFloatRect_intersects(&hitPlat, &hitMob, &intersection))
		{
			if (intersection.width > intersection.height)
			{
				if (mob[_i].velocity.y >= 0)
				{
					mob[_i].act = IS_GROUNDED_MOB;
					mob[_i].velocity.y = 0;
					sfSprite_setPosition(mob[_i].sprite, (sfVector2f) { sfSprite_getPosition(mob[_i].sprite).x, hitPlat.top + 1.f });
				}
			}
		}

	}




}

void StateMob(float _dt, unsigned _i)
{
	//-------MOVE PLAYER---------//

	mob[_i].timer.timerTakeHit += _dt;
	mob[_i].timer.timerAttack += _dt;

	
	
		if (sfSprite_getPosition(player.sprite).x >= mob[_i].position.x)
		{

			sfSprite_setScale(mob[_i].sprite, (sfVector2f) { GAME_SCALE, GAME_SCALE });
		}
		else
		{

			sfSprite_setScale(mob[_i].sprite, (sfVector2f) { -GAME_SCALE, GAME_SCALE });
		}
	

		if (mob[_i].currentState != TAKE_HIT && mob[_i].currentMobAnimation->isPlaying)
		{
			if (GetDistancePlayerMobVector(_i) < mob[_i].rangeMove && GetDistancePlayerMobY(_i) < (player.shape.collisionPlayerRect.height * 2))
			{
				if (GetDistancePlayerMobX(&player, _i) > (player.shape.collisionPlayerRect.width))
				{
						StateMobMachine(RUN_MOB, _i);
						mob[_i].act = IS_MOVING;
					
				}
				else
				{
					mob[_i].act = IS_ATTACK;
				}
			}
			else if (GetDistancePlayerMobY(_i) > (player.shape.collisionPlayerRect.height * 2))
			{
				StateMobMachine(IDLE_MOB, _i);
				mob[_i].act = IS_IDLE;
			}
			

			//---------ATTACK MOB---------//




			if (mob[_i].act == IS_ATTACK && mob[_i].timer.timerAttack > mob[_i].timer.timerAttackLimit)
			{

				StateMobMachine(ATTACK_MOB, _i);
				mob[_i].timer.timerAttack = 0;
			}
			else if (!mob[_i].currentMobAnimation->isPlaying && mob[_i].act != IS_TAKE_HIT)
			{
				mob[_i].act = IS_IDLE;
				StateMobMachine(IDLE_MOB, _i);
			}



			//---------TAKE IT---------//


			if (player.currentState == SWORD || player.currentState == AXE)
			{
				sfFloatRect hitAttackPlayer = sfRectangleShape_getGlobalBounds(player.shape.collisionAttackShape);
				sfFloatRect hitMob = sfRectangleShape_getGlobalBounds(mob[_i].rect);
				if (sfFloatRect_intersects(&hitAttackPlayer, &hitMob, NULL))
				{
					if (mob[_i].timer.timerTakeHit > mob[_i].timer.timerTakeHitLimit && mob[_i].act != IS_ATTACK)
					{
						printf("wn");
						mob[_i].hp -= (50 + rand() % 51);
						StateMobMachine(TAKE_HIT, _i);
						mob[_i].act = IS_TAKE_HIT;
						mob[_i].timer.timerTakeHit = 0;
					}
					else if (mob[_i].act == IS_TAKE_HIT)
					{
						StateMobMachine(IDLE_MOB, _i);
						mob[_i].act = IS_IDLE;
					}

				}
			}

		}
		else if((mob[_i].currentState == TAKE_HIT || mob[_i].currentState == ATTACK_MOB) && !mob[_i].currentMobAnimation->isPlaying)
		{
			StateMobMachine(IDLE_MOB, _i);
			mob[_i].act = IS_IDLE;
		}

	SetVelocity(_i, _dt);
}


float GetDistancePlayerMobX(Player* _player, unsigned _i)
{
	float distX = _player->data.position.x - sfSprite_getPosition(mob[_i].sprite).x;

	if (distX < 0.f)
	{
		return distX * -1;
	}
	else
	{
		return distX;
	}
}

float GetDistancePlayerMobY(unsigned _i)
{
	float distY = player.data.position.y - sfSprite_getPosition(mob[_i].sprite).y;

	if (distY < 0)
	{
		return distY * -1;
	}
	else
	{
		return distY;
	}
}

float GetDistancePlayerMobVector(unsigned _i)
{
	return sqrtf(powf(GetDistancePlayerMobX(&player, _i), 2.f) + powf(GetDistancePlayerMobY(_i), 2.f));
}

void DrawMob(sfRenderWindow* _renderWindow)
{
	for (unsigned i = 0; i < mobCount; i++)
	{
		sfRenderWindow_drawRectangleShape(_renderWindow, mob[i].rect, NULL);
		sfRenderWindow_drawSprite(_renderWindow, mob[i].sprite, NULL);
	}
}

void CleanupMob(void)
{
	sfTexture_destroy(texture[MUSHROOM]);
	sfTexture_destroy(texture[SKELETON]);
	texture[MUSHROOM] = NULL;
	texture[SKELETON] = NULL;


	for (unsigned i = 0; i < mobCount; i++)
	{
		sfSprite_destroy(mob[i].sprite);
		sfRectangleShape_destroy(mob[i].rect);
		mob[i] = (Mob){ 0 };
	}
	mobCount = 0;
	free(mob);
	mob = (Mob*){ NULL };
}

void SetVelocity(unsigned _i, float _dt)
{

	if (mob[_i].act == IS_MOVING)
	{
		if (sfSprite_getPosition(player.sprite).x > mob[_i].position.x)
		{
			mob[_i].velocity.x = mob[_i].speed;
		}
		else
		{
			mob[_i].velocity.x = -mob[_i].speed;
		}
	}

	if (mob[_i].act == IS_IDLE || mob[_i].act == IS_ATTACK || mob[_i].act == IS_TAKE_HIT)
	{
		mob[_i].velocity.x = 0;
	}

	if (mob[_i].act != IS_GROUNDED_MOB)
	{
		mob[_i].velocity.y += GRAVITY * _dt;
	}
	else
	{
		mob[_i].velocity.y = 0;
	}

	sfSprite_move(mob[_i].sprite, (sfVector2f) { mob[_i].velocity.x* _dt, mob[_i].velocity.y* _dt });

}





