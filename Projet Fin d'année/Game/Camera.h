#ifndef CAMERA_H
#define CAMERA_H

#include "Common.h"
#include "Player.h"

typedef struct BackgroundGame
{
	sfSprite* sprite1stLayer;
	sfSprite* sprite2ndLayer;
	sfSprite* sprite3rdLayer;

	sfTexture* texture1stLayer;
	sfTexture* texture2ndLayer;
	sfTexture* texture3rdLayer;

}BackgroundGame;
typedef struct Camera
{
	sfView* cameraView;
	sfVector2f cameraPosition;
	float layerWidth1;
	float layerWidth2;
	float layerWidth3;
	sfVector2f lastCamPos;
	BackgroundGame backgroundGame;
}Camera;


void LoadCamera();
void UpdateCamera(float _dt);
void DrawCamera(sfRenderWindow* _renderWindow);
void SnapCamera();
void CleanUpCamera();
#endif // !CAMERA_H
