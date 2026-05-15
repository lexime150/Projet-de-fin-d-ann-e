#include "Flying mob.h"
#include "Map.h"
#include "Player.h"
//
//
//
//
//FlyingMob* flyMob = { 0 };
//Player* player;
//unsigned flyMobCount = 0;
//
//void SetFlyingMobAnimation(FlyingMobState _state, _I);
//void StateFlyingMobAnimation(FlyingMobState _state, _I);
//
//void StateFlyMob(float _dt, _I);
//
//
//void LoadFlyMobAnimation(_I);
//
//void AddFlyMob(sfVector2f _pos);
//
//void LoadFlyMob(void)
//{
//
//	flyMob = malloc(sizeof(FlyingMob));
//	if (flyMob == NULL)
//	{
//		fprintf(stderr, "malloc failure\n");
//	}
//
//	for (unsigned i = 0; i < GetFlyMobSpawnTabSize(); i++)
//	{
//		AddFlyMob(GetFlyMobSpawn(i));
//	}
//
//	//LoadFlyMobAnimation();
//}
//
//void AddFlyMob(sfVector2f _pos)
//{
//	FlyingMob* temp = realloc(flyMob, (flyMobCount + 1) * sizeof(FlyingMob));
//	if (temp == NULL)
//	{
//		fprintf(stderr, "realloc failure\n");
//		return;
//	}
//	flyMob = temp;
//	temp = NULL;
//
//
//	FlyingMob newMob = { 0 };
//
//	newMob.sprite = CreateSprite("Assets/Sprites/Game/Mob/Flying mob.png", _pos);
//	sfSprite_setTextureRect(newMob.sprite, (sfIntRect) { 0, 0, FLY_MOB_SIZE, FLY_MOB_SIZE });
//	sfSprite_setOrigin(newMob.sprite, (sfVector2f) { FLY_MOB_SIZE / 2.f, FLY_MOB_SIZE / 2.f });
//	
//	newMob.projectile.timerShoot = 0;
//
//	newMob.projectile.sprite = CreateSprite("Assets/Sprites/Game/Mob/Flying mob.png", _pos);
//
//	flyMob[flyMobCount] = newMob;
//
//
//	LoadFlyMobAnimation(flyMobCount);
//	flyMobCount++;
//
//}
//
//void LoadFlyMobAnimation(_I)
//{
//	sfIntRect firstFrame = { 0, 0, FLY_MOB_SIZE, FLY_MOB_SIZE };
//	flyMob[_i].animation[FLIGHT] = CreateAnimation(flyMob[_i].sprite, 8, 11, sfTrue, sfTrue, firstFrame);
//
//	firstFrame.top += FLY_MOB_SIZE;
//	flyMob[_i].animation[HURT_FLY_MOB] = CreateAnimation(flyMob[_i].sprite, 4, 7, sfTrue, sfFalse, firstFrame);
//
//	firstFrame.top += FLY_MOB_SIZE;
//	flyMob[_i].animation[DEATH_FLY_MOB] = CreateAnimation(flyMob[_i].sprite, 4, 7, sfTrue, sfFalse, firstFrame);
//
//	firstFrame.top += FLY_MOB_SIZE;
//	flyMob[_i].animation[ATTACK_1_FLY_MOB] = CreateAnimation(flyMob[_i].sprite, 8, 11, sfTrue, sfFalse, firstFrame);
//
//	firstFrame.top += FLY_MOB_SIZE;
//	flyMob[_i].animation[ATTACK_2_FLY_MOB] = CreateAnimation(flyMob[_i].sprite, 8, 13, sfTrue, sfFalse, firstFrame);
//
//	firstFrame.top += FLY_MOB_SIZE;
//	flyMob[_i].animation[ATTACK_PROJECTILES] = CreateAnimation(flyMob[_i].sprite, 6, 9, sfTrue, sfFalse, firstFrame);
//
//	firstFrame.top += FLY_MOB_SIZE; firstFrame.width = 48; firstFrame.height = 48;
//	flyMob[_i].projectile.animation = CreateAnimation(flyMob[_i].projectile.sprite, 8, 10, sfTrue, sfFalse, firstFrame);
//
//
//	SetFlyingMobAnimation(FLIGHT, _i);
//
//}
//
//void UpdateFlyMob(float _dt)
//{
//	for (unsigned i = 0; i < flyMobCount; i++)
//	{
//		//StateFlyMob(_dt, i);
//		//UpdateAnimation(flyMob[i].currentAnimation, _dt);
//	}
//
//}
//
//void DrawFlyMob(sfRenderWindow* _renderWindow)
//{
//	for (unsigned i = 0; i < flyMobCount; i++)
//	{
//		if (&flyMob[i] != NULL)
//		{
//			sfRenderWindow_drawSprite(_renderWindow, flyMob[i].sprite, NULL);
//		}
//	}
//}
//
//
//void CleanupFlyMob(void)
//{
//	for (unsigned i = 0; i < flyMobCount; i++)
//	{
//		sfSprite_destroy(flyMob[i].sprite);
//	}
//
//	free(flyMob);
//	flyMob = (FlyingMob*){ NULL };
//	flyMobCount = 0;
//
//
//}
//
//void SetFlyingMobAnimation(FlyingMobState _state, _I)
//{
//	flyMob[_i].lastState = flyMob[_i].currentState;
//	flyMob[_i].currentState = _state;
//	flyMob[_i].currentAnimation = &flyMob[_i].animation[_state];
//	flyMob[_i].currentAnimation->isPlaying = sfTrue;
//	flyMob[_i].currentAnimation->currentFrame = 0;
//	flyMob[_i].currentAnimation->timer = 0;
//
//}
//
//void StateFlyingMobAnimation(FlyingMobState _state, _I)
//{
//	if (flyMob[_i].currentState != _state)
//	{
//		SetFlyingMobAnimation(_state, _i);
//	}
//}
//
//void StateFlyMob(float _dt, _I)
//{
//	flyMob[_i].projectile.timerShoot += _dt;
//	flyMob[_i].timer.timerMoveY += _dt;
//
//	if (flyMob[_i].timer.knockBackTimer > 0)
//	{
//		flyMob[_i].timer.knockBackTimer -= _dt;
//		return;
//
//	}
//
//
//	if (flyMob[_i].currentState != DEATH_FLY_MOB)
//	{
//		sfVector2f posPlayer = player->data.position;
//		sfVector2f posFlyMob = sfSprite_getPosition(flyMob[_i].sprite);
//		sfVector2f dist = GetDistanceObjectVector(posPlayer, posFlyMob);
//
//		if (dist.x < 700.f && dist.x > 250.f)
//		{
//
//			if (flyMob[_i].projectile.timerShoot < 3.5f)
//			{
//				if (posPlayer.x < posFlyMob.x)
//				{
//					sfSprite_setScale(flyMob[_i].sprite, (sfVector2f) { -GAME_SCALE, GAME_SCALE });
//					flyMob[_i].velocity.x = -200.f;
//				}
//				else
//				{
//					sfSprite_setScale(flyMob[_i].sprite, (sfVector2f) { GAME_SCALE, GAME_SCALE });
//					flyMob[_i].velocity.x = 200.f;
//				}
//
//			}
//			else
//			{
//				StateFlyingMobAnimation(ATTACK_PROJECTILES, _i);
//				flyMob[_i].velocity.x = 0;
//
//				if (!flyMob[_i].currentAnimation->isPlaying)
//				{
//					flyMob[_i].projectile.timerShoot = 0;
//					StateFlyingMobAnimation(FLIGHT, _i);
//				}
//
//			}
//
//		}
//		else
//		{
//			flyMob[_i].velocity.x = 0;
//		}
//
//		sfSprite_setPosition(flyMob[_i].sprite, (sfVector2f) { posFlyMob.x, posFlyMob.y + sinf(flyMob[_i].timer.timerMoveY * 4.f) });
//		sfSprite_move(flyMob[_i].sprite, (sfVector2f) { flyMob[_i].velocity.x* _dt, flyMob[_i].velocity.y* _dt });
//	}
//}





#include "Flying mob.h"
#include "Map.h"
#include "Player.h"

FlyingMob* flyMob = NULL;
Player* player = NULL;
unsigned flyMobCount = 0;

// --- Prototypes ---
void SetFlyingMobAnimation(FlyingMobState _state, _I);
void StateFlyingMobAnimation(FlyingMobState _state, _I);
void StateFlyMob(float _dt, _I);
void LoadFlyMobAnimation(_I);
void AddFlyMob(sfVector2f _pos);

// -------------------------------------------------------

void LoadFlyMob(void)
{
    // Pas de malloc ici : flyMob démarre à NULL,
    // realloc(NULL, size) se comporte comme malloc
    for (unsigned i = 0; i < GetFlyMobSpawnTabSize(); i++)
    {
        AddFlyMob(GetFlyMobSpawn(i));
    }
}

void AddFlyMob(sfVector2f _pos)
{
    FlyingMob* temp = realloc(flyMob, (flyMobCount + 1) * sizeof(FlyingMob));
    if (temp == NULL)
    {
        fprintf(stderr, "realloc failure\n");
        return;
    }
    flyMob = temp;

    // Zero-init du nouvel élément pour éviter des valeurs indéfinies
    memset(&flyMob[flyMobCount], 0, sizeof(FlyingMob));

    FlyingMob* mob = &flyMob[flyMobCount]; // alias lisible

    mob->sprite = CreateSprite("Assets/Sprites/Game/Mob/Flying mob.png", _pos);
    if (mob->sprite == NULL)
    {
        fprintf(stderr, "CreateSprite failure for FlyingMob\n");
        return;
    }
    sfSprite_setTextureRect(mob->sprite, (sfIntRect) { 0, 0, FLY_MOB_SIZE, FLY_MOB_SIZE });
    sfSprite_setOrigin(mob->sprite, (sfVector2f) { FLY_MOB_SIZE / 2.f, FLY_MOB_SIZE / 2.f });

    mob->projectile.timerShoot = 0.f;
    mob->projectile.sprite = CreateSprite("Assets/Sprites/Game/Mob/Flying mob.png", _pos);
    if (mob->projectile.sprite == NULL)
    {
        fprintf(stderr, "CreateSprite failure for projectile\n");
        return;
    }

    LoadFlyMobAnimation(flyMobCount);
    flyMobCount++;
}

void LoadFlyMobAnimation(_I)
{
    if (_i >= flyMobCount + 1) return; // sécurité

    sfIntRect firstFrame = { 0, 0, FLY_MOB_SIZE, FLY_MOB_SIZE };

    flyMob[_i].animation[FLIGHT] =
        CreateAnimation(flyMob[_i].sprite, 8, 11, sfTrue, sfTrue, firstFrame);

    firstFrame.top += FLY_MOB_SIZE;
    flyMob[_i].animation[HURT_FLY_MOB] =
        CreateAnimation(flyMob[_i].sprite, 4, 7, sfTrue, sfFalse, firstFrame);

    firstFrame.top += FLY_MOB_SIZE;
    flyMob[_i].animation[DEATH_FLY_MOB] =
        CreateAnimation(flyMob[_i].sprite, 4, 7, sfTrue, sfFalse, firstFrame);

    firstFrame.top += FLY_MOB_SIZE;
    flyMob[_i].animation[ATTACK_1_FLY_MOB] =
        CreateAnimation(flyMob[_i].sprite, 8, 11, sfTrue, sfFalse, firstFrame);

    firstFrame.top += FLY_MOB_SIZE;
    flyMob[_i].animation[ATTACK_2_FLY_MOB] =
        CreateAnimation(flyMob[_i].sprite, 8, 13, sfTrue, sfFalse, firstFrame);

    firstFrame.top += FLY_MOB_SIZE;
    flyMob[_i].animation[ATTACK_PROJECTILES] =
        CreateAnimation(flyMob[_i].sprite, 6, 9, sfTrue, sfFalse, firstFrame);

    // Projectile : taille différente (48x48)
    firstFrame.top += FLY_MOB_SIZE;
    firstFrame.width = 48;
    firstFrame.height = 48;
    flyMob[_i].projectile.animation =
        CreateAnimation(flyMob[_i].projectile.sprite, 8, 10, sfTrue, sfFalse, firstFrame);

    SetFlyingMobAnimation(FLIGHT, _i);
}

// -------------------------------------------------------

void UpdateFlyMob(float _dt)
{
    if (flyMob == NULL || player == NULL) return; // garde-fou

    for (unsigned i = 0; i < flyMobCount; i++)
    {
        StateFlyMob(_dt, i);
        UpdateAnimation(flyMob[i].currentAnimation, _dt);
    }
}

void DrawFlyMob(sfRenderWindow* _renderWindow)
{
    if (flyMob == NULL) return;

    for (unsigned i = 0; i < flyMobCount; i++)
    {
        sfRenderWindow_drawSprite(_renderWindow, flyMob[i].sprite, NULL);
    }
}

void CleanupFlyMob(void)
{
    if (flyMob == NULL) return;

    for (unsigned i = 0; i < flyMobCount; i++)
    {
        if (flyMob[i].sprite)          sfSprite_destroy(flyMob[i].sprite);
        if (flyMob[i].projectile.sprite) sfSprite_destroy(flyMob[i].projectile.sprite);
    }

    free(flyMob);
    flyMob = NULL;      // ? syntaxe correcte
    flyMobCount = 0;
}

// -------------------------------------------------------

void SetFlyingMobAnimation(FlyingMobState _state, _I)
{
    flyMob[_i].lastState = flyMob[_i].currentState;
    flyMob[_i].currentState = _state;
    flyMob[_i].currentAnimation = &flyMob[_i].animation[_state];
    flyMob[_i].currentAnimation->isPlaying = sfTrue;
    flyMob[_i].currentAnimation->currentFrame = 0;
    flyMob[_i].currentAnimation->timer = 0;
}

void StateFlyingMobAnimation(FlyingMobState _state, _I)
{
    if (flyMob[_i].currentState != _state)
    {
        SetFlyingMobAnimation(_state, _i);
    }
}

void StateFlyMob(float _dt, _I)
{
    flyMob[_i].projectile.timerShoot += _dt;
    flyMob[_i].timer.timerMoveY += _dt;

    if (flyMob[_i].timer.knockBackTimer > 0.f)
    {
        flyMob[_i].timer.knockBackTimer -= _dt;
        return;
    }

    if (flyMob[_i].currentState == DEATH_FLY_MOB) return;

    sfVector2f posPlayer = player->data.position;
    sfVector2f posFlyMob = sfSprite_getPosition(flyMob[_i].sprite);
    sfVector2f dist = GetDistanceObjectVector(posPlayer, posFlyMob);

    if (dist.x < 700.f && dist.x > 250.f)
    {
        if (flyMob[_i].projectile.timerShoot < 3.5f)
        {
            if (posPlayer.x < posFlyMob.x)
            {
                sfSprite_setScale(flyMob[_i].sprite, (sfVector2f) { -GAME_SCALE, GAME_SCALE });
                flyMob[_i].velocity.x = -200.f;
            }
            else
            {
                sfSprite_setScale(flyMob[_i].sprite, (sfVector2f) { GAME_SCALE, GAME_SCALE });
                flyMob[_i].velocity.x = 200.f;
            }
            StateFlyingMobAnimation(FLIGHT, _i);
        }
        else
        {
            StateFlyingMobAnimation(ATTACK_PROJECTILES, _i);
            flyMob[_i].velocity.x = 0.f;

            if (!flyMob[_i].currentAnimation->isPlaying)
            {
                flyMob[_i].projectile.timerShoot = 0.f;
                StateFlyingMobAnimation(FLIGHT, _i);
            }
        }
    }
    else
    {
        flyMob[_i].velocity.x = 0.f;
        StateFlyingMobAnimation(FLIGHT, _i);
    }

    // Mouvement sinusoïdal vertical + déplacement horizontal
    sfSprite_setPosition(flyMob[_i].sprite,
        (sfVector2f) {
        posFlyMob.x, posFlyMob.y + sinf(flyMob[_i].timer.timerMoveY * 4.f)
    });
    sfSprite_move(flyMob[_i].sprite,
        (sfVector2f) {
        flyMob[_i].velocity.x* _dt, flyMob[_i].velocity.y* _dt
    });
}