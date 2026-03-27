#include "Game.h"


Game game;
Player player;

void LoadGame(void)
{
	LoadMap("Map");
	LoadPlayer();
	LoadCamera();
	LoadMob();
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
	UpdatePlayer(_dt);
	UpdateCamera(_dt);
	UpdateMob(NULL, _dt);
}

void DrawGame(sfRenderWindow* _renderWindow)
{

	DrawCamera(_renderWindow);
	DrawMap(_renderWindow);
	DrawPlayer(_renderWindow);
	DrawMob(_renderWindow);
}

void CleanupGame(void)
{
	CleanupMap();
	CleanUpPlayer();
	CleanUpCamera();
}