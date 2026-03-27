#include "Mob.h"

Mob mobMushroom = { 0 };
sfTexture* textureMushroom;

void SetAnimationMushroom(MobState _state);

void LoadMob(void)
{
	 textureMushroom = sfTexture_createFromFile("Assets/Sprites/MOBil.png", NULL);

	
	 mobMushroom.sprite = sfSprite_create();
	 sfSprite_setTexture(mobMushroom.sprite, textureMushroom, sfTrue);
	 sfSprite_setScale(mobMushroom.sprite, (sfVector2f){GAME_SCALE, GAME_SCALE});
	 sfSprite_setPosition(mobMushroom.sprite, (sfVector2f){100, 100});
	 
	 LoadMobAnimation();
}

void LoadMobAnimation(void)
{
	sfIntRect firstFrame = {0, 0, MUSHROOM_SIZE, MUSHROOM_SIZE };
	mobMushroom.mobAnimation[IDLE_MOB] = CreateAnimation(mobMushroom.sprite, 4, 10, sfTrue, sfTrue, firstFrame);

	SetAnimationMushroom(IDLE_MOB);

}

void SetAnimationMushroom(MobState _state)
{
	mobMushroom.lastState = mobMushroom.currentState;
	mobMushroom.currentState = _state;
	mobMushroom.currentMobAnimation = &mobMushroom.mobAnimation[_state];
	mobMushroom.currentMobAnimation->currentFrame = 0;
	mobMushroom.currentMobAnimation->isPlaying = sfTrue;
	mobMushroom.currentMobAnimation->timer = 0.f;

}

void UpdateMob(sfRenderWindow* _renderWindow, float _dt)
{
	UpdateAnimation(mobMushroom.currentMobAnimation, _dt);
}

void DrawMob(sfRenderWindow* _renderWindow)
{
	sfRenderWindow_drawSprite(_renderWindow, mobMushroom.sprite, NULL);
}

void CleanupMob(void)
{
}
