#ifndef ANGEL_STATUE
#define ANGEL_STATUE
#include "Common.h"
#include "Player.h"
#include "Map.h"

typedef struct AngelStatue
{
	sfSprite* sprite;
	sfVector2f position;
	sfBool isActivated;

} AngelStatue;

void LoadAngelStatue(void);
void UpdateAngelStatue(float _dt);
void CleanupAngelStatue(void);


#endif // !ANGEL_STATUE
