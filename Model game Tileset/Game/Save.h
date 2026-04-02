#ifndef SAVE_H
#define SAVE_H

#include "Common.h"


#define SAVE_VERSION 1
#define SAVE_PATH "Saves/save.dat"


typedef struct
{
    unsigned int version;
    int mapLevel;
    float health;

} PlayerSaveData;
sfBool SavePlayer(void);

PlayerSaveData* LoadSave(void);

void DeleteSave(void);

sfBool SaveExists(void);

#endif