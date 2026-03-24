#include "Game.h"


Game game;
Player player = { 0 };

void LoadAnimation(Animation* _animation);






void LoadGame(void)
{
	//LoadMap("Map");

	//sfFloatRect collision = GetMapCollision(0);
	//printf("%.2f %.2f %.2f %.2f\n", collision.left, collision.top, collision.width, collision.height);

	//Trigger trigger = GetMapTrigger(0);
	//printf("%s %.2f %.2f %.2f %.2f\n", trigger.name, trigger.left, trigger.top, trigger.width, trigger.height);


	player.texture = sfTexture_createFromFile("Assets/Sprites/FREE_Samurai 2D Pixel Art v1.2/Sprites/IDLE.png", NULL);
	player.sprite = sfSprite_create();
	sfSprite_setTexture(player.sprite, player.texture, sfTrue);

	sfIntRect firstFrame = { 0, 0, 96, 96};
	sfSprite_setTextureRect(player.sprite, firstFrame);
	//sfSprite_setOrigin(player.sprite, (sfVector2f){48, 92});
	sfSprite_setScale(player.sprite, (sfVector2f){4.f, 4.f});
	sfSprite_setPosition(player.sprite, (sfVector2f){SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2});
	player.animation = CreateAnimation(player.sprite, 10, 10, sfTrue, sfTrue, firstFrame);
	player.currentAnimation = &player.animation;

	LoadAnimation(player.currentAnimation);

}

void PollEventGame(sfRenderWindow* _renderWindow)
{
	sfEvent event;

	while (sfRenderWindow_pollEvent(_renderWindow, &event))
	{
		switch (event.type)
		{
		case sfEvtClosed:
			sfRenderWindow_close(_renderWindow);
			break;
		case sfEvtKeyPressed:
			KeyPressedGame(_renderWindow, event.key);
			break;
		default:
			break;
		}
	}
}

void KeyPressedGame(sfRenderWindow* _renderWindow, sfKeyEvent _keyEvent)
{
	switch (_keyEvent.code)
	{
	case sfKeyEscape:
		SetGameState(MENU);
		break;
	case sfKeySpace:
		break;
	default:
		break;
	}
}

void UpdateGame(float _dt)
{
	UpdateAnimation(player.currentAnimation, _dt);

}

void DrawGame(sfRenderWindow* _renderWindow)
{
	//DrawMap(_renderWindow);
	sfRenderWindow_drawSprite(_renderWindow, player.sprite, NULL);
}

void CleanupGame(void)
{
	//CleanupMap();

}



void LoadAnimation(Animation* _animation)
{
	_animation->timer = 0.f;
	_animation->isPlaying = sfTrue;
	_animation->currentFrame = 0;

}