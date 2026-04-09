#include "Map.h"


sfTexture* tileTexture;
sfSprite* tileSprite;
cute_tiled_map_t* map;

sfFloatRect* collisionTab;
unsigned int collisionTabSize;
Trigger* triggerTab;
unsigned int triggerTabSize;

sfFloatRect* spikeTab;
unsigned spikeTabSize;
sfFloatRect* deathZoneTab;
unsigned int deathZoneTabSize;

sfVector2f* enemySpawnTab;
unsigned int enemySpawnTabSize;

sfVector2f playerSpawn;

void LoadCollisionAndTrigger(void);
void DrawTileLayer(sfRenderWindow* _renderWindow, cute_tiled_layer_t* _layer);
void DrawObjectGroup(sfRenderWindow* _renderWindow, cute_tiled_layer_t* _layer);

void LoadMap(char* _mapName)
{
	char filename[FILENAME_MAX];

	// Load the map
	sprintf_s(filename, FILENAME_MAX, "Assets/Map/Levels/%s.json", _mapName);
	map = cute_tiled_load_map_from_file(filename, NULL);

	// Load the texture used as Tileset
	sprintf_s(filename, FILENAME_MAX, "Assets/Map/Tilesets/%s", map->tilesets->image.ptr);
	tileTexture = sfTexture_createFromFile(filename, NULL);

	// Create the sprite used to draw each tile
	tileSprite = sfSprite_create();
	sfSprite_setTexture(tileSprite, tileTexture, sfTrue);
	sfSprite_setScale(tileSprite, (sfVector2f) { GAME_SCALE, GAME_SCALE });

	// Load the layer of collisions and triggers
	LoadCollisionAndTrigger();
}

void DrawMap(sfRenderWindow* _renderWindow)
{
	cute_tiled_layer_t* layer = map->layers;

	// Parse all the layers
	while (layer)
	{
		if (layer->visible)
		{
			if (strcmp(layer->type.ptr, "tilelayer") == 0)
			{
				DrawTileLayer(_renderWindow, layer);
			}
			else if (strcmp(layer->type.ptr, "objectgroup") == 0)
			{
				DrawObjectGroup(_renderWindow, layer);
			}
		}

		layer = layer->next;
	}
}

void CleanupMap(void)
{
	sfTexture_destroy(tileTexture);
	tileTexture = NULL;

	sfSprite_destroy(tileSprite);
	tileSprite = NULL;

	cute_tiled_free_map(map);
	map = NULL;

	free(collisionTab);
	collisionTab = NULL;

	free(spikeTab);
	spikeTab = NULL;
	spikeTabSize = 0;

	free(triggerTab);
	triggerTab = NULL;

	free(enemySpawnTab);
	enemySpawnTab = NULL;


}

