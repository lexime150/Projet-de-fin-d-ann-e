#include "Save.h"
#include "Player.h"

Player player;


sfBool SavePlayer(void)
{
    FILE* f = fopen(SAVE_PATH, "wb");
    if (!f)
    {
        printf("[Save] Erreur : impossible d'ouvrir %s en écriture.\n", SAVE_PATH);
        return sfFalse;
    }

    PlayerSaveData save;


    save.version = SAVE_VERSION;
    save.health = player.data.health;



    size_t written = fwrite(&save, sizeof(PlayerSaveData), 1, f);
    fclose(f);

    if (written != 1)
    {
        printf("[Save] Erreur : écriture incomplète.\n");
        return sfFalse;
    }

    printf("[Save] Sauvegarde réussie. (hp=%.0f)\n", save.health);
    return sfTrue;
}


PlayerSaveData* LoadSave(void)
{
    FILE* f = fopen(SAVE_PATH, "rb");
    if (!f)
    {
        printf("[Save] Aucune save trouvée.\n");
        return NULL;
    }

    PlayerSaveData save;
    size_t read = fread(&save, sizeof(PlayerSaveData), 1, f);
    fclose(f);

    if (read != 1)
    {
        printf("[Save] Erreur : lecture incomplète.\n");
        return NULL;
    }

    if (save.version != SAVE_VERSION)
    {
        printf("[Save] Version incompatible (save=%u, attendu=%u). Save ignorée.\n",
            save.version, SAVE_VERSION);
        return NULL;
    }

    printf("[Save] Save chargée. (hp=%.0f\n",
        save.health);
    return &save;
}


void DeleteSave(void)
{
    if (remove(SAVE_PATH) == 0)
        printf("[Save] Fichier supprimé.\n");
    else
        printf("[Save] Aucun fichier à supprimer.\n");
}

sfBool SaveExists(void)
{
    FILE* f = fopen(SAVE_PATH, "rb");
    if (!f) return sfFalse;
    fclose(f);
    return sfTrue;
}
