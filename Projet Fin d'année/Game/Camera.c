#include "Camera.h"

Camera camera;
Player* player;
void CenterCamera(float _dt);
void LoadCamera()
{
	camera.cameraView = sfView_create();
	sfView_setCenter(camera.cameraView, player->data.position);
	sfView_setSize(camera.cameraView, (sfVector2f) { SCREEN_WIDTH, SCREEN_HEIGHT });
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
	sfView_setCenter(camera.cameraView, camPos);
}