void LoadCollisionAndTrigger(void)
{
	// Create the array of collisions
	collisionTab = calloc(1, sizeof(sfFloatRect));
	collisionTabSize = 0;
	if (collisionTab == NULL)
	{
		return;
	}

	// Create the array of triggers
	triggerTab = calloc(1, sizeof(Trigger));
	triggerTabSize = 0;
	if (triggerTab == NULL)
	{
		return;
	}

	spikeTab = calloc(1, sizeof(sfFloatRect));
	spikeTabSize = 0;
	if (spikeTab == NULL)
	{
		return;
	}

	deathZoneTab = calloc(1, sizeof(sfFloatRect));
	deathZoneTabSize = 0;
	if (deathZoneTab == NULL)
	{
		return;
	}




	enemySpawnTab = calloc(1, sizeof(sfVector2f));
	enemySpawnTabSize = 0;

	playerSpawn = (sfVector2f){ 0, 0 };


	// Select the first layer
	cute_tiled_layer_t* layer = map->layers;

	// Parse all the layers
	while (layer)
	{
		cute_tiled_object_t* object = layer->objects;

		while (object)
		{
			if (object->ellipse == 0 && object->point == 0 && object->vertices == 0)
			{
				if (strcmp(layer->name.ptr, "Collision") == 0)
				{
					// Resize the collision array
					sfFloatRect* collisionTabTemp = realloc(collisionTab, (unsigned long long)(collisionTabSize + 1) * sizeof(sfFloatRect));
					if (collisionTabTemp == NULL)
					{
						return;
					}


					collisionTab = collisionTabTemp;

					// Add the collision in the array
					collisionTab[collisionTabSize] = (sfFloatRect){
						object->x * GAME_SCALE,
						object->y * GAME_SCALE,
						object->width * GAME_SCALE,
						object->height * GAME_SCALE };
					collisionTabSize++;
				}
				else if (strcmp(layer->name.ptr, "Trigger") == 0)
				{
					// Resize the trigger array
					Trigger* triggerTabTemp = realloc(triggerTab, (unsigned long long)(triggerTabSize + 1) * sizeof(Trigger));
					if (triggerTabTemp == NULL)
					{
						return;
					}
					triggerTab = triggerTabTemp;

					// Add the collision in the array
					strcpy_s(triggerTab[triggerTabSize].name, FILENAME_MAX, object->name.ptr);
					triggerTab[triggerTabSize].left = object->x * GAME_SCALE;
					triggerTab[triggerTabSize].top = object->y * GAME_SCALE;
					triggerTab[triggerTabSize].width = object->width * GAME_SCALE;
					triggerTab[triggerTabSize].height = object->height * GAME_SCALE;
					triggerTabSize++;
				}
				else if (strcmp(layer->name.ptr, "Spike") == 0)
				{

					sfFloatRect* spikeTemp = realloc(spikeTab, (unsigned long long)(spikeTabSize + 1) * sizeof(sfFloatRect));
					if (spikeTemp == NULL)
					{
						return;
					}
					spikeTab = spikeTemp;

					spikeTab[spikeTabSize] = (sfFloatRect){
					object->x * GAME_SCALE,
					object->y * GAME_SCALE,
					object->width * GAME_SCALE,
					object->height * GAME_SCALE };
					spikeTabSize++;
				}
				else if (strcmp(layer->name.ptr, "DeathZone") == 0)
				{
					sfFloatRect* deathZoneTemp = realloc(deathZoneTab, (unsigned long long)(deathZoneTabSize + 1) * sizeof(sfFloatRect));
					if (deathZoneTemp == NULL)
					{
						return;
					}
					deathZoneTab = deathZoneTemp;

					deathZoneTab[deathZoneTabSize] = (sfFloatRect){
						object->x * GAME_SCALE,
						object->y * GAME_SCALE,
						object->width * GAME_SCALE,
						object->height * GAME_SCALE };
						deathZoneTabSize++;
				}

				
			}
			



			if (object->point == 1)
			{
				if (strcmp(layer->name.ptr, "Enemy-Spawn") == 0)
				{
					sfVector2f* enemySpawnTabTemp = realloc(enemySpawnTab, (unsigned long long)(enemySpawnTabSize + 1) * sizeof(sfVector2f));
					if (enemySpawnTabTemp == NULL) return;
					enemySpawnTab = enemySpawnTabTemp;

					enemySpawnTab[enemySpawnTabSize] = (sfVector2f){
						object->x * GAME_SCALE,
						object->y * GAME_SCALE
					};
					enemySpawnTabSize++;
				}
				else if (strcmp(layer->name.ptr, "Player-Spawn") == 0)
				{
					playerSpawn = (sfVector2f){
						object->x * GAME_SCALE,
						object->y * GAME_SCALE
					};
				}
			}
			// Next object
			object = object->next;
		}
		// Next layer
		layer = layer->next;
	}
}

void DrawTileLayer(sfRenderWindow* _renderWindow, cute_tiled_layer_t* _layer)
{
	sfSprite_setColor(tileSprite, (sfColor) { 255, 255, 255, (int)(_layer->opacity * 255) });

	int tileWidth = map->tilewidth;
	int tileHeight = map->tileheight;

	for (int line = 0; line < _layer->height; line++)
	{
		for (int column = 0; column < _layer->width; column++)
		{
			int tileId = _layer->data[line * _layer->width + column] - 1;
			if (tileId >= 0)
			{
				int tileX = (tileId % map->tilesets->columns) * tileWidth;
				int tileY = (tileId / map->tilesets->columns) * tileHeight;
				sfSprite_setTextureRect(tileSprite, (sfIntRect) { tileX, tileY, tileWidth, tileHeight });

				sfSprite_setPosition(tileSprite, (sfVector2f) { (float)column* tileWidth* GAME_SCALE, (float)line* tileHeight* GAME_SCALE });
				sfRenderWindow_drawSprite(_renderWindow, tileSprite, NULL);
			}
		}
	}
}

void DrawObjectGroup(sfRenderWindow* _renderWindow, cute_tiled_layer_t* _layer)
{
	cute_tiled_object_t* object = _layer->objects;

	while (object)
	{
		if (object->ellipse == 0 && object->point == 0 && object->vertices == 0)
		{
			sfRectangleShape* rectangle = sfRectangleShape_create();
			sfRectangleShape_setPosition(rectangle, (sfVector2f) { object->x* GAME_SCALE, object->y* GAME_SCALE});
			sfRectangleShape_setSize(rectangle, (sfVector2f) { object->width* GAME_SCALE, object->height* GAME_SCALE});
			sfRectangleShape_setFillColor(rectangle, sfTransparent);

			if (strcmp(_layer->name.ptr, "Collision") == 0)
			{
				sfRectangleShape_setOutlineColor(rectangle, (sfColor) { 255, 0, 0, (int)(_layer->opacity * 255) });
			}
			else
			{
				sfRectangleShape_setOutlineColor(rectangle, (sfColor) { 255, 255, 255, (int)(_layer->opacity * 255) });
			}
			sfRectangleShape_setOutlineThickness(rectangle, 1);

			sfRenderWindow_drawRectangleShape(_renderWindow, rectangle, NULL);
			sfRectangleShape_destroy(rectangle);
		}

		object = object->next;
	}
}

unsigned int GetCollisionTabSize(void)
{
	return collisionTabSize;
}

unsigned int GetTriggerTabSize(void)
{
	return triggerTabSize;
}

sfFloatRect GetMapCollision(unsigned int _index)
{
	if (_index < collisionTabSize)
	{
		return collisionTab[_index];
	}
	else
	{
		return (sfFloatRect) { 0, 0, 0, 0 };
	}
}

Trigger GetMapTrigger(unsigned int _index)
{
	if (_index < triggerTabSize)
	{
		return triggerTab[_index];
	}
	else
	{
		return (Trigger) { "NoName", 0, 0, 0, 0 };
	}
}

unsigned int GetEnemySpawnTabSize(void)
{
    return enemySpawnTabSize;
}

sfVector2f GetEnemySpawn(unsigned int _index)
{
    if (_index < enemySpawnTabSize)
        return enemySpawnTab[_index];
    else
        return (sfVector2f){ 0, 0 };
}

sfVector2f GetPlayerSpawn(void)
{
    return playerSpawn;
}

sfFloatRect GetSpikeTab(unsigned _index)
{
	if (_index < spikeTabSize)
		return spikeTab[_index];
	else
		return (sfFloatRect) { 0 };
}

unsigned GetSpikeTabSize(void)
{
	return spikeTabSize;
}


unsigned int GetDeathZoneTabSize(void)
{
	return deathZoneTabSize;
}

sfFloatRect GetDeathZoneTab(unsigned int _index)
{
	if (_index < deathZoneTabSize)
	{
		return deathZoneTab[_index];
	}
	else
	{
		return (sfFloatRect) { 0, 0, 0, 0 };
	}
}