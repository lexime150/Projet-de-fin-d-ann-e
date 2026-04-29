#include "Save.h"
#include "Player.h"

extern Player* player;
PlayerSaveData playerSaveData;

void GetSavePath(int slot, char* path)
{
	snprintf(path, 32, SAVE_PATH_FMT, slot);
}

static sfBool IsValidSlot(int slot)
{
	if (slot < 1 || slot > SAVE_SLOT_COUNT)
	{
		printf("[Save] Slot invalide : %d (attendu 1-%d).\n", slot, SAVE_SLOT_COUNT);
		return sfFalse;
	}
	return sfTrue;
}

sfBool SavePlayer(int slot)
{
	if (!IsValidSlot(slot))
		return sfFalse;

	if (!player)
	{
		printf("[Save] Erreur : pointeur player NULL.\n");
		return sfFalse;
	}

	char path[32];
	GetSavePath(slot, path);

	FILE* f = fopen(path, "wb");
	if (!f)
	{
		printf("[Save] Erreur : impossible d'ouvrir %s en écriture.\n", path);
		return sfFalse;
	}

	PlayerSaveData save;
	save.save = SAVE_VERSION;
	save.canDoubleJump = player->data.canDoubleJump;
	save.canWallJump = player->data.canWallJump;
	snprintf(save.level, sizeof(save.level), "%s", player->data.level);

	size_t written = fwrite(&save, sizeof(PlayerSaveData), 1, f);
	fclose(f);

	if (written != 1)
	{
		printf("[Save] Erreur : écriture incomplète dans %s.\n", path);
		return sfFalse;
	}

	return sfTrue;
}

PlayerSaveData* LoadSave(int slot)
{
	if (!IsValidSlot(slot))
		return NULL;

	char path[32];
	GetSavePath(slot, path);

	FILE* f = fopen(path, "rb");
	if (!f)
	{
		printf("[Save] Slot %d : aucune save trouvée (%s).\n", slot, path);
		return NULL;
	}

	PlayerSaveData tmp;
	size_t read = fread(&tmp, sizeof(PlayerSaveData), 1, f);
	fclose(f);

	if (read != 1)
	{
		printf("[Save] Slot %d : lecture incomplète.\n", slot);
		return NULL;
	}

	if (tmp.save != SAVE_VERSION)
	{
		printf("[Save] Slot %d : version incompatible (save=%u, attendu=%u). Save ignorée.\n",
			slot, tmp.save, SAVE_VERSION);
		return NULL;
	}

	playerSaveData = tmp;
	playerSaveData.save = slot;

	if (player)
	{
		snprintf(player->data.level, sizeof(player->data.level), "%s", playerSaveData.level);
		player->data.canDoubleJump = playerSaveData.canDoubleJump;
		player->data.canWallJump = playerSaveData.canWallJump;
	}

	printf("[Save] Slot %d chargé. (hp=%.0f)\n", slot, playerSaveData.health);
	return &playerSaveData;
}

void DeleteSave(int slot)
{
	if (!IsValidSlot(slot))
		return;

	char path[32];
	GetSavePath(slot, path);

	if (remove(path) == 0)
		printf("[Save] Slot %d supprimé (%s).\n", slot, path);
	else
		printf("[Save] Slot %d : aucun fichier à supprimer.\n", slot);
}

sfBool SaveExists(int slot)
{
	if (!IsValidSlot(slot))
		return sfFalse;

	char path[32];
	GetSavePath(slot, path);

	FILE* f = fopen(path, "rb");
	if (!f)
		return sfFalse;

	fclose(f);
	return sfTrue;
}