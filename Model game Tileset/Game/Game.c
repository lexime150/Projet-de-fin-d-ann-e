#include "Game.h"


Game game;

void LoadGame(void)
{
	//LoadMap("Map");

	//sfFloatRect collision = GetMapCollision(0);
	//printf("%.2f %.2f %.2f %.2f\n", collision.left, collision.top, collision.width, collision.height);

	//Trigger trigger = GetMapTrigger(0);
	//printf("%s %.2f %.2f %.2f %.2f\n", trigger.name, trigger.left, trigger.top, trigger.width, trigger.height);

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


}

void DrawGame(sfRenderWindow* _renderWindow)
{
	//DrawMap(_renderWindow);
}

void CleanupGame(void)
{
	//CleanupMap();

}
