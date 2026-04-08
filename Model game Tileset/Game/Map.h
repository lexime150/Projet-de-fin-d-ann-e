#ifndef MAP_H
#define MAP_H

#include "Common.h"
#include "cute_tiled.h"

typedef struct Trigger
{
	char name[FILENAME_MAX];
	float left;
	float top;
	float width;
	float height;
}Trigger;

void LoadMap(char* _mapName);
void DrawMap(sfRenderWindow* _renderWindow);
void CleanupMap(void);

unsigned int GetCollisionTabSize(void);
unsigned int GetTriggerTabSize(void);
sfFloatRect GetMapCollision(unsigned int _index);
Trigger GetMapTrigger(unsigned int _index);

unsigned int GetEnemySpawnTabSize(void);
sfVector2f GetEnemySpawn(unsigned int _index);

sfVector2f GetPlayerSpawn(void);



sfFloatRect GetSpikeTab(unsigned _index);
unsigned GetSpikeTabSize(void);
void ChangeLevel(const char* level);


#endif
