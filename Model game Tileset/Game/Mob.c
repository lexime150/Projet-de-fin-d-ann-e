#include "Mob.h"
#include "Map.h"
#include "Player.h"

Mob* mob = { 0 };

sfTexture* texture[MOB_NUMBER];
Player player;
unsigned mobCount;

void Swap(Mob* _mob, int _i, int _j);
void BubbleSort(Mob* _mob, int _i);
float GetDistancePlayerMob(Player* _player, unsigned _i);
void MobIsAttack(unsigned _i);

void SetAnimationMob(MobState _state, unsigned _i);
void StateMobMachine(MobState _state, unsigned _i);
void CheckCollisionMobPlat(float _dt, unsigned _i);
void CheckCollisionMobPlayer(float _dt, unsigned _i);
void MoveMob(float _dt, unsigned _i);
void AttackMob(float _dt, unsigned _i);
void CheckVelocityY(float _dt, unsigned _i);
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
		break;
	case SKELETON:
		sfSprite_setTexture(newMob.sprite, texture[SKELETON], sfTrue);
		sfSprite_setOrigin(newMob.sprite, (sfVector2f) { HITBOX_SKELETON_HEIGHT / 2, HITBOX_SKELETON_HEIGHT });
		sfRectangleShape_setSize(newMob.rect, (sfVector2f) { HITBOX_SKELETON_HEIGHT, HITBOX_SKELETON_HEIGHT });
		sfRectangleShape_setOrigin(newMob.rect, (sfVector2f) { HITBOX_SKELETON_HEIGHT / 2, HITBOX_SKELETON_HEIGHT });
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
		mob = realloc(mob, mobCount * (sizeof(Mob)));
		if (!mob)
		{
			fprintf(stderr, "Realloc Failure\n");
			return;
		}
		*_i--;
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
		mob[_i].mobAnimation[TAKE_IT] = CreateAnimation(mob[_i].sprite, 3, 9, sfTrue, sfFalse, firstFrame);

		firstFrame.top += MUSHROOM_SIZE;
		mob[_i].mobAnimation[DEATH] = CreateAnimation(mob[_i].sprite, 4, 7, sfTrue, sfFalse, firstFrame);
		break;
	case SKELETON:
		firstFrame = (sfIntRect){0, 0, HITBOX_SKELETON_HEIGHT, HITBOX_SKELETON_HEIGHT};
		mob[_i].mobAnimation[IDLE_MOB] = CreateAnimation(mob[_i].sprite, 3, 6, sfTrue, sfTrue, firstFrame);

		firstFrame.top += HITBOX_SKELETON_HEIGHT;
		mob[_i].mobAnimation[RUN_MOB] = CreateAnimation(mob[_i].sprite, 4, 11, sfTrue, sfTrue, firstFrame);

		firstFrame.top += HITBOX_SKELETON_HEIGHT; firstFrame.width = HITBOX_ATTACK_SKELETON_WIDTH;
		mob[_i].mobAnimation[ATTACK_MOB] = CreateAnimation(mob[_i].sprite, 6, 10, sfTrue, sfFalse, firstFrame);

		firstFrame.top += HITBOX_SKELETON_HEIGHT; firstFrame.width = HITBOX_SKELETON_HEIGHT;
		mob[_i].mobAnimation[TAKE_IT] = CreateAnimation(mob[_i].sprite, 4, 9, sfTrue, sfFalse, firstFrame);

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


void UpdateMob(sfRenderWindow* _renderWindow, float _dt)
{
	//AddMob(rand() % 2, rand() % 5000, 125);
	for (unsigned i = 0; i < mobCount; i++)
	{

		mob[i].hitRect = sfRectangleShape_getGlobalBounds(mob[i].rect);
		MoveMob(_dt, i);
		sfSprite_move(mob[i].sprite, (sfVector2f) { mob[i].velocity.x* _dt, mob[i].velocity.y* _dt });
		sfRectangleShape_setPosition(mob[i].rect, sfSprite_getPosition(mob[i].sprite));

		mob[i].position = sfSprite_getPosition(mob[i].sprite);

		CheckCollisionMobPlayer(_dt, i);
		SetVelocity(i, _dt);
		AttackMob(_dt, i);

		CheckCollisionMobPlat(_dt, i);

		UpdateAnimation(mob[i].currentMobAnimation, _dt);
	
		DeleteMob(&i);

	}
}


