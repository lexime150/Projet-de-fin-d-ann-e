#include "Game.h"


Game game;
Player player;

void CollisionPlayerPlatforms();

void LoadGame(void)
{
	LoadMap("Map");
	LoadPlayer();
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
	CollisionPlayerPlatforms();

}

void DrawGame(sfRenderWindow* _renderWindow)
{
	DrawMap(_renderWindow);
	DrawPlayer(_renderWindow);
}

void CleanupGame(void)
{
	CleanupMap();
	CleanUpPlayer();

}

void CollisionPlayerPlatforms()
{
	for (int i = 0; i < GetCollisionTabSize(); i++)
	{
		sfFloatRect platformRect = GetMapCollision(i);

		if (sfFloatRect_intersects(&player.playerRect, &platformRect, NULL))
		{
			float overlapLeft = (player.playerRect.left + player.playerRect.width) - platformRect.left;
			float overlapRight = (platformRect.left + platformRect.width) - player.playerRect.left;
			float overlapTop = (player.playerRect.top + player.playerRect.height) - platformRect.top;
			float overlapBottom = (platformRect.top + platformRect.height) - player.playerRect.top;

			float minOverlapX = overlapLeft < overlapRight ? overlapLeft : overlapRight;
			float minOverlapY = overlapTop < overlapBottom ? overlapTop : overlapBottom;

			if (minOverlapX < minOverlapY)
			{
				if (overlapLeft < overlapRight)
					player.playerRect.left -= overlapLeft;
				else
					player.playerRect.left += overlapRight;

				player.velocity.x = 0;
			}
			else
			{
				if (overlapTop < overlapBottom)
					player.playerRect.top -= overlapTop;
				else
					player.playerRect.top += overlapBottom;

				player.velocity.y = 0;
			}
		}
	}
	sfSprite_setPosition(player.sprite, (sfVector2f) {player.playerRect.left,player.playerRect.top});
}