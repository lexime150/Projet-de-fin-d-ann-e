#include "Camera.h"

Camera camera;
Player* player;
void CenterCamera(float _dt);

void LoadBackgroundGame()
{
	camera.backgroundGame.texture3rdLayer =
		sfTexture_createFromFile("Assets/Sprites/Game/Background/Sun_Backscreen.png", NULL);

	camera.backgroundGame.texture2ndLayer =
		sfTexture_createFromFile("Assets/Sprites/Game/Background/City backscreen.png", NULL);

	camera.backgroundGame.texture1stLayer =
		sfTexture_createFromFile("Assets/Sprites/Game/Background/City frontscreen.png", NULL);

	if (!camera.backgroundGame.texture1stLayer ||
		!camera.backgroundGame.texture2ndLayer ||
		!camera.backgroundGame.texture3rdLayer)
	{
		printf("Erreur chargement textures background\n");
		return;
	}

	camera.backgroundGame.sprite1stLayer = sfSprite_create();
	camera.backgroundGame.sprite2ndLayer = sfSprite_create();
	camera.backgroundGame.sprite3rdLayer = sfSprite_create();

	sfSprite_setTexture(camera.backgroundGame.sprite1stLayer, camera.backgroundGame.texture1stLayer, sfTrue);
	sfSprite_setTexture(camera.backgroundGame.sprite2ndLayer, camera.backgroundGame.texture2ndLayer, sfTrue);
	sfSprite_setTexture(camera.backgroundGame.sprite3rdLayer, camera.backgroundGame.texture3rdLayer, sfTrue);

	sfTexture_setRepeated(camera.backgroundGame.texture1stLayer, sfTrue);
	sfTexture_setRepeated(camera.backgroundGame.texture2ndLayer, sfTrue);
	sfTexture_setRepeated(camera.backgroundGame.texture3rdLayer, sfTrue);

	sfSprite_setScale(camera.backgroundGame.sprite1stLayer, (sfVector2f) { 1.5 * GAME_SCALE, 1.5 * GAME_SCALE });
	sfSprite_setScale(camera.backgroundGame.sprite2ndLayer, (sfVector2f) { 1.5 * GAME_SCALE, 1.5 * GAME_SCALE });
	sfSprite_setScale(camera.backgroundGame.sprite3rdLayer, (sfVector2f) { 1.5 * GAME_SCALE, 1.5 * GAME_SCALE });

	sfVector2u s3 = sfTexture_getSize(camera.backgroundGame.texture3rdLayer);
	sfVector2u s2 = sfTexture_getSize(camera.backgroundGame.texture2ndLayer);
	sfVector2u s1 = sfTexture_getSize(camera.backgroundGame.texture1stLayer);

	camera.layerWidth3 = s3.x * GAME_SCALE;
	camera.layerWidth2 = s2.x * GAME_SCALE;
	camera.layerWidth1 = s1.x * GAME_SCALE;
}
void DrawParallaxLayer(sfRenderWindow* window, sfSprite* sprite, float factor, float width)
{
	sfVector2f camCenter = sfView_getCenter(camera.cameraView);

	float x = camCenter.x * factor;

	float startX = floorf(x / width) * width;

	for (int i = -2; i <= 2; i++)
	{
		sfSprite_setPosition(sprite, (sfVector2f) { startX + i * width, GetPlayerSpawn().y - sfTexture_getSize(camera.backgroundGame.texture3rdLayer).y * GAME_SCALE - 320 });
		sfRenderWindow_drawSprite(window, sprite, NULL);
	}
}
float ClampFloat(float v, float min, float max)
{
	if (v < min)
	{
		return min;
	}
	if (v > max)
	{
		return max;
	}
	return v;
}
void DrawBackgroundGame(sfRenderWindow* window)
{
	DrawParallaxLayer(window,
		camera.backgroundGame.sprite3rdLayer,
		0.4f,
		camera.layerWidth3);

	DrawParallaxLayer(window,
		camera.backgroundGame.sprite2ndLayer,
		0.6f,
		camera.layerWidth2);

	DrawParallaxLayer(window,
		camera.backgroundGame.sprite1stLayer,
		0.8f,
		camera.layerWidth1);
}
void LoadCamera()
{
	camera.cameraView = sfView_create();
	sfView_setCenter(camera.cameraView, player->data.position);
	sfView_setSize(camera.cameraView, (sfVector2f) { SCREEN_WIDTH, SCREEN_HEIGHT });
	camera.lastCamPos = player->data.position;
	LoadBackgroundGame();
}

void UpdateCamera(float _dt)
{
	CenterCamera(_dt);
}
void SnapCamera()
{
	sfView_setCenter(camera.cameraView, player->data.position);
}
void DrawCamera(sfRenderWindow* _renderWindow)
{

	sfRenderWindow_setView(_renderWindow, camera.cameraView);
	if (strcmp(player->data.level, "Level_06") == 0)
	{
		DrawBackgroundGame(_renderWindow);

	}
}

void CleanUpCamera()
{
	sfView_destroy(camera.cameraView);
}

void CenterCamera(float _dt)
{
	sfVector2f camPos = sfView_getCenter(camera.cameraView);

	float speed = 45.0f;
	float t = 1.0f - powf(1.0f - 0.1f, speed * _dt);

	camPos.x += (player->data.position.x - camPos.x) * t;
	camPos.y += (player->data.position.y - camPos.y) * t;

	if (strcmp(player->data.level, "Level_06") == 0)
	{
		float halfW = SCREEN_WIDTH / 2.0f;
		float halfH = SCREEN_HEIGHT / 2.0f;

		float minX = 0 + halfW;
		float maxX = 2880 * GAME_SCALE - halfW;

		float minY = 0 + halfH;
		float maxY = 320 * GAME_SCALE - halfH;

		camPos.x = ClampFloat(camPos.x, minX, maxX);
		camPos.y = ClampFloat(camPos.y, minY, maxY);

	}

	sfView_setCenter(camera.cameraView, camPos);
}