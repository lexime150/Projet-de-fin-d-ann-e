#include "Key.h"
#include "Player.h"

Key key[KEY_NUMBER] = { 0 };
Key extraKey[EXTRAS_KEY_NUMBER] = { 0 };
sfSprite* playerSprite;
Animation animPlayer;
Player* player;


void UpdateNormalKey(float _dt);
void UpdateExtraKey(float _dt);


void LoadKey(void)
{

	if (strcmp(player->data.level, "Level_00") == 0)
	{

		sfTexture* texture = sfTexture_createFromFile("Assets/Sprites/Game/Key/Key.png", NULL);
		sfTexture* textureExtraKey = sfTexture_createFromFile("Assets/Sprites/Game/Key/Keyboard Extras.png", NULL);
		sfTexture* texturePlayer = sfTexture_createFromFile("Assets/Sprites/Game/Player/playerUpD.png", NULL);

		CreateSprite(texturePlayer, &playerSprite, ORIGIN_VANILLA, (sfVector2f) { GetKeyTab(0).left, GetKeyTab(0).top + 100.f });
		sfSprite_setTextureRect(playerSprite, (sfIntRect) { 1 * 32, 1 * 32, 32, 32 });
		sfSprite_setScale(playerSprite, (sfVector2f) { 2.5f, 2.5f });


		for (int i = 0; i < KEY_NUMBER; i++)
		{
			key[i].keySprite = sfSprite_create();
			sfSprite_setTexture(key[i].keySprite, texture, sfTrue);
			sfSprite_setOrigin(key[i].keySprite, (sfVector2f) { KEY_SIZE / 2, KEY_SIZE / 2 });
			sfSprite_setScale(key[i].keySprite, (sfVector2f) { GAME_SCALE + 1.f, GAME_SCALE + 1.f });
			key[i].isKeyPressed = sfFalse;
		}

		for (int i = 0; i < EXTRAS_KEY_NUMBER; i++)
		{
			CreateSprite(textureExtraKey, &extraKey[i].keySprite, ORIGIN_VANILLA, (sfVector2f) { GetKeyTab((KEY_NUMBER - 1) + i).left, GetKeyTab((KEY_NUMBER - 1) + i).top });
			sfSprite_setScale(extraKey[i].keySprite, (sfVector2f) { GAME_SCALE, GAME_SCALE });
			extraKey[i].isKeyPressed = sfFalse;
		}

		animPlayer = CreateAnimation(playerSprite, 5, 8, sfTrue, sfTrue, (sfIntRect) { 1 * PLAYER_WIDTH, 1 * PLAYER_HEIGHT, PLAYER_WIDTH, PLAYER_HEIGHT });


		KeyPosition((sfVector2f) { GetKeyTab(0).left, GetKeyTab(0).top }, 0, sfKeyQ);
		KeyPosition((sfVector2f) { GetKeyTab(1).left, GetKeyTab(1).top }, 1, sfKeyD);

		ExtraKeyPosition((sfVector2f) { GetKeyTab(2).left, GetKeyTab(2).top }, 0, SPACE);

		sfSprite_setPosition(playerSprite, (sfVector2f) { 1360.f, 2660.f });
	}


}

void KeyPosition(sfVector2f _pos, int _keyNumber, sfKeyCode _i)
{
	int mod = 8;
	int width = (_i % mod) * KEY_SIZE;
	int height;

	if (_i >= mod)
	{
		if (_i >= mod * 2)
		{
			if (_i >= mod * 3)
			{

				height = (KEY_HEIGHT_MIN + 3) * KEY_SIZE;
			}
			else
			{
				height = (KEY_HEIGHT_MIN + 2) * KEY_SIZE;
			}
		}
		else
		{
			height = (KEY_HEIGHT_MIN + 1) * KEY_SIZE;
		}
	}
	else
	{
		height = KEY_HEIGHT_MIN * KEY_SIZE;
	}

	sfSprite_setTextureRect(key[_keyNumber].keySprite, (sfIntRect) { width, height, KEY_SIZE, KEY_SIZE });
	sfSprite_setPosition(key[_keyNumber].keySprite, _pos);
}

void ExtraKeyPosition(sfVector2f _pos, int _keyNumber, sfKeyCode _i)
{
	int mod = 4;
	float scale = sfSprite_getScale(extraKey[_keyNumber].keySprite).x;
	int width = (_i % mod) * EXTRAS_KEY_SIZE.x;
	int height = 0;

	if (_i >= mod)
	{
		if (_i >= mod * 2)
		{
			if (_i >= mod * 3)
			{
				height = 3 * (int)EXTRAS_KEY_SIZE.y;
			}
			else
			{
				height = 2 * (int)EXTRAS_KEY_SIZE.y;
			}
		}
		else
		{
			height = scale * (int)EXTRAS_KEY_SIZE.y;
		}

	}

	sfSprite_setTextureRect(extraKey[_keyNumber].keySprite, (sfIntRect) { width, height, EXTRAS_KEY_SIZE.x, EXTRAS_KEY_SIZE.y });
	sfSprite_setPosition(extraKey[_keyNumber].keySprite, _pos);

}

void UpdateKey(float _dt)
{

	UpdateNormalKey(_dt);
	UpdateExtraKey(_dt);

	UpdateAnimation(&animPlayer, _dt);



}

void UpdateNormalKey(float _dt)
{
	for (int i = 0; i < KEY_NUMBER; i++)
	{
		if (key[i].keySprite)
		{
			key[i].cooldownAnimation += _dt;

			if (key[i].cooldownAnimation > 1.f)
			{
				key[i].isKeyPressed = !key[i].isKeyPressed;
				key[i].cooldownAnimation = 0;
				sfIntRect temp = sfSprite_getTextureRect(key[i].keySprite);

				if (key[i].isKeyPressed == sfTrue)
				{
					sfSprite_setTextureRect(key[i].keySprite, (sfIntRect) { temp.left, temp.top + (temp.height * 7), temp.width, temp.height });
				}
				else if (key[i].isKeyPressed == sfFalse)
				{
					sfSprite_setTextureRect(key[i].keySprite, (sfIntRect) { temp.left, temp.top - (temp.height * 7), temp.width, temp.height });
				}

			}

		}


	}
}

void UpdateExtraKey(float _dt)
{
	for (int i = 0; i < EXTRAS_KEY_NUMBER - 2; i++)
	{
		if (extraKey[i].keySprite)
		{
			extraKey[i].cooldownAnimation += _dt;

			if (extraKey[i].cooldownAnimation > 1.f)
			{
				extraKey[i].isKeyPressed = !extraKey[i].isKeyPressed;
				extraKey[i].cooldownAnimation = 0;
				sfIntRect temp = sfSprite_getTextureRect(extraKey[i].keySprite);

				if (extraKey[i].isKeyPressed == sfTrue)
				{
					sfSprite_setTextureRect(extraKey[i].keySprite, (sfIntRect) { temp.left, temp.top + (temp.height * 4), temp.width, temp.height });
				}
				else if (extraKey[i].isKeyPressed == sfFalse)
				{
					sfSprite_setTextureRect(extraKey[i].keySprite, (sfIntRect) { temp.left, temp.top - (temp.height * 4), temp.width, temp.height });
				}

			}

		}


	}


}

void DrawKey(sfRenderWindow* _renderWindow)
{
	//sfVector2f temp = sfSprite_getPosition(playerSprite);
	//sfSprite_setScale(playerSprite, (sfVector2f) { 2.5f, 2.5f });
	if (playerSprite)
	{
		sfRenderWindow_drawSprite(_renderWindow, playerSprite, NULL);

	}



	for (int i = 0; i < KEY_NUMBER; i++)
	{
		if (key[i].keySprite)
		{
			sfRenderWindow_drawSprite(_renderWindow, key[i].keySprite, NULL);

		}
	}
	//	for (int i = 0; i < EXTRAS_KEY_NUMBER; i++)
	//	{
	if (extraKey[0].keySprite)
	{
		sfRenderWindow_drawSprite(_renderWindow, extraKey[0].keySprite, NULL);

	}
	//	}
}

void CleanupKey(void)
{
	if (key != NULL)
	{
		for (int i = 0; i < KEY_NUMBER; i++)
		{
			sfSprite_destroy(key[i].keySprite);
			key[i].keySprite = NULL;
		}
		for (int i = 0; i < EXTRAS_KEY_NUMBER; i++)
		{
			sfSprite_destroy(extraKey[i].keySprite);
			extraKey[i].keySprite = NULL;
		}
	}

	sfSprite_destroy(playerSprite);
	playerSprite = NULL;
}
