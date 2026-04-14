#include "Menu.h"

Menu menu;
PlayerSaveData playerSaveData;
PlayerSaveData* save = NULL;
sfBool saveExist = sfFalse;
void UpdateSlotText();

void InitButton(Button* _btn, sfFont* _font, const char* _str, float _y)
{



	_btn->text = sfText_create();
	sfText_setFont(_btn->text, _font);
	sfText_setCharacterSize(_btn->text, 48);
	sfText_setOutlineColor(_btn->text, sfBlack);
	sfText_setOutlineThickness(_btn->text, 2);
	sfText_setString(_btn->text, _str);

	_btn->bounds = sfText_getGlobalBounds(_btn->text);

	sfText_setPosition(_btn->text, (sfVector2f) { (SCREEN_WIDTH - _btn->bounds.width) / 2, _y });

	_btn->bounds = sfText_getGlobalBounds(_btn->text);
}

void InitMainButton(Button* _btn, const char* _texturePath, float _y)
{
	_btn->sprite = sfSprite_create();
	_btn->texture = sfTexture_createFromFile(_texturePath, NULL);
	sfSprite_setTexture(_btn->sprite, _btn->texture, sfTrue);
	_btn->scale = GAME_SCALE;
	sfSprite_setScale(_btn->sprite, (sfVector2f) { _btn->scale, _btn->scale });
	_btn->bounds = sfSprite_getGlobalBounds(_btn->sprite);
	sfSprite_setPosition(_btn->sprite, (sfVector2f) { (SCREEN_WIDTH - _btn->bounds.width) - 20, _y });
	_btn->bounds = sfSprite_getGlobalBounds(_btn->sprite);

	if (_texturePath == "Assets/Sprites/Menu/Title.png")
	{
		sfSprite_setPosition(_btn->sprite, (sfVector2f) { (SCREEN_WIDTH - _btn->bounds.width) / 2, _y });
	}

}


void LoadMenu(void)
{
	menu.state = MENU_MAIN;
	menu.hoverActive = sfFalse;
	menu.hoveredIndex = -1;

	menu.font = sfFont_createFromFile("Assets/Fonts/Arcade.ttf");

	InitMainButton(&menu.mainButtons[0], "Assets/Sprites/Menu/Play.png", (SCREEN_HEIGHT / 2) - 25);
	InitMainButton(&menu.mainButtons[1], "Assets/Sprites/Menu/Settings.png", SCREEN_HEIGHT / 2 + 132);
	InitMainButton(&menu.mainButtons[2], "Assets/Sprites/Menu/Quit.png", SCREEN_HEIGHT / 2 + 295);
	InitMainButton(&menu.mainButtons[3], "Assets/Sprites/Menu/Title.png", 20);


	InitButton(&menu.saveButtons[0], menu.font, "SAVE 1 (EMPTY)", SCREEN_HEIGHT / 2 - 100);
	InitButton(&menu.saveButtons[1], menu.font, "SAVE 2 (EMPTY)", SCREEN_HEIGHT / 2 + 000);
	InitButton(&menu.saveButtons[2], menu.font, "SAVE 3 (EMPTY)", SCREEN_HEIGHT / 2 + 100);
	InitButton(&menu.saveButtons[3], menu.font, "BACK", SCREEN_HEIGHT / 2 + 250);

	menu.hoverLeft = sfText_create();
	sfText_setFont(menu.hoverLeft, menu.font);
	sfText_setCharacterSize(menu.hoverLeft, 48);
	sfText_setString(menu.hoverLeft, "[");

	menu.hoverRight = sfText_create();
	sfText_setFont(menu.hoverRight, menu.font);
	sfText_setCharacterSize(menu.hoverRight, 48);
	sfText_setString(menu.hoverRight, "]");
}


