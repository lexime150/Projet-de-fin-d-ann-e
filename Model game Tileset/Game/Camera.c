#include "Camera.h"

Camera camera;
Player player;
void CenterCamera();
void LoadCamera()
{
	camera.cameraView = sfView_create();
	sfView_setCenter(camera.cameraView, player.position);
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
	camPos.x += (player.position.x - camPos.x) * 0.1f;
	camPos.y += (player.position.y - camPos.y) * 0.1f;

	if (camPos.y > SCREEN_HEIGHT / 2)
	{
		camPos.y = SCREEN_HEIGHT / 2;
	}
	sfView_setCenter(camera.cameraView, camPos);
}
