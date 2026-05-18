#ifndef MAP_H
#define MAP_H

#include "Common.h"
#include "cute_tiled.h"
typedef struct AnimatedTile
{
	int tileId;

	unsigned frameCount;
	int* frames;
	int currentFrame;

	float timer;
	float* durations;
} AnimatedTile;

typedef struct Trigger
{
	char name[FILENAME_MAX];
	float left;
	float top;
	float width;
	float height;
}Trigger;

typedef struct KeyStruct
{
	char name[FILENAME_MAX];
	float left;
	float top;
	float width;
	float height;
}KeyStruct;

void LoadMap(char* _mapName);
void UpdateMap(float _dt);
void DrawMap(sfRenderWindow* _renderWindow);
void CleanupMap(void);

unsigned int GetCollisionTabSize(void);
unsigned int GetTriggerTabSize(void);
sfFloatRect GetMapCollision(unsigned int _index);
Trigger GetMapTrigger(unsigned int _index);

unsigned int GetEnemySpawnTabSize(void);
sfVector2f GetEnemySpawn(unsigned int _index);

sfVector2f GetSkeletonSpawn(unsigned int _index);
sfVector2f GetMushroomSpawn(unsigned int _index);
unsigned int GetSkeletonSpawnTabSize();
unsigned int GetMushroomSpawnTabSize();
sfVector2f GetPlayerSpawn(void);


sfFloatRect GetSpikeTab(unsigned _index);
unsigned GetSpikeTabSize(void);
unsigned int GetDeathZoneTabSize(void);
sfFloatRect GetDeathZoneTab(unsigned int _index);

unsigned int GetSemiSolidCollisionTabSize(void);
sfFloatRect GetSemiSolidCollisionTab(unsigned int _index);

void ChangeLevel(const char* level);
unsigned int GetKeyTabSize();
KeyStruct GetKeyTab(unsigned _index);

sfVector2f GetFlyMobSpawn(unsigned int _index);
unsigned int GetFlyMobSpawnTabSize();

sfFloatRect GetCameraLimit(void);
sfBool HasCameraLimit(void);

unsigned int GetLadderTabSize(void);
sfFloatRect GetLadderTab(unsigned int _index);
#endif
