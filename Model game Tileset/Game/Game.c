#include "Game.h"
void CheckSaveAndLoadLevel(int _slot);
void ChangeLevel(const char* _level);
void LevelTransition();
void UpdateTransition(float _dt);
float transitionAlpha = 255;


sfRectangleShape* transitionShape;
Player player;


void LoadGame(void)
{

	int slot = playerSaveData.save;
	CheckSaveAndLoadLevel(slot);
	LevelTransition();

}

void UpdateTransition(float _dt)
{
	if (!player.action.isTransitioning)
	{
		return;
	}

	transitionAlpha -= 300.f * _dt;

	if (transitionAlpha <= 0)
	{
		transitionAlpha = 0;
		player.action.isTransitioning = sfFalse;
		transitionAlpha = 255;
	}

	sfRectangleShape_setFillColor(transitionShape,sfColor_fromRGBA(0, 0, 0, (sfUint8)transitionAlpha));
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
	case sfKeyNum1:
		ChangeLevel("Level_00");
		break;
	case sfKeyNum2:
		ChangeLevel("Level_01");
		break;
	case sfKeyNum3:
		ChangeLevel("Level_02");
		break;
	case sfKeyNum4:
		ChangeLevel("Level_03");
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
	UpdateTransition(_dt);
}

void DrawGame(sfRenderWindow* _renderWindow)
{
	DrawCamera(_renderWindow);
	DrawMap(_renderWindow);
	DrawMob(_renderWindow);
	DrawPlayer(_renderWindow);
	sfRenderWindow_setView(_renderWindow, sfRenderWindow_getDefaultView(_renderWindow));
	DrawHUD(_renderWindow);
	if (player.action.isTransitioning)
	{
		sfRenderWindow_drawRectangleShape(_renderWindow, transitionShape, NULL);
	}
}

void CleanupGame(void)
{
	CleanupMob();
	CleanupMap();
	CleanUpPlayer();
	CleanUpCamera();
	SavePlayer(playerSaveData.save);
}

void CheckSaveAndLoadLevel(int _slot)
{

	PlayerSaveData* save = NULL;

	if (SaveExists(_slot))
	{
		save = LoadSave(_slot);
	}

	if (save != NULL)
	{

		LoadMap(save->level);
		LoadPlayer(save);
		SetSavedStat(save);

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

void ChangeLevel(const char* _level)
{

	snprintf(player.data.level, sizeof(player.data.level), "%s", _level);


	CleanupMob();
	CleanupMap();


	LoadMap(player.data.level);
	LoadCamera();
	LoadMob();
	LoadHUD();

	player.data.position = GetPlayerSpawn();

}

void LevelTransition()
{
	transitionShape = sfRectangleShape_create();
	sfRectangleShape_setSize(transitionShape, (sfVector2f) { 1920, 1080 });
	sfRectangleShape_setScale(transitionShape, (sfVector2f) { GAME_SCALE, GAME_SCALE });
	sfRectangleShape_setPosition(transitionShape, (sfVector2f) { 0, 0 });
	sfRectangleShape_setFillColor(transitionShape, sfBlack);
}