void UpdateHover(sfRenderWindow* window, float _dt)
{
	sfVector2i mouse = sfMouse_getPositionRenderWindow(window);

	menu.hoverActive = sfFalse;
	menu.hoveredIndex = -1;

	Button* buttons = NULL;
	int count = 0;

	if (menu.state == MENU_PLAY)
	{
		buttons = menu.saveButtons;
		count = 4;
	}
	if (menu.state == MENU_MAIN)
	{
		buttons = menu.mainButtons;
		count = 3;
	}

	for (int i = 0; i < count; i++)
	{
		if (sfFloatRect_contains(&buttons[i].bounds, (float)mouse.x, (float)mouse.y))
		{
			menu.hoveredIndex = i;

			if (menu.state == MENU_PLAY)
			{

				sfVector2f pos = sfText_getPosition(buttons[i].text);

				sfText_setPosition(menu.hoverLeft, (sfVector2f) { pos.x - 40, pos.y });
				sfText_setPosition(menu.hoverRight, (sfVector2f) { pos.x + buttons[i].bounds.width + 10, pos.y });

			}
			float targetScale;
			float speed = 10.0f;

			for (int i = 0; i < count; i++)
			{
				sfBool isHovered = sfFloatRect_contains(&buttons[i].bounds, (float)mouse.x, (float)mouse.y);

				if (menu.state == MENU_MAIN)
				{
					targetScale = isHovered ? GAME_SCALE * 1.1f : GAME_SCALE;

					buttons[i].scale += (targetScale - buttons[i].scale) * speed * _dt;

					sfSprite_setScale(buttons[i].sprite,(sfVector2f) {buttons[i].scale, buttons[i].scale});
				}

				if (isHovered)
				{
					menu.hoveredIndex = i;
					menu.hoverActive = sfTrue;
				}
			}
		}

	}
}


void HandleClick(sfRenderWindow* window)
{
	if (!menu.hoverActive)
	{
		return;
	}

	if (menu.state == MENU_MAIN)
	{
		switch (menu.hoveredIndex)
		{
		case 0: // PLAY




			menu.state = MENU_PLAY;

			UpdateSlotText(window);





			break;

		case 1: // SETTING
			printf("Settings\n");


			break;

		case 2: // QUIT
			sfRenderWindow_close(window);


			break;
		}
	}
	else if (menu.state == MENU_PLAY)
	{
		switch (menu.hoveredIndex)
		{
		case 0:
		case 1:
		case 2:
			playerSaveData.save = menu.hoveredIndex + 1;
			SetGameState(GAME);
			break;

		case 3: // BACK
			menu.state = MENU_MAIN;
			break;
		default:
			break;
		}

	}
}


void PollEventMenu(sfRenderWindow* window)
{
	sfEvent event;

	while (sfRenderWindow_pollEvent(window, &event))
	{
		switch (event.type)
		{
		case sfEvtClosed:
			sfRenderWindow_close(window);
			break;

		case sfEvtMouseButtonPressed:
			HandleClick(window);
			break;

		case sfEvtKeyPressed:
			if (event.key.code == sfKeyEscape)
				sfRenderWindow_close(window);
			break;

		default:
			break;
		}
	}
}


void UpdateMenu(sfRenderWindow* window, float _dt)
{
	UpdateHover(window, _dt);
	for (int i = 0; i < 3; i++)
	{
		menu.saveButtons[i].bounds = sfText_getGlobalBounds(menu.saveButtons[i].text);
	}
}


void DrawMenu(sfRenderWindow* window)
{
	sfRenderWindow_setView(window, sfRenderWindow_getDefaultView(window));

	Button* buttons = NULL;
	int count = 0;

	if (menu.state == MENU_MAIN)
	{
		buttons = menu.mainButtons;
		count = 4;

		for (int i = 0; i < count; i++)
		{
			sfRenderWindow_drawSprite(window, buttons[i].sprite, NULL);
		}
	}
	else if (menu.state == MENU_PLAY)
	{
		buttons = menu.saveButtons;
		count = 4;
		for (int i = 0; i < count; i++)
		{
			sfRenderWindow_drawText(window, buttons[i].text, NULL);
		}

		if (menu.hoverActive)
		{
			sfRenderWindow_drawText(window, menu.hoverLeft, NULL);
			sfRenderWindow_drawText(window, menu.hoverRight, NULL);
		}

	}
}


void CleanupMenu(void)
{

	for (int i = 0; i < 3; i++)
	{
		sfText_destroy(menu.saveButtons[i].text);
		sfSprite_destroy(menu.mainButtons[i].sprite);
		sfTexture_destroy(menu.mainButtons[i].texture);
	}

	sfText_destroy(menu.hoverLeft);
	sfText_destroy(menu.hoverRight);
	sfFont_destroy(menu.font);
}

void UpdateSlotText()
{
	for (int i = 0; i < 3; i++)
	{
		menu.saveButtons[i].bounds = sfText_getGlobalBounds(menu.saveButtons[i].text);
		if (SaveExists(i + 1))
		{
			save = LoadSave(i + 1);

			snprintf(menu.buffer, sizeof(menu.buffer), "SAVE %d (%s)", i + 1, save->level);

			sfText_setString(menu.saveButtons[i].text, menu.buffer);
		}
		else
		{
			snprintf(menu.buffer, sizeof(menu.buffer), "SAVE %d (EMPTY)", i + 1);

			sfText_setString(menu.saveButtons[i].text, menu.buffer);
		}
	}
}
