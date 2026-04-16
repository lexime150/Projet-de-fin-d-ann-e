#include "Common.h"
#include "GameState.h"

#define CUTE_TILED_IMPLEMENTATION
#include "cute_tiled.h"

typedef struct MainData
{
	sfRenderWindow* renderWindow;
	sfClock* clock;
}MainData;

void Load(MainData* _mainData);
void PollEvent(sfRenderWindow* _renderWindow);
void Update(MainData _mainData);
void Draw(sfRenderWindow* _renderWindow);
void Cleanup(MainData* _mainData);

void LoadMainData(MainData* _mainData);
void CleanupMainData(MainData* _mainData);

int main(void)
{
	MainData mainData = { 0 };
	srand(_getpid());
	Load(&mainData);

	while (sfRenderWindow_isOpen(mainData.renderWindow))
	{
		PollEvent(mainData.renderWindow);
		Update(mainData);
		Draw(mainData.renderWindow);
	}

	Cleanup(&mainData);

	return EXIT_SUCCESS;
}

void Load(MainData* _mainData)
{
	LoadMainData(_mainData);
	SetGameState(MENU);
}

void PollEvent(sfRenderWindow* _renderWindow)
{
	switch (GetGameState())
	{
	case MENU:
		PollEventMenu(_renderWindow);
		break;
	case GAME:
		PollEventGame(_renderWindow);
		break;
	case GAME_OVER:
		PollEventGameOver(_renderWindow);
		break;
	default:
		break;
	}
}

void Update(MainData _mainData)
{
	float dt = sfTime_asSeconds(sfClock_restart(_mainData.clock));

	if (dt >= 0.03f)
	{
		dt = 0.03f;
	}

	switch (GetGameState())
	{
	case MENU:
		UpdateMenu(_mainData.renderWindow,dt);
		break;
	case GAME:
		UpdateGame(_mainData.renderWindow, dt);
		break;
	case GAME_OVER:
		UpdateGameOver(dt);
		break;
	default:
		break;
	}
}

void Draw(sfRenderWindow* _renderWindow)
{
	sfRenderWindow_clear(_renderWindow, sfBlack);

	switch (GetGameState())
	{
	case MENU:
		DrawMenu(_renderWindow);
		break;
	case GAME:
		DrawGame(_renderWindow);
		break;
	case GAME_OVER:
		DrawGameOver(_renderWindow);
		break;
	default:
		break;
	}

	sfRenderWindow_display(_renderWindow);
}

void Cleanup(MainData* _mainData)
{
	CleanupMainData(_mainData);

	switch (GetGameState())
	{
	case MENU:
		CleanupMenu();
		break;
	case GAME:
		CleanupGame();
		break;
	case GAME_OVER:
		CleanupGameOver();
		break;
	default:
		break;
	}
}

void LoadMainData(MainData* _mainData)
{
	sfVideoMode videoMode = { (unsigned int) SCREEN_WIDTH, (unsigned int) SCREEN_HEIGHT, BPP };
	_mainData->renderWindow = sfRenderWindow_create(videoMode, "Game loop", sfDefaultStyle, NULL);
	sfRenderWindow_setFramerateLimit(_mainData->renderWindow, 60);
	_mainData->clock = sfClock_create();
}

void CleanupMainData(MainData* _mainData)
{
	sfRenderWindow_destroy(_mainData->renderWindow);
	_mainData->renderWindow = NULL;

	sfClock_destroy(_mainData->clock);
	_mainData->clock = NULL;
}