void CheckCollisionMobPlat(float _dt, unsigned _i)
{

	sfFloatRect hitMob = { 0 };
	sfFloatRect hitPlat = { 0 };

	for (int i = 0; i < GetCollisionTabSize(); i++)
	{
		hitPlat = GetMapCollision(i);
		hitMob = mob[_i].hitRect;

		if (sfFloatRect_intersects(&hitPlat, &hitMob, NULL))
		{
			if (mob[_i].velocity.y >= 0)
			{
				mob[_i].isGroundedMob = sfTrue;
				mob[_i].velocity.y = 0;
				sfSprite_setPosition(mob[_i].sprite, (sfVector2f) { sfSprite_getPosition(mob[_i].sprite).x, hitPlat.top});
			}
			
		}
		else
		{
			mob[_i].isGroundedMob = sfFalse;
		}
	}

}

void CheckCollisionMobPlayer(float _dt, unsigned _i)
{
	if (mob[_i].hitRect.left < (player.collisionPlayerRect.left + player.collisionPlayerRect.width))
	{
		
	}
	else if ((mob[_i].hitRect.left + mob[_i].hitRect.width) > player.collisionPlayerRect.left)
	{
		
	}


}

void MoveMob(float _dt, unsigned _i)
{

	if (GetDistancePlayerMob(&player, _i) < DIST_RUN && GetDistancePlayerMob(&player, _i) > -DIST_RUN && !mob[_i].isAttack)
	{

			StateMobMachine(RUN_MOB, _i);
			mob[_i].isMoving = sfTrue;
		
	}
	else
	{
		mob[_i].isMoving = sfFalse;
	}


}

void AttackMob(float _dt, unsigned _i)
{
	//mob[_i].timerState += _dt;
	//if ((GetDistancePlayerMob(&player, _i) < ((player.playerRect.left + player.playerRect.width) - mob[_i].hitRect.left) &&
	//	GetDistancePlayerMob(&player, _i) > player.collisionPlayerRect.left - (mob[_i].hitRect.left + mob[_i].hitRect.width)) && mob[_i].timerState > 0.85f)
	//{
	//	mob[_i].velocity.x = 0;
	//	mob[_i].timerState = 0.f;
	//	StateMobMachine(ATTACK_MOB, _i);
	//}

	//if (!mob[_i].currentMobAnimation->isPlaying)
	//{
	//	StateMobMachine(IDLE_MOB, _i);

	//}

	mob[_i].timerState += _dt;

	if ((GetDistancePlayerMob(&player, _i) < player.playerRect.width && (GetDistancePlayerMob(&player, _i) > -player.playerRect.width && mob[_i].timerState > 1.f)))
	{
		mob[_i].isAttack = sfTrue;
		MobIsAttack(_i);
	}
	else
	{
		mob[_i].isAttack = sfFalse;
	}

}


void MobIsAttack(unsigned _i)
{
	
	if (mob[_i].isAttack && mob[_i].currentState != ATTACK_MOB)
	{
		StateMobMachine(ATTACK_MOB, _i);
	}
	else if (mob[_i].currentState == ATTACK_MOB && !mob[_i].currentMobAnimation->isPlaying)
	{
		mob[_i].timerState = 0;
		mob[_i].isAttack = sfFalse;
		StateMobMachine(IDLE_MOB, _i);
	}

}


float GetDistancePlayerMob(Player* _player, unsigned _i)
{
	float distX = _player->data.position.x - sfSprite_getPosition(mob[_i].sprite).x;
	float distY = _player->data.position.y - sfSprite_getPosition(mob[_i].sprite).y;

	return distX; //sqrtf((distX * distX) + (distY * distY));

}


void DrawMob(sfRenderWindow* _renderWindow)
{
	for (unsigned i = 0; i < mobCount; i++)
	{
	//	sfRenderWindow_drawRectangleShape(_renderWindow, mob[i].rect, NULL);
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
	if (mob[_i].isMoving)
	{
		if (sfSprite_getPosition(player.sprite).x > mob[_i].position.x)
		{
			mob[_i].velocity.x = mob[_i].speed;
			sfSprite_setScale(mob[_i].sprite, (sfVector2f){GAME_SCALE, GAME_SCALE});
		}
		else 
		{
			mob[_i].velocity.x = -mob[_i].speed;
			sfSprite_setScale(mob[_i].sprite, (sfVector2f){-GAME_SCALE, GAME_SCALE});
		}
	}
	
	if (mob[_i].isAttack && !mob[_i].isMoving)
	{
		mob[_i].velocity.x = 0;
	}

	if (!mob[_i].isGroundedMob)
	{
		mob[_i].velocity.y += GRAVITY * _dt;
	}
	else
	{
		mob[_i].velocity.y = 0;
	}

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
