#include "Mob.h"
#include "Map.h"
#include "Player.h"

Mob* mob = { 0 };

sfTexture* texture[MOB_NUMBER];
sfSoundBuffer* mushroomHitBuffer;

Player* player;
unsigned mobCount;
char* adresse[MOB_NUMBER];

void Swap(Mob* _mob, int _i, int _j);
void BubbleSort(Mob* _mob, int _i);

void UpdateMobInfo(float _dt, unsigned _i);
void SetAnimationMob(MobState _state, unsigned _i);
void CheckCollisionMobEntities(float _dt, unsigned _i);

void StateMob(float _dt, unsigned _i);

void AddMob(TypeMob _type, float _x, float _y);
void SetBubbleSort(void);
void DeleteMob(unsigned* _i);
void SetVelocity(unsigned _i, float _dt);

sfBool GetDistanceMobSpike(unsigned _mob, unsigned _spike);



float RandomFloatMob(float min, float max)
{
	return min + (float)rand() / (float)RAND_MAX * (max - min);
}

void LoadMob(void)
{
	texture[MUSHROOM] = sfTexture_createFromFile("Assets/Sprites/Game/Mob/Champignon du Mordhor.png", NULL);
	texture[SKELETON] = sfTexture_createFromFile("Assets/Sprites/Game/Mob/skeleton.png", NULL);



	mob = malloc(sizeof(Mob));
	if (!mob)
	{
		fprintf(stderr, "MALLOC FAILURE");
		return;
	}




	//for (unsigned i = 0; i < GetEnemySpawnTabSize(); i++)
	//{
	//	int randMobType = rand() % 2;
	//	AddMob(randMobType, GetEnemySpawn(i).x, GetEnemySpawn(i).y);
	//}

	for (unsigned i = 0; i < GetMushroomSpawnTabSize(); i++)
	{
		AddMob(MUSHROOM, GetMushroomSpawn(i).x, GetMushroomSpawn(i).y);
	}

	for (unsigned i = 0; i < GetSkeletonSpawnTabSize(); i++)
	{
		AddMob(SKELETON, GetSkeletonSpawn(i).x, GetSkeletonSpawn(i).y);
	}

	for (unsigned i = 0; i < GetEnemySpawnTabSize(); i++)
	{
		AddMob(rand() % 2, GetEnemySpawn(i).x, GetEnemySpawn(i).y);
	}
	if (mobCount > 0)
	{
		GetDistancePlayerMobVector(0);

	}
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

	CreateSprite(texture[_type], &newMob.sprite, ORIGIN_CENTER_X, (sfVector2f) { _x, _y });

	
	newMob.shape.attackRect = sfRectangleShape_create();

	newMob.soundMob.soundAttack = sfSound_create();
	newMob.soundMob.soundDead = sfSound_create();
	newMob.soundMob.soundTakeHit = sfSound_create();

	newMob.data = (Data){ 0 };
	newMob.data.speed = 200.f;

	switch (newMob.mobType)
	{
	case MUSHROOM:
		sfSprite_setOrigin(newMob.sprite, (sfVector2f) { MUSHROOM_SIZE_SPRITE / 2, MUSHROOM_SIZE_SPRITE });

		newMob.shape.rect = CreateRectangle((sfVector2f) { HITBOX_MUSHROOM_WIDTH, HITBOX_MUSHROOM_HEIGHT }, (sfVector2f){ HITBOX_MUSHROOM_WIDTH / 2.f, HITBOX_MUSHROOM_HEIGHT }, (sfVector2f) { GAME_SCALE, GAME_SCALE }, sfYellow);
		newMob.shape.collisionRect = CreateRectangle((sfVector2f) { COLLISION_MUSHROOM_WIDTH, COLLISION_MUSHROOM_HEIGHT }, (sfVector2f){ COLLISION_MUSHROOM_WIDTH / 2.f, COLLISION_MUSHROOM_HEIGHT }, (sfVector2f){GAME_SCALE}, sfMagenta);
	

		newMob.rangeMove = DIST_RUN_MUSHROOM;
		newMob.rangeAttack = DIST_ATTACK_MUSHROOM;
		newMob.data.timerAttackLimit = TIMER_ATTACK_MUSHROOM;
		newMob.data.timerTakeHitLimit = TIMER_TAKE_HIT_MUSHROOM;
		newMob.data.hp = 300;
		newMob.damage = MUSHROOM_DAMAGE;
		//----AttackRect

		sfRectangleShape_setSize(newMob.shape.attackRect, (sfVector2f) { HITBOX_MUSHROOM_ATTACK_WIDTH, HITBOX_MUSHROOM_HEIGHT });
		sfRectangleShape_setOrigin(newMob.shape.attackRect, (sfVector2f) { HITBOX_MUSHROOM_ATTACK_WIDTH / 2, HITBOX_MUSHROOM_HEIGHT });
		newMob.mobType = MUSHROOM;

		newMob.soundMob.soundBufferTakeHit = sfSoundBuffer_createFromFile("Assets/Audio/Sounds/Mobs/Mushroom hurt.ogg");
		newMob.soundMob.soundBufferAttack = sfSoundBuffer_createFromFile("Assets/Audio/Sounds/Mobs/Mushroom attack.ogg");
		newMob.soundMob.soundBufferDead = sfSoundBuffer_createFromFile("Assets/Audio/Sounds/Mobs/Mushroom dead.ogg");

		newMob.frameAttackSound = MUSHROOM_ATTACK_SOUND;

		break;
	case SKELETON:

		newMob.shape.rect = CreateRectangle((sfVector2f){ HITBOX_SKELETON_WIDTH - 10.f, HITBOX_SKELETON_HEIGHT }, (sfVector2f){ (HITBOX_SKELETON_WIDTH - 10.f) / 2.f, HITBOX_SKELETON_HEIGHT }, (sfVector2f) { GAME_SCALE, GAME_SCALE }, sfYellow);
		newMob.shape.collisionRect = CreateRectangle((sfVector2f){ HITBOX_SKELETON_WIDTH, HITBOX_SKELETON_WIDTH }, (sfVector2f){ HITBOX_SKELETON_WIDTH / 2.f, HITBOX_SKELETON_WIDTH }, (sfVector2f){GAME_SCALE}, sfColor_fromRGBA(24, 72, 185, 125));

		sfSprite_setOrigin(newMob.sprite, (sfVector2f) { HITBOX_SKELETON_WIDTH / 2.f, HITBOX_SKELETON_WIDTH });
		sfSprite_setTextureRect(newMob.sprite, (sfIntRect) { 0, 0, 32, 32 });

		newMob.soundMob.soundBufferAttack = sfSoundBuffer_createFromFile("Assets/Audio/Sounds/Mobs/skeleton attack.ogg");
		newMob.soundMob.soundBufferTakeHit = sfSoundBuffer_createFromFile("Assets/Audio/Sounds/Mobs/Skeleton damage.ogg");
		newMob.soundMob.soundBufferDead = sfSoundBuffer_createFromFile("Assets/Audio/Sounds/Mobs/skeleton dying.ogg");

		newMob.rangeMove = DIST_RUN_MUSHROOM;
		newMob.rangeAttack = DIST_ATTACK_SKELETON;
		newMob.data.timerAttackLimit = TIMER_ATTACK_SKELETON;
		newMob.data.timerTakeHitLimit = TIMER_TAKE_HIT_SKELETON;
		newMob.data.hp = 180;
		newMob.damage = SKELETON_DAMAGE;

		newMob.isGrounded = sfFalse;

		//----Attack Rect

		sfRectangleShape_setSize(newMob.shape.attackRect, (sfVector2f) { HITBOX_ATTACK_SKELETON, HITBOX_SKELETON_HEIGHT });
		sfRectangleShape_setOrigin(newMob.shape.attackRect, (sfVector2f) { HITBOX_ATTACK_SKELETON / 2, HITBOX_SKELETON_HEIGHT });
		newMob.mobType = SKELETON;

		newMob.frameAttackSound = SKELETON_ATTACK_SOUND;

		break;
	default:
		break;
	}

	newMob.data.timerAttack = newMob.data.timerAttackLimit;

	//---Attack Rect
	sfRectangleShape_setFillColor(newMob.shape.attackRect, sfTransparent);
	sfRectangleShape_setOutlineColor(newMob.shape.attackRect, sfMagenta);
	sfRectangleShape_setOutlineThickness(newMob.shape.attackRect, 1.f);
	sfRectangleShape_setScale(newMob.shape.attackRect, (sfVector2f) { GAME_SCALE, GAME_SCALE });
	sfRectangleShape_setPosition(newMob.shape.attackRect, sfSprite_getPosition(newMob.sprite));


	sfSprite_setScale(newMob.sprite, (sfVector2f) { GAME_SCALE, GAME_SCALE });
//	sfRectangleShape_setScale(newMob.shape.rect, (sfVector2f) { GAME_SCALE, GAME_SCALE });
	sfSprite_setPosition(newMob.sprite, (sfVector2f) { _x, _y });
	sfRectangleShape_setPosition(newMob.shape.rect, sfSprite_getPosition(newMob.sprite));

	sfRectangleShape_setScale(newMob.shape.collisionRect, (sfVector2f) { GAME_SCALE, GAME_SCALE });

	newMob.shape.floorSecurity = sfRectangleShape_create();
	sfRectangleShape_setSize(newMob.shape.floorSecurity, FLOOR_SECURITY_SIZE);
	sfRectangleShape_setOrigin(newMob.shape.floorSecurity, (sfVector2f) { FLOOR_SECURITY_SIZE.x / 2, 0 });
	sfRectangleShape_setScale(newMob.shape.floorSecurity, (sfVector2f) { GAME_SCALE, GAME_SCALE });


	sfSound_setBuffer(newMob.soundMob.soundTakeHit, newMob.soundMob.soundBufferTakeHit);
	sfSound_setBuffer(newMob.soundMob.soundAttack, newMob.soundMob.soundBufferAttack);
	sfSound_setBuffer(newMob.soundMob.soundDead, newMob.soundMob.soundBufferDead);


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
		sfRectangleShape_destroy(mob[*_i].shape.rect);
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
		firstFrame = (sfIntRect){ 0, 0, MUSHROOM_SIZE_SPRITE, MUSHROOM_SIZE_SPRITE };
		mob[_i].mobAnimation[IDLE_MOB] = CreateAnimation(mob[_i].sprite, 4, 8, sfTrue, sfTrue, firstFrame);

		firstFrame.top += MUSHROOM_SIZE_SPRITE;
		mob[_i].mobAnimation[RUN_MOB] = CreateAnimation(mob[_i].sprite, 8, 10, sfTrue, sfTrue, firstFrame);

		firstFrame.top += MUSHROOM_SIZE_SPRITE;
		mob[_i].mobAnimation[ATTACK_MOB] = CreateAnimation(mob[_i].sprite, 8, 10, sfTrue, sfFalse, firstFrame);

		firstFrame.top += MUSHROOM_SIZE_SPRITE;
		mob[_i].mobAnimation[TAKE_HIT] = CreateAnimation(mob[_i].sprite, 3, 9, sfTrue, sfFalse, firstFrame);

		firstFrame.top += MUSHROOM_SIZE_SPRITE;
		mob[_i].mobAnimation[DEATH] = CreateAnimation(mob[_i].sprite, 4, 7, sfTrue, sfFalse, firstFrame);
		break;
	case SKELETON:
		firstFrame = (sfIntRect){ 0, 0, HITBOX_SKELETON_WIDTH, HITBOX_SKELETON_WIDTH };
		mob[_i].mobAnimation[IDLE_MOB] = CreateAnimation(mob[_i].sprite, 3, 10, sfTrue, sfTrue, firstFrame);

		firstFrame.top += HITBOX_SKELETON_WIDTH;
		mob[_i].mobAnimation[RUN_MOB] = CreateAnimation(mob[_i].sprite, 4, 11, sfTrue, sfTrue, firstFrame);

		firstFrame.top += HITBOX_SKELETON_WIDTH; firstFrame.width = HITBOX_ATTACK_STATE_SKELETON_WIDTH;
		mob[_i].mobAnimation[ATTACK_MOB] = CreateAnimation(mob[_i].sprite, 6, 8, sfTrue, sfFalse, firstFrame);

		firstFrame.top += HITBOX_SKELETON_WIDTH; firstFrame.width = HITBOX_SKELETON_WIDTH;
		mob[_i].mobAnimation[TAKE_HIT] = CreateAnimation(mob[_i].sprite, 4, 9, sfTrue, sfFalse, firstFrame);

		firstFrame.top += HITBOX_SKELETON_WIDTH;
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
	sfVector2f posMob = sfSprite_getPosition(mob[_i].sprite);

	sfRectangleShape_setPosition(mob[_i].shape.rect, posMob);
	mob[_i].shape.hitRect = sfRectangleShape_getGlobalBounds(mob[_i].shape.rect);
	mob[_i].data.position = sfSprite_getPosition(mob[_i].sprite);

	//----AttackRect
	sfRectangleShape_setPosition(mob[_i].shape.attackRect, posMob);
	sfRectangleShape_setPosition(mob[_i].shape.collisionRect, posMob);
	sfRectangleShape_setPosition(mob[_i].shape.floorSecurity, posMob);

}

