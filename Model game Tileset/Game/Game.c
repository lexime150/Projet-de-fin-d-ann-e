#include "Game.h"
void CheckSaveAndLoadLevel(int slot);

Game game;
Player player;

void LoadGame(void)
{

	//sfVector2f pSpawn = GetPlayerSpawn();
	//printf("Player Spawn -> x: %.2f, y: %.2f\n", pSpawn.x, pSpawn.y);

	//unsigned int enemyCount = GetEnemySpawnTabSize();
	//printf("Enemy Spawn count: %u\n", enemyCount);

	//for (unsigned int i = 0; i < enemyCount; i++)
	//{
	//	sfVector2f eSpawn = GetEnemySpawn(i);
	//	printf("Enemy Spawn [%u] -> x: %.2f, y: %.2f\n", i, eSpawn.x, eSpawn.y);
	//}
	int slot = playerSaveData.save;
	CheckSaveAndLoadLevel(slot);

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
	SavePlayer(playerSaveData.save);
}

void CheckSaveAndLoadLevel(int slot)
{

	PlayerSaveData* save = NULL;

	if (SaveExists(slot));
	{
		save = LoadSave(slot);
	}

	if (save != NULL)
	{

		LoadMap(save->level);

		LoadPlayer(save);
		setSavedStat(save);

		LoadCamera();
		LoadMob();
		LoadHUD();
	}
	else
	{
		LoadMap("level_00");
		LoadPlayer(save);
		LoadCamera();
		LoadMob();
		LoadHUD();
	}
}
