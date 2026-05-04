#ifndef SAVE_H
#define SAVE_H

#include "Common.h"
#include <stdio.h>


#define SAVE_VERSION    1
#define SAVE_SLOT_COUNT 3
#define SAVE_PATH_FMT  "Saves/save_%d.dat"


typedef struct
{
    unsigned int save; 
    char level[20];
    float health;
    sfBool canWallJump;
    sfBool doubleJumpUnlocked;

	
    sfBool dashUnlocked;
	sfBool upDashUnlocked;
	sfBool diagonalDashUnlocked;
	sfBool horizontalDashUnlocked;

    sfVector2f position;

} PlayerSaveData;


extern PlayerSaveData playerSaveData;


sfBool SavePlayer(int slot);


PlayerSaveData* LoadSave(int slot);


void DeleteSave(int slot);


sfBool SaveExists(int slot);


void GetSavePath(int slot, char* path);

#endif