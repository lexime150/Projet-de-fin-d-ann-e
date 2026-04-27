#include "GameOver.h"

void LoadGameOver(void)
{
}

void PollEventGameOver(sfRenderWindow* _renderWindow)
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
			KeyPressedGameOver(_renderWindow, event.key);
			break;
		default:
			break;
		}
	}
}

void KeyPressedGameOver(sfRenderWindow* _renderWindow, sfKeyEvent _keyEvent)
{
	switch (_keyEvent.code)
	{
	case sfKeyEscape:
		SetGameState(MENU);
		break;
	case sfKeySpace:
		SetGameState(GAME);
		break;
	default:
		break;
	}
}

void UpdateGameOver(float _dt)
{
}

void DrawGameOver(sfRenderWindow* _renderWindow)
{
}

void CleanupGameOver(void)
{
}