void UpdateMob(sfRenderWindow* _renderWindow, float _dt)
{
	for (unsigned i = 0; i < mobCount; i++)
	{

		StateMob(_dt, i);
		CheckCollisionMobEntities(_dt, i);
		UpdateMobInfo(_dt, i);
		UpdateAnimation(mob[i].currentMobAnimation, _dt);
		DeleteMob(&i);

	}
}

static void CheckMobSpikeCollision(float _dt, unsigned _i)
{
	MobSide side = NOTHING_MOB;
	sfFloatRect hitMob = sfRectangleShape_getGlobalBounds(mob[_i].shape.collisionRect);
	sfFloatRect hitSpike = { 0 }, intersects = { 0 };
	float posMobY = hitMob.top + hitMob.height, posMobX = 0;


	UpdateMobInfo(_dt, _i);



	for (unsigned i = 0; i < GetSpikeTabSize(); i++)
	{
		hitSpike = GetSpikeTab(i);
		if (sfFloatRect_intersects(&hitMob, &hitSpike, &intersects))
		{
			if ((hitSpike.top + (hitSpike.height / 2) > (hitMob.top + hitMob.height)))
			{
				side = HEIGHT_MOB;
				posMobY -= intersects.height;
				break;
			}
		}

	}

	if (side == HEIGHT_MOB)
	{
		mob[_i].data.velocity.y = 0;
		sfSprite_setPosition(mob[_i].sprite, (sfVector2f) { sfSprite_getPosition(mob[_i].sprite).x, posMobY });
		mob[_i].data.hp = 0;
	}

}

void CheckCollisionMobEntities(float _dt, unsigned _i)
{
	mob[_i].isGrounded = sfFalse;

	sfBool platTransition = sfFalse;
	sfFloatRect hitMob = { 0 }, hitPlat = { 0 }, hitSemiPlat = { 0 }, intersection = { 0 }, hitDeathZone = { 0 };
	sfVector2f posMob = sfSprite_getPosition(mob[_i].sprite);

	hitMob = mob[_i].shape.hitRect;

	for (unsigned i = 0; i < GetDeathZoneTabSize(); i++)
	{
		hitDeathZone = GetDeathZoneTab(i);

		if (sfFloatRect_intersects(&hitMob, &hitDeathZone, NULL))
		{
			mob[_i].data.hp = 0;
			break;
		}


	}

	//-----TRANSITION PLATFORMS-----//

	UpdateMobInfo(_dt, _i);

	if (mob[_i].currentState != DEATH)
	{
		for (unsigned i = 0; i < GetCollisionTabSize(); i++)
		{

			hitPlat = GetMapCollision(i);

			for (unsigned x = 0; x < GetSemiSolidCollisionTabSize(); x++)
			{
				hitSemiPlat = GetSemiSolidCollisionTab(x);
				if (sfFloatRect_intersects(&hitMob, &hitPlat, NULL) && sfFloatRect_intersects(&hitMob, &hitSemiPlat, NULL))
				{
					platTransition = sfTrue;
					break;
				}

				if (platTransition)
				{
					break;
				}
			}

		}
	}

	UpdateMobInfo(_dt, _i);
	CheckMobSpikeCollision(_dt, _i);

	//-----SEMI-SOLID PLATFORMS-----//

	for (unsigned i = 0; i < GetSemiSolidCollisionTabSize(); i++)
	{
		hitPlat = GetSemiSolidCollisionTab(i);
		hitMob = mob[_i].shape.hitRect;

		if (sfFloatRect_intersects(&hitMob, &hitPlat, &intersection))
		{
			if (intersection.width < intersection.height && !platTransition)
			{
				if (mob[_i].data.velocity.x > 0)
				{
					posMob.x -= intersection.width;
				}
				else if (mob[_i].data.velocity.x < 0)
				{
					posMob.x += intersection.width;
				}

			}

			if (intersection.width > intersection.height)
			{
				if (mob[_i].data.velocity.y < 0.f)
				{
					posMob.y += intersection.height;
				}
				else
				{
					posMob.y -= intersection.height;
				}
			}

			sfSprite_setPosition(mob[_i].sprite, posMob);
		}

	}
	UpdateMobInfo(_dt, _i);

	//-----PLATFORMS-----//

	sfFloatRect hitFloorMob = sfRectangleShape_getGlobalBounds(mob[_i].shape.floorSecurity);


	for (unsigned i = 0; i < GetCollisionTabSize(); i++)
	{
		hitPlat = GetMapCollision(i);
		hitMob = mob[_i].shape.hitRect;
		float dy = (hitMob.top + hitMob.height) - hitPlat.top;

		if (sfFloatRect_intersects(&hitPlat, &hitMob, &intersection) && mob[_i].currentState != DEATH)
		{

			if (intersection.width > intersection.height && dy < 20.f)
			{
				if (mob[_i].currentState == RUN_MOB && mob[_i].data.timerTakeHit > 1.5f)
				{
					if (mob[_i].data.velocity.x < 0 && hitMob.left < hitPlat.left && (hitFloorMob.left + hitFloorMob.width) > hitPlat.left)
					{
						posMob.x = hitPlat.left + (hitMob.width / 2);
						//StateMobMachine(IDLE_MOB, _i);
					}
					else if (mob[_i].data.velocity.x > 0 && (hitMob.left + hitMob.width) > (hitPlat.left + hitPlat.width) && (hitFloorMob.left < (hitPlat.left + hitPlat.width)))
					{
						//StateMobMachine(IDLE_MOB, _i);
						posMob.x = (hitPlat.left + hitPlat.width) - (hitMob.width / 2);
					}
				}
			}
			else
			{
				posMob.x += intersection.width;
			}

			sfSprite_setPosition(mob[_i].sprite, posMob);
		}


	}

	UpdateMobInfo(_dt, _i);

	for (unsigned i = 0; i < GetCollisionTabSize(); i++)
	{
		hitPlat = GetMapCollision(i);
		hitMob = mob[_i].shape.hitRect;


		if (sfFloatRect_intersects(&hitPlat, &hitMob, &intersection))
		{
			if (intersection.width > intersection.height)
			{
				if (mob[_i].data.velocity.y >= 0)
				{

					mob[_i].isGrounded = sfTrue;
					mob[_i].data.velocity.y = 0;
					sfSprite_setPosition(mob[_i].sprite, (sfVector2f) { sfSprite_getPosition(mob[_i].sprite).x, hitPlat.top + 1.f });
				}
			}
		}

	}

	UpdateMobInfo(_dt, _i);

	for (unsigned i = 0; i < GetSemiSolidCollisionTabSize(); i++)
	{
		hitSemiPlat = GetSemiSolidCollisionTab(i);
		hitMob = mob[_i].shape.hitRect;

		mob[_i].isGrounded = sfFalse;

		if (sfFloatRect_intersects(&hitSemiPlat, &hitMob, &intersection))
		{
			if (mob[_i].data.velocity.y > 0)
			{

				mob[_i].isGrounded = sfTrue;
				mob[_i].data.velocity.y = 0;
				posMob.y -= intersection.height;
			}


			sfSprite_setPosition(mob[_i].sprite, posMob);
		}

	}


}

void StateMob(float _dt, unsigned _i)
{
	//-------MOVE MOB---------//

	

	mob[_i].data.timerTakeHit += _dt;
	mob[_i].data.timerAttack += _dt;




	if (mob[_i].data.timerKnockBack > 0)
	{
		mob[_i].data.timerKnockBack -= _dt;
		SetVelocity(_i, _dt);
		return;
	}

	if (mob[_i].act == IS_IDLE)
	{
		StateMobMachine(IDLE_MOB, _i);
	}



	if (mob[_i].data.hp > 0 && mob[_i].act != IS_DEATH)
	{
		if (sfSprite_getPosition(player->sprite).x >= mob[_i].data.position.x)
		{

			sfSprite_setScale(mob[_i].sprite, (sfVector2f) { GAME_SCALE, GAME_SCALE });
		}
		else
		{

			sfSprite_setScale(mob[_i].sprite, (sfVector2f) { -GAME_SCALE, GAME_SCALE });
		}



		if (mob[_i].currentState != TAKE_HIT && mob[_i].currentMobAnimation->isPlaying)
		{
			sfBool blockedBySpike = sfFalse;
			for (unsigned i = 0; i < GetSpikeTabSize(); i++)
			{

				if (GetDistanceMobSpike(_i, i))
				{
					blockedBySpike = sfTrue;
					break;
				}

			}

			float distPlayerMobX = GetDistancePlayerMobX(_i);
			float distPlayerMobY = GetDistancePlayerMobY(_i);
			float distPlayerMob = GetDistancePlayerMobVector(_i);

				if (distPlayerMobX < mob[_i].rangeMove && distPlayerMobY < (player->shape.collisionPlayerRect.height * 2))
				{

					if (mob[_i].currentState == ATTACK_MOB && !sfFloatRect_intersects(&mob[_i].shape.hitRect, &player->shape.collisionAttackRect, NULL))
					{
						SetVelocity(_i, _dt);
						return;
					}

					if (distPlayerMobX > (player->shape.collisionPlayerRect.width))
					{
						if (!blockedBySpike && mob[_i].data.timerAttack > mob[_i].data.timerAttackLimit || mob[_i].act != IS_ATTACK)
						{
							StateMobMachine(RUN_MOB, _i);
							mob[_i].act = IS_MOVING;
						}
						else
						{
							mob[_i].act = IS_IDLE;
							//StateMobMachine(IDLE_MOB, _i);
						}
					}
					else
					{
						mob[_i].act = IS_ATTACK;
					}
				}
				else if (distPlayerMobY > (player->shape.collisionPlayerRect.height * 2) && mob[_i].act != IS_ATTACK)
				{
					//StateMobMachine(IDLE_MOB, _i);
					mob[_i].act = IS_IDLE;
				}
			
			//---------TAKE IT---------//

		}
		else if ((mob[_i].currentState == TAKE_HIT || mob[_i].currentState == ATTACK_MOB) && !mob[_i].currentMobAnimation->isPlaying)
		{
			//StateMobMachine(IDLE_MOB, _i);
			mob[_i].act = IS_IDLE;
		}

	}
	else if (mob[_i].data.hp <= 0 && mob[_i].act != IS_DEATH)
	{
		mob[_i].act = IS_DEATH;
		sfVector2f mobPos = sfSprite_getPosition(mob[_i].sprite);
		if (mob[_i].mobType == MUSHROOM)
		{
			if (rand() % 3 == 1)
			{
				Additem(ITEM_HEALTH, mobPos.x, mobPos.y - 30);
			}
		}
		Additem(ITEM_KEY, mobPos.x, mobPos.y - 5);
		StateMobMachine(DEATH, _i);
		sfSound_play(mob[_i].soundMob.soundDead);
		mob[_i].data.velocity.x = 0.f;
	}

	sfBool swordAttack = player->currentState == SWORD || player->currentState == SWORD_UP || player->currentState == SWORD_DOWN;
	sfBool axeAttack = player->currentState == AXE || player->currentState == AXE_UP || player->currentState == AXE_DOWN;

	if (swordAttack || axeAttack)
	{
		sfFloatRect hitPlayer = sfRectangleShape_getGlobalBounds(player->shape.collisionAttackShape);
		sfFloatRect hitMob = sfRectangleShape_getGlobalBounds(mob[_i].shape.rect);
		float posMobX = hitMob.left + (hitMob.width / 2);
		float posAttackPlayerX = sfSprite_getPosition(player->sprite).x;

		if (sfFloatRect_intersects(&hitPlayer, &hitMob, NULL) && mob[_i].currentState != DEATH)
		{


			if (swordAttack)
			{
				mob[_i].data.timerKnockBack += 0.25f;
				mob[_i].data.hp -= (SWORD_DAMAGES + rand() % 21);
				mob[_i].data.timerTakeHit = 0;
				StateMobMachine(TAKE_HIT, _i);
				mob[_i].act = IS_TAKE_HIT;
			}
			if (axeAttack)
			{
				mob[_i].data.timerKnockBack += 0.25f;
				mob[_i].data.hp -= (AXE_DAMAGES + rand() % 21);
				mob[_i].data.timerTakeHit = 0;
				StateMobMachine(TAKE_HIT, _i);
				mob[_i].act = IS_TAKE_HIT;
			}

			sfSound_setPitch(mob[_i].soundMob.soundTakeHit, RandomFloatMob(0.80f, 1.2f));
			sfTime offset = sfSeconds(0.4f);
			sfSound_setPlayingOffset(mob[_i].soundMob.soundTakeHit, offset);
			sfSound_play(mob[_i].soundMob.soundTakeHit);


			if (posMobX > posAttackPlayerX)
			{
				mob[_i].side = LEFT_MOB;
			}
			else if (posMobX < posAttackPlayerX)
			{
				mob[_i].side = WIDTH_MOB;
			}


			if (mob[_i].act == IS_TAKE_HIT || mob[_i].act == IS_ATTACK && !mob[_i].currentMobAnimation->isPlaying)
			{
				//StateMobMachine(IDLE_MOB, _i);
				mob[_i].act = IS_IDLE;
			}

		}
	}

	if (mob[_i].side != NOTHING_MOB && mob[_i].currentState != DEATH)
	{
		mob[_i].isGrounded = sfFalse;
		mob[_i].data.velocity.y = -MOB_ATTACK_PLAYER_VELOCITY_Y;

		if (mob[_i].side == LEFT_MOB)
		{
			mob[_i].data.velocity.x = MOB_ATTACK_PLAYER_VELOCITY_X;
		}
		else if (mob[_i].side == WIDTH_MOB)
		{
			mob[_i].data.velocity.x = -MOB_ATTACK_PLAYER_VELOCITY_X;
		}

		if (mob[_i].data.timerKnockBack <= 0)
		{
			mob[_i].side = NOTHING_MOB;
		}

	}


	SetVelocity(_i, _dt);
}


float GetDistancePlayerMobX(unsigned _i)
{

	if (mobCount > 0)
	{

		float distX = player->data.position.x - sfSprite_getPosition(mob[_i].sprite).x;

		return (float)fabs(distX);
		
		
	}
	return 0;
}

float GetDistancePlayerMobY(unsigned _i)
{

	if (mobCount > 0)
	{

		float distY = player->data.position.y - sfSprite_getPosition(mob[_i].sprite).y;

		return (float)fabs(distY);
		
	
	}
	return 0;
}

float GetDistancePlayerMobVector(unsigned _i)
{
	return sqrtf(powf(GetDistancePlayerMobX(_i), 2.f) + powf(GetDistancePlayerMobY(_i), 2.f));
}

void DrawMob(sfRenderWindow* _renderWindow)
{
	sfBool drawHitbox;
	for (unsigned i = 0; i < mobCount; i++)
	{

		drawHitbox = sfFalse;

		if (sfKeyboard_isKeyPressed(sfKeyC))
		{
			drawHitbox = sfTrue;
		}

		if (drawHitbox)
		{
			sfRenderWindow_drawRectangleShape(_renderWindow, mob[i].shape.rect, NULL);
			//sfRenderWindow_drawRectangleShape(_renderWindow, mob[i].shape.attackRect, NULL);
			//sfRenderWindow_drawRectangleShape(_renderWindow, mob[i].shape.collisionRect, NULL);
			//sfRenderWindow_drawRectangleShape(_renderWindow, mob[i].shape.floorSecurity, NULL);
		}
		//sfRenderWindow_drawRectangleShape(_renderWindow, mob[i].attackRect, NULL);
		//sfRenderWindow_drawRectangleShape(_renderWindow, mob[i].rect, NULL);
		//sfRenderWindow_drawRectangleShape(_renderWindow, mob[i].floorSecurity, NULL);
		//sfRenderWindow_drawRectangleShape(_renderWindow, mob[i].collisionRect, NULL);

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
		sfRectangleShape_destroy(mob[i].shape.rect);
		mob[i] = (Mob){ 0 };
	}
	mobCount = 0;
	free(mob);
	mob = (Mob*){ NULL };
}

