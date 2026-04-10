#include "Key.h"

Key key[KEY_NUMBER];


void LoadKey(void)
{
	sfTexture* texture = sfTexture_createFromFile("Assets/Sprites/Key.png", NULL);

	for (int i = 0; i < KEY_NUMBER; i++)
	{
		key[i].keySprite = sfSprite_create();
		sfSprite_setTexture(key[i].keySprite, texture, sfTrue);
		sfSprite_setOrigin(key[i].keySprite, (sfVector2f){KEY_SIZE / 2, KEY_SIZE / 2});
		sfSprite_setScale(key[i].keySprite, (sfVector2f){ GAME_SCALE, GAME_SCALE});


	}

	KeyPosition((sfVector2f) { 1400.f, 2650.f }, 0, sfKeyQ);
	KeyPosition((sfVector2f) { 1600.f, 2650.f }, 1, sfKeyD);

}

void KeyPosition(sfVector2f _pos , int _keyNumber, int _i)
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

	sfSprite_setTextureRect(key[_keyNumber].keySprite, (sfIntRect){width, height, KEY_SIZE, KEY_SIZE});
	sfSprite_setPosition(key[_keyNumber].keySprite, (sfVector2f){_pos.x, _pos.y});
}

void UpdateKey(float _dt)
{
	for (int i = 0; i < KEY_NUMBER; i++)
	{





	}



}


void DrawKey(sfRenderWindow* _renderWindow)
{
	for (int i = 0; i < KEY_NUMBER; i++)
	{
	//	sfRenderWindow_drawSprite(_renderWindow, key[i].keySprite, NULL);
	}
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
	}

}
