#include "Game.h"


Game game;
Player player;

void LoadGame(void)
{
	LoadMap("Map");
	LoadPlayer();
	LoadCamera();
	LoadMob();
	LoadHUD();
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
	case sfKeyG:
		player.data.health -= rand() % 50;
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
	UpdateHUD();
}

void DrawGame(sfRenderWindow* _renderWindow)
{

	DrawCamera(_renderWindow);
	DrawMap(_renderWindow);
	DrawMob(_renderWindow);
	DrawPlayer(_renderWindow);
	sfRenderWindow_setView(_renderWindow, sfRenderWindow_getDefaultView(_renderWindow));
	DrawHUD(_renderWindow);
}

void CleanupGame(void)
{
	CleanupMap();
	CleanUpPlayer();
	CleanUpCamera();
	CleanupMob();
}