#include "Flying mob.h"
#include "Map.h"
#include "Player.h"


FlyingMob flyMob = { 0 };
Player* player;


void SetFlyingMobAnimation(FlyingMobState _state);
void StateFlyingMobAnimation(FlyingMobState _state);


void LoadFlyMobAnimation(void);

void LoadFlyMob(void)
{
flyMob.sprite = CreateSprite("Assets/Sprites/Game/Mob/Flying mob.png", 
    (sfVector2f){ GetPlayerSpawn().x + 300.f, GetPlayerSpawn().y - 200.f });
	sfSprite_setTextureRect(flyMob.sprite, (sfIntRect) { 0, 0, FLY_MOB_SIZE, FLY_MOB_SIZE });
	sfSprite_setOrigin(flyMob.sprite, (sfVector2f) { FLY_MOB_SIZE / 2, FLY_MOB_SIZE / 2 });
	sfSprite_setScale(flyMob.sprite, (sfVector2f){GAME_SCALE, GAME_SCALE});
	flyMob.projectile.sprite = CreateSprite("Assets/Sprites/Game/Mob/Flying mob.png", (sfVector2f) { GetPlayerSpawn().x + 25.f, GetPlayerSpawn().y - 200.f });
	printf("%f %f\n", sfSprite_getPosition(player->sprite).x, sfSprite_getPosition(player->sprite).y);

	LoadFlyMobAnimation();
}

void LoadFlyMobAnimation(void)
{
	sfIntRect firstFrame = { 0, 0, FLY_MOB_SIZE, FLY_MOB_SIZE };
	flyMob.animation[FLIGHT] = CreateAnimation(flyMob.sprite, 8, 11, sfTrue, sfTrue, firstFrame);

	firstFrame.top += FLY_MOB_SIZE;
	flyMob.animation[HURT_FLY_MOB] = CreateAnimation(flyMob.sprite, 4, 7, sfTrue, sfFalse, firstFrame);

	firstFrame.top += FLY_MOB_SIZE;
	flyMob.animation[DEATH_FLY_MOB] = CreateAnimation(flyMob.sprite, 4, 7, sfTrue, sfFalse, firstFrame);

	firstFrame.top += FLY_MOB_SIZE;
	flyMob.animation[ATTACK_1_FLY_MOB] = CreateAnimation(flyMob.sprite, 8, 11, sfTrue, sfFalse, firstFrame);

	firstFrame.top += FLY_MOB_SIZE;
	flyMob.animation[ATTACK_2_FLY_MOB] = CreateAnimation(flyMob.sprite, 8, 13, sfTrue, sfFalse, firstFrame);

	firstFrame.top += FLY_MOB_SIZE;
	flyMob.animation[ATTACK_PROJECTILES] = CreateAnimation(flyMob.sprite, 6, 9, sfTrue, sfFalse, firstFrame);

	firstFrame.top += FLY_MOB_SIZE; firstFrame.width = 48; firstFrame.height = 48;
	flyMob.projectile.animation = CreateAnimation(flyMob.projectile.sprite, 8, 10, sfTrue, sfFalse, firstFrame);


	SetFlyingMobAnimation(ATTACK_1_FLY_MOB);

}

void UpdateFlyMob(float _dt)
{

	UpdateAnimation(flyMob.currentAnimation, _dt);

}

void DrawFlyMob(sfRenderWindow* _renderWindow)
{
	printf("%f  %f\n", sfSprite_getPosition(flyMob.sprite).x, sfSprite_getPosition(flyMob.sprite).y);
	sfRenderWindow_drawSprite(_renderWindow, flyMob.sprite, NULL);
}

void CleanupFlyMob(void)
{
}




void SetFlyingMobAnimation(FlyingMobState _state)
{
	flyMob.lastState = flyMob.currentState;
	flyMob.currentState = _state;
	flyMob.currentAnimation = &flyMob.animation[_state];
	flyMob.currentAnimation->isPlaying = sfTrue;
	flyMob.currentAnimation->currentFrame = 0;
	flyMob.currentAnimation->timer = 0;

}








void StateFlyingMobAnimation(FlyingMobState _state)
{
	if (flyMob.currentState != _state)
	{
		SetFlyingMobAnimation(_state);
	}
}
