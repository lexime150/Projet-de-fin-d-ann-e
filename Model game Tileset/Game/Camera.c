#include "Camera.h"

Camera camera;
Player player;
void CenterCamera();
void LoadCamera()
{
	camera.cameraView = sfView_create();
	sfView_setCenter(camera.cameraView, player.data.position);
	sfView_setSize(camera.cameraView, (sfVector2f) { SCREEN_WIDTH, SCREEN_HEIGHT });
}

void UpdateCamera(float _dt)
{
	CenterCamera();
}

void DrawCamera(sfRenderWindow* _renderWindow)
{
	sfRenderWindow_setView(_renderWindow, camera.cameraView);
}

void CleanUpCamera()
{
}

void CenterCamera()
{
	sfVector2f camPos = sfView_getCenter(camera.cameraView);
	camPos.x += (player.data.position.x - camPos.x) * 0.1f;
	camPos.y += (player.data.position.y - camPos.y) * 0.1f;

	sfView_setCenter(camera.cameraView, camPos);
}
