#include "Game.h"


Game game;
Player player = { 0 };

void LoadAnimation(Animation* _animation);






void LoadGame(void)
{

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

}

void CleanupGame(void)
{


}



