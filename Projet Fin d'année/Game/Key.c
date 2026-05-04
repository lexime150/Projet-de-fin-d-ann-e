#include "Key.h"
#include "Player.h"

Key key[KEY_NUMBER];
Key extraKey[EXTRAS_KEY_NUMBER];
sfSprite* playerSprite;
Animation animPlayer; 
sfBool keyIsPressed[KEY_NUMBER] = { sfFalse, sfFalse };

void LoadKey(void)
{
	sfTexture* texture = sfTexture_createFromFile("Assets/Sprites/Game/Key/Key.png", NULL);
	sfTexture* textureExtraKey = sfTexture_createFromFile("Assets/Sprites/Game/Key/Keyboard Extras.png", NULL);
	sfTexture* texturePlayer = sfTexture_createFromFile("Assets/Sprites/Game/Player/playerUpD.png", NULL);

	CreateSprite(texturePlayer, &playerSprite, ORIGIN_VANILLA, (sfVector2f) { GetKeyTab(0).left, GetKeyTab(0).top + 100.f });
	sfSprite_setTextureRect(playerSprite, (sfIntRect) { 1 * 32.f, 1 * 32.f, 32.f, 32.f });
	sfSprite_setScale(playerSprite, (sfVector2f) { -2.5f, 2.5f });


	for (int i = 0; i < KEY_NUMBER; i++)
	{
		key[i].keySprite = sfSprite_create();
		sfSprite_setTexture(key[i].keySprite, texture, sfTrue);
		sfSprite_setOrigin(key[i].keySprite, (sfVector2f) { KEY_SIZE / 2, KEY_SIZE / 2 });
		sfSprite_setScale(key[i].keySprite, (sfVector2f) { GAME_SCALE + 1.f, GAME_SCALE + 1.f });
	}

	animPlayer = CreateAnimation(playerSprite, 5, 8, sfTrue, sfTrue, (sfIntRect) { 1 * PLAYER_WIDTH, 1 * PLAYER_HEIGHT, PLAYER_WIDTH, PLAYER_HEIGHT});
	

	KeyPosition((sfVector2f) { GetKeyTab(0).left, GetKeyTab(0).top }, 0, sfKeyD);
	KeyPosition((sfVector2f) { GetKeyTab(1).left, GetKeyTab(1).top }, 1, sfKeyQ);

	sfSprite_setPosition(playerSprite, (sfVector2f) { 1300.f, 2660.f });
}

void KeyPosition(sfVector2f _pos, int _keyNumber, int _i)
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
	sfSprite_setPosition(key[_keyNumber].keySprite, (sfVector2f) { _pos.x, _pos.y });
}

void UpdateKey(float _dt)
{
	for (int i = 0; i < KEY_NUMBER; i++)
	{
		key[i].cooldownAnimation += _dt;

		if (key[i].cooldownAnimation > 1.f)
		{
			keyIsPressed[i] = !keyIsPressed[i];
			key[i].cooldownAnimation = 0;
			sfIntRect temp = sfSprite_getTextureRect(key[i].keySprite);

			if (keyIsPressed[i] == sfTrue)
			{
				sfSprite_setTextureRect(key[i].keySprite, (sfIntRect) { temp.left, temp.top + (temp.height * 7), temp.width, temp.height });
			}
			else if(keyIsPressed[i] == sfFalse)
			{
				sfSprite_setTextureRect(key[i].keySprite, (sfIntRect) { temp.left, temp.top - (temp.height * 7),temp.width, temp.height });
			}
		
		}


	}


	UpdateAnimation(&animPlayer, _dt);



}


void DrawKey(sfRenderWindow* _renderWindow)
{
	//sfVector2f temp = sfSprite_getPosition(playerSprite);

	sfRenderWindow_drawSprite(_renderWindow, playerSprite, NULL);
	sfSprite_move(playerSprite, (sfVector2f) { 130.f, 0.f });
	sfSprite_setScale(playerSprite, (sfVector2f) { 2.5f, 2.5f });

	sfRenderWindow_drawSprite(_renderWindow, playerSprite, NULL);
	sfSprite_move(playerSprite, (sfVector2f) { -130.f, 0.f });
	sfSprite_setScale(playerSprite, (sfVector2f) { -2.5f, 2.5f });

	for (int i = 0; i < KEY_NUMBER; i++)
	{
		sfRenderWindow_drawSprite(_renderWindow, key[i].keySprite, NULL);
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
