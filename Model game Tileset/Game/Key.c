#include "Key.h"

sfSprite* keySprite[KEY_NUMBER];



void LoadKey(void)
{
	sfTexture* texture = sfTexture_createFromFile("Assets/Sprites/Key.png", NULL);

	for (int i = 0; i < KEY_NUMBER; i++)
	{
		keySprite[i] = sfSprite_create();
		sfSprite_setTexture(keySprite[i], texture, sfTrue);
		sfSprite_setOrigin(keySprite[i], (sfVector2f){KEY_SIZE / 2, KEY_SIZE / 2});
		sfSprite_setScale(keySprite[i], (sfVector2f){ GAME_SCALE, GAME_SCALE});


	}

	KeyPosition(6000, 1000, 1, 5, 0);


}

void KeyPosition(float _x, float _y, float _textureRectX, int _textureRectY ,int _keyNumber)
{
	sfSprite_setTextureRect(keySprite[_keyNumber], (sfIntRect){_textureRectX, _textureRectY, KEY_SIZE, KEY_SIZE});

	sfSprite_setPosition(keySprite[_keyNumber], (sfVector2f){_x, _y});
}


void DrawKey(sfRenderWindow* _renderWindow)
{
	/*for (int i = 0; i < KEY_NUMBER; i++)
	{
		//sfRenderWindow_drawSprite(_renderWindow, keySprite[i], NULL);
	//}*/
	sfRenderWindow_drawSprite(_renderWindow, keySprite[0], NULL);
}

void CleanupKey(void)
{
}