void SetVelocity(unsigned _i, float _dt)
{
	if (mob[_i].data.timerKnockBack <= 0)
	{
		if (mob[_i].act == IS_MOVING)
		{
			if (sfSprite_getPosition(player->sprite).x > mob[_i].data.position.x)
			{
				mob[_i].data.velocity.x = mob[_i].data.speed;
			}
			else
			{
				mob[_i].data.velocity.x = -mob[_i].data.speed;
			}
		}

		if (mob[_i].act == IS_IDLE || mob[_i].act == IS_ATTACK || mob[_i].act == IS_TAKE_HIT)
		{
			mob[_i].data.velocity.x = 0;
		}

		if (!mob[_i].isGrounded)
		{
			mob[_i].data.velocity.y += GRAVITY * _dt;
		}
		else
		{
			mob[_i].data.velocity.y = 0;
		}
	}
	sfSprite_move(mob[_i].sprite, (sfVector2f) { mob[_i].data.velocity.x* _dt, mob[_i].data.velocity.y* _dt });

}

unsigned GetMobCount(void)
{
	return mobCount;
}

sfBool GetDistanceMobSpike(unsigned _mob, unsigned _spike)
{
	float posPlayer = sfSprite_getPosition(player->sprite).x;
	sfFloatRect hitMob = sfSprite_getGlobalBounds(mob[_mob].sprite);
	float posMobX = hitMob.left + (hitMob.width / 2), posMobY = hitMob.top + (hitMob.height / 2);
	sfFloatRect hitSpike = GetSpikeTab(_spike);
	float spikeOriginX = hitSpike.left + (hitSpike.width / 2), spikeOriginY = hitSpike.top + (hitSpike.height / 2);

	float minX = fminf(posPlayer, posMobX), maxX = fmaxf(posPlayer, posMobX), minY = (float)fabs(posMobY - spikeOriginY);

	if (spikeOriginX > minX && spikeOriginX < maxX && minY < 100.f)
	{
		return sfTrue;
	}
	return sfFalse;

}