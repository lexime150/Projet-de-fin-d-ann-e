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


void LoadMenu(void)
{
	menu.state = MENU_MAIN;
	menu.hoverActive = sfFalse;
	menu.hoveredIndex = -1;

	menu.font = sfFont_createFromFile("Assets/Fonts/Arcade.ttf");

	menu.backgroundSprite = sfSprite_create();
	menu.backgroundTexture = sfTexture_createFromFile("Assets/Sprites/Menu/newBackground.png", NULL);
	sfSprite_setTexture(menu.backgroundSprite, menu.backgroundTexture, sfTrue);
	sfSprite_setScale(menu.backgroundSprite, (sfVector2f) { GAME_SCALE * 1.41f, GAME_SCALE * 1.3f });
	sfSprite_setPosition(menu.backgroundSprite, (sfVector2f) { 0, 0 });

	menu.mainButtons[0].sprite = sfSprite_create();
	menu.mainButtons[0].texture = sfTexture_createFromFile("Assets/Sprites/Menu/Buttons.png", NULL);
	sfSprite_setTexture(menu.mainButtons[0].sprite, menu.mainButtons[0].texture, sfTrue);
	menu.mainButtons[0].scale = GAME_SCALE;
	sfSprite_setScale(menu.mainButtons[0].sprite, (sfVector2f) { GAME_SCALE, GAME_SCALE });
	menu.mainButtons[0].bounds = sfSprite_getGlobalBounds(menu.mainButtons[0].sprite);
	sfSprite_setPosition(menu.mainButtons[0].sprite, (sfVector2f) { 435, 80 });
	menu.mainButtons[0].bounds = sfSprite_getGlobalBounds(menu.mainButtons[0].sprite);

	sfVector2f spritePos = sfSprite_getPosition(menu.mainButtons[0].sprite);

	menu.fightBounds = (sfFloatRect){ spritePos.x + 240 * GAME_SCALE, spritePos.y + 71 * GAME_SCALE, 81 * GAME_SCALE, 42 * GAME_SCALE };
	menu.settingsBounds = (sfFloatRect){ spritePos.x + 230 * GAME_SCALE, spritePos.y + 114 * GAME_SCALE, 89 * GAME_SCALE, 35 * GAME_SCALE };
	menu.exitBounds = (sfFloatRect){ spritePos.x + 219 * GAME_SCALE, spritePos.y + 150 * GAME_SCALE, 96 * GAME_SCALE, 31 * GAME_SCALE };

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

	if (menu.state == MENU_PLAY)
	{
		for (int i = 0; i < 4; i++)
		{
			if (sfFloatRect_contains(&menu.saveButtons[i].bounds, (float)mouse.x, (float)mouse.y))
			{
				menu.hoveredIndex = i;
				menu.hoverActive = sfTrue;

				sfVector2f pos = sfText_getPosition(menu.saveButtons[i].text);
				sfText_setPosition(menu.hoverLeft, (sfVector2f) { pos.x - 40, pos.y });
				sfText_setPosition(menu.hoverRight, (sfVector2f) { pos.x + menu.saveButtons[i].bounds.width + 10, pos.y });
			}
		}
	}
}


void HandleClick(sfRenderWindow* window)
{
	sfVector2i mouse = sfMouse_getPositionRenderWindow(window);

	if (menu.state == MENU_MAIN)
	{
		if (sfFloatRect_contains(&menu.fightBounds, (float)mouse.x, (float)mouse.y))
		{
			menu.state = MENU_PLAY;
			UpdateSlotText();
		}
		else if (sfFloatRect_contains(&menu.settingsBounds, (float)mouse.x, (float)mouse.y))
		{
			printf("Settings\n");
		}
		else if (sfFloatRect_contains(&menu.exitBounds, (float)mouse.x, (float)mouse.y))
		{
			sfRenderWindow_close(window);
		}
		return;
	}

	if (menu.state == MENU_PLAY)
	{
		if (!menu.hoverActive)
			return;

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

	if (menu.state == MENU_MAIN)
	{
		sfRenderWindow_drawSprite(window, menu.backgroundSprite, NULL);
		sfRenderWindow_drawSprite(window, menu.mainButtons[0].sprite, NULL);
	}
	else if (menu.state == MENU_PLAY)
	{
		for (int i = 0; i < 4; i++)
		{
			sfRenderWindow_drawText(window, menu.saveButtons[i].text, NULL);
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
	sfSprite_destroy(menu.mainButtons[0].sprite);
	sfTexture_destroy(menu.mainButtons[0].texture);

	for (int i = 0; i < 4; i++)
	{
		sfText_destroy(menu.saveButtons[i].text);
	}

	sfText_destroy(menu.hoverLeft);
	sfText_destroy(menu.hoverRight);
	sfFont_destroy(menu.font);
	sfSprite_destroy(menu.backgroundSprite);
	sfTexture_destroy(menu.backgroundTexture);
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