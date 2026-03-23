#include "Menu.h"

void LoadMenu(void)
{
}

void PollEventMenu(sfRenderWindow* _renderWindow)
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
			KeyPressedMenu(_renderWindow, event.key);
			break;
		default:
			break;
		}
	}
}

void KeyPressedMenu(sfRenderWindow* _renderWindow, sfKeyEvent _keyEvent)
{
	switch (_keyEvent.code)
	{
	case sfKeyEscape:
		sfRenderWindow_close(_renderWindow);
		break;
	case sfKeySpace:
		SetGameState(GAME);
		break;
	default:
		break;
	}
}

void UpdateMenu(float _dt)
{
}

void DrawMenu(sfRenderWindow* _renderWindow)
{
}

void CleanupMenu(void)
{
}
