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

	camera.layerWidth3 = s3.x * 1.5f * GAME_SCALE;
	camera.layerWidth2 = s2.x * 1.5f * GAME_SCALE;
	camera.layerWidth1 = s1.x * 1.5f * GAME_SCALE;
}
void DrawParallaxLayer(sfRenderWindow* window, sfSprite* sprite, sfTexture* texture, float factor, float width, float layerHeight)
{
	sfVector2f camCenter = sfView_getCenter(camera.cameraView);

	float limitLeft = 0.0f;
	float limitTop = 0.0f;
	if (HasCameraLimit())
	{
		sfFloatRect limit = GetCameraLimit();
		limitLeft = limit.left;
		limitTop = limit.top;
	}

	float scrollX = -(camCenter.x - limitLeft) * factor;

	float baseX = fmodf(scrollX, width);
	if (baseX > 0) baseX -= width;

	float leftEdge = camCenter.x - SCREEN_WIDTH / 2.0f;

	float posY = limitTop;

	for (int i = 0; i <= 2; i++)
	{
		float posX = leftEdge + baseX + i * width;
		sfSprite_setPosition(sprite, (sfVector2f) { posX, posY });
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
	sfVector2u s3 = sfTexture_getSize(camera.backgroundGame.texture3rdLayer);
	sfVector2u s2 = sfTexture_getSize(camera.backgroundGame.texture2ndLayer);
	sfVector2u s1 = sfTexture_getSize(camera.backgroundGame.texture1stLayer);

	DrawParallaxLayer(window, camera.backgroundGame.sprite3rdLayer, camera.backgroundGame.texture3rdLayer, 0.1f, camera.layerWidth3, (float)s3.y);
	DrawParallaxLayer(window, camera.backgroundGame.sprite2ndLayer, camera.backgroundGame.texture2ndLayer, 0.15f, camera.layerWidth2, (float)s2.y);
	DrawParallaxLayer(window, camera.backgroundGame.sprite1stLayer, camera.backgroundGame.texture1stLayer, 0.2f, camera.layerWidth1, (float)s1.y);
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
	if (HasCameraLimit())
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

	if (HasCameraLimit())
	{
		sfFloatRect limit = GetCameraLimit();
		float halfW = SCREEN_WIDTH / 2.0f;
		float halfH = SCREEN_HEIGHT / 2.0f;

		float minX = limit.left + halfW;
		float maxX = limit.left + limit.width - halfW;

		float minY = limit.top + halfH;
		float maxY = limit.top + limit.height - halfH;

		camPos.x = ClampFloat(camPos.x, minX, maxX);
		camPos.y = ClampFloat(camPos.y, minY, maxY);
	}

	sfView_setCenter(camera.cameraView, camPos);
}