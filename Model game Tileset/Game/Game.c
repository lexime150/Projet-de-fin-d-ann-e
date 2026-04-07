#include "Game.h"
void CheckSaveAndLoadLevel(int slot);
void changeLevel(const char* level);
Player player;


void LoadGame(void)
{

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
	case sfKeyNum1:
		changeLevel("Level_00");
		break;
	case sfKeyNum2:
		changeLevel("Level_01");
		break;
	case sfKeyNum3:
		changeLevel("Level_02");
		break;
	case sfKeyNum4:
		changeLevel("Level_03");
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
	CleanupMob();
	CleanupMap();
	CleanUpPlayer();
	CleanUpCamera();
	SavePlayer(playerSaveData.save);
}

void CheckSaveAndLoadLevel(int slot)
{

	PlayerSaveData* save = NULL;

	if (SaveExists(slot))
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

void changeLevel(const char* level)
{

	snprintf(player.data.level, sizeof(player.data.level), "%s", level);


	CleanupMob();
	CleanupMap();


	LoadMap(player.data.level);
	LoadCamera();
	LoadMob();
	LoadHUD();

	player.data.position = GetPlayerSpawn();

}