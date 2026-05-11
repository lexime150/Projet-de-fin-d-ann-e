#ifndef CAMERA_H
#define CAMERA_H

#include "Common.h"
#include "Player.h"

typedef struct Camera
{
	sfView* cameraView;
	sfVector2f cameraPosition;

}Camera;


void LoadCamera();
void UpdateCamera(float _dt);
void DrawCamera(sfRenderWindow* _renderWindow);
void SnapCamera();
void CleanUpCamera();
#endif // !CAMERA_H
