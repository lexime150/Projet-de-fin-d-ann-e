#include "Boss.h"
#include "Map.h"

Boss boss;

void LoadBossAnimation();
void SetBossAnimation(BossState _state);
void StateBossMachine(BossState _state);
//void SetVelocityBoss(void);
//void StateBoss(void);



void LoadBoss(void)
{
	boss.timer = (Timer){ 0 };
	boss.sprite = sfSprite_create();
	boss.texture = sfTexture_createFromFile("Assets/Sprites/Game/Boss/Boss.png", NULL);
	sfSprite_setTexture(boss.sprite, boss.texture, sfTrue);
	sfSprite_setTextureRect(boss.sprite, (sfIntRect) { 0, 0, 128, 128 });
	sfSprite_setOrigin(boss.sprite, (sfVector2f) { 64, 128 });
	sfSprite_setScale(boss.sprite, (sfVector2f) { GAME_SCALE, GAME_SCALE });
	sfSprite_setPosition(boss.sprite, GetPlayerSpawn());

	LoadBossAnimation();
}

void LoadBossAnimation()
{

	sfIntRect firstFrame = { 0, 0, BOSS_SIZE, BOSS_SIZE };
	boss.animationBoss[IDLE_1] = CreateAnimation(boss.sprite, 7, 11, sfTrue, sfTrue, firstFrame);

	firstFrame.top += BOSS_SIZE;
	boss.animationBoss[IDLE_2] = CreateAnimation(boss.sprite, 5, 8, sfTrue, sfTrue, firstFrame);

	firstFrame.top += BOSS_SIZE;
	boss.animationBoss[WALK] = CreateAnimation(boss.sprite, 13, 11, sfTrue, sfTrue, firstFrame);

	firstFrame.top += BOSS_SIZE;
	boss.animationBoss[RUN_BOSS] = CreateAnimation(boss.sprite, 7, 12, sfTrue, sfTrue, firstFrame);

	firstFrame.top += BOSS_SIZE;
	boss.animationBoss[ATTACK_1] = CreateAnimation(boss.sprite, 16, 20, sfTrue, sfFalse, firstFrame);

	firstFrame.top += BOSS_SIZE;
	boss.animationBoss[ATTACK_2] = CreateAnimation(boss.sprite, 7, 11, sfTrue, sfFalse, firstFrame);

	firstFrame.top += BOSS_SIZE;
	boss.animationBoss[ATTACK_3] = CreateAnimation(boss.sprite, 10, 12, sfTrue, sfFalse, firstFrame);

	firstFrame.top += BOSS_SIZE;
	boss.animationBoss[SPECIAL] = CreateAnimation(boss.sprite, 5, 8, sfTrue, sfFalse, firstFrame);

	firstFrame.top += BOSS_SIZE;
	boss.animationBoss[HURT] = CreateAnimation(boss.sprite, 3, 6, sfTrue, sfFalse, firstFrame);

	firstFrame.left += (BOSS_SIZE * 3);
	boss.animationBoss[DEATH_BOSS] = CreateAnimation(boss.sprite, 3, 4, sfTrue, sfFalse, firstFrame);

	SetBossAnimation(IDLE_1);
}

void SetBossAnimation(BossState _state)
{

	boss.lastState = boss.currentState;
	boss.currentState = _state;
	boss.currentAnimation = &boss.animationBoss[_state];
	//boss.currentAnimation->firstFrame.left = 0;
	boss.currentAnimation->timer = 0;
	boss.currentAnimation->currentFrame = 0;
	boss.currentAnimation->isPlaying = sfTrue;

}

void StateBossMachine(BossState _state)
{
	if (boss.currentState != _state)
	{
		SetBossAnimation(_state);
	}

}

void UpdateBoss(float _dt)
{
	StateBossMachine(DEATH_BOSS);

	UpdateAnimation(boss.currentAnimation, _dt);

}

void DrawBoss(sfRenderWindow* _renderWindow)
{
	sfRenderWindow_drawSprite(_renderWindow, boss.sprite, NULL);
}

void CleanupBoss(void)
{
}







//void SetVelocity(void)
//{
//
//
//
//
//
//}
