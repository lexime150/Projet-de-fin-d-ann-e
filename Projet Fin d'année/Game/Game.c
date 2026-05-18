#include "Game.h"
void CheckSaveAndLoadLevel(int _slot);
void ChangeLevel(const char* _level);
void LevelTransition();
void UpdateTransition(float _dt);
void LoadMusic();
float transitionAlpha = 255;

sfMusic* gameMusic;

sfRectangleShape* transitionShape;
Player* player;
AngelStatue angelStatue;

void LoadGame(void)
{

	int slot = playerSaveData.save;
	CheckSaveAndLoadLevel(slot);
	LevelTransition();
	LoadMusic();
	LoadKey();
	LoadBoss();
	LoadAngelStatue();
	LoadTalkbox();
	LoadFlyMob();

}

void UpdateTransition(float _dt)
{
	if (!player->action.isTransitioning)
	{
		return;
	}

	transitionAlpha -= 300.f * _dt;

	if (transitionAlpha <= 0)
	{
		transitionAlpha = 0;
		player->action.isTransitioning = sfFalse;
		transitionAlpha = 255;
	}

	sfRectangleShape_setFillColor(transitionShape, sfColor_fromRGBA(0, 0, 0, (sfUint8)transitionAlpha));
}
void LoadMusic()
{
	gameMusic = sfMusic_createFromFile("Assets/Audio/Musics/Game music.ogg");
	sfMusic_play(gameMusic);
	sfMusic_setLoop(gameMusic, sfTrue);
}
void GivePlayerKeys()
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
	case sfKeyNum5:
		ChangeLevel("Level_04");
		break;
	case sfKeyNum6:
		ChangeLevel("Level_05");
		break;
	case sfKeyNum7:
		ChangeLevel("Level_06");
		break;
	default:
		break;
	}
}

void UpdateGame(sfRenderWindow* _renderWindow, float _dt)
{
	float fps = 1.0f / _dt;

	//printf("FPS: %.2f\n", fps);
	if (sfRenderWindow_hasFocus(_renderWindow))
	{
		UpdatePlayer(_renderWindow, _dt);
		UpdateCamera(_dt);
		UpdateMob(NULL, _dt);
		Updateitem(_dt);
		UpdateMap(_dt);
		UpdateHUD();
		UpdateTransition(_dt);
		UpdateKey(_dt);
		UpdateBoss(_dt);
		UpdateAngelStatue(_dt);
		UpdateTalkbox(_dt);
		UpdateFlyMob(_dt);

		for (int i = 0; i < GetItemCount(); i++)
		{
			GetItemDistance(i);
		}
	}


}

void DrawGame(sfRenderWindow* _renderWindow)
{
	DrawCamera(_renderWindow);
	DrawMap(_renderWindow);
	DrawMob(_renderWindow);
	DrawKey(_renderWindow);
	Drawitem(_renderWindow);
	DrawAngelStatue(_renderWindow);
	DrawPlayer(_renderWindow);
	DrawBoss(_renderWindow);
	DrawFlyMob(_renderWindow);
	sfRenderWindow_setView(_renderWindow, sfRenderWindow_getDefaultView(_renderWindow));
	DrawHUD(_renderWindow);
	DrawTalkbox(_renderWindow);


	if (player->action.isTransitioning)
	{
		sfRenderWindow_drawRectangleShape(_renderWindow, transitionShape, NULL);
	}
}

void CleanupGame(void)
{
	CleanupMob();
	CleanupFlyMob();
	CleanupMap();
	CleanUpPlayer();
	CleanUpCamera();
	Cleanupitem();
	CleanupKey();
	CleanupBoss();
	CleanupAngelStatue();
	CleanupTalkbox();
	sfMusic_destroy(gameMusic);
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
		Loaditem();
		LoadKey();
		LoadCamera();
		LoadMob();
		LoadHUD();
		LoadBoss();
		LoadAngelStatue();
		LoadFlyMob();
	}
	else
	{
		LoadMap("Level_00");
		LoadPlayer(save);
		Loaditem();

		LoadCamera();
		LoadMob();
		LoadKey();
		LoadBoss();
		LoadHUD();
		LoadAngelStatue();
		LoadFlyMob();
	}
}

void ChangeLevel(const char* _level)
{

	snprintf(player->data.level, sizeof(player->data.level), "%s", _level);


	CleanupMob();
	CleanupFlyMob();
	CleanupMap();
	CleanupKey();
	CleanupHUD();
	CleanUpCamera();
	Cleanupitem();
	CleanupBoss();
	CleanupAngelStatue();

	LoadMap(player->data.level);
	LoadCamera();
	LoadMob();
	LoadKey();
	LoadHUD();
	Loaditem();
	LoadBoss();
	LoadAngelStatue();
	LoadFlyMob();

	player->data.keyNumber = 0;
	player->data.position = GetPlayerSpawn();
	player->data.velocity.x = 0;
	player->data.velocity.y = 0;
	player->data.health = player->data.maxHealth;

	player->action.isGrounded = sfFalse;
	player->action.isSliding = sfFalse;
	player->action.isWallJumping = sfFalse;
	player->action.isSlideJumping = sfFalse;
	SnapCamera();
}

void LevelTransition()
{
	transitionShape = sfRectangleShape_create();
	sfRectangleShape_setSize(transitionShape, (sfVector2f) { 1920, 1080 });
	sfRectangleShape_setScale(transitionShape, (sfVector2f) { GAME_SCALE, GAME_SCALE });
	sfRectangleShape_setPosition(transitionShape, (sfVector2f) { 0, 0 });
	sfRectangleShape_setFillColor(transitionShape, sfBlack);
}
