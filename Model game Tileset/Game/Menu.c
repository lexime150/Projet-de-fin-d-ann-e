#include "Menu.h"

Menu menu;
PlayerSaveData playerSaveData;
PlayerSaveData* save = NULL;
sfBool saveExist = sfFalse;
void UpdateSlotText();

void InitButton(Button* btn, sfFont* font, const char* str, float y)
{



	btn->text = sfText_create();
	sfText_setFont(btn->text, font);
	sfText_setCharacterSize(btn->text, 48);
	sfText_setOutlineColor(btn->text, sfBlack);
	sfText_setOutlineThickness(btn->text, 2);
	sfText_setString(btn->text, str);

	btn->bounds = sfText_getGlobalBounds(btn->text);

	sfText_setPosition(btn->text, (sfVector2f) { (SCREEN_WIDTH - btn->bounds.width) / 2, y });

	btn->bounds = sfText_getGlobalBounds(btn->text);
}


void LoadMenu(void)
{
	menu.state = MENU_MAIN;
	menu.hoverActive = sfFalse;
	menu.hoveredIndex = -1;

	menu.font = sfFont_createFromFile("Assets/Fonts/Arcade.ttf");

	InitButton(&menu.mainButtons[0], menu.font, "PLAY", SCREEN_HEIGHT / 2);
	InitButton(&menu.mainButtons[1], menu.font, "SETTING", SCREEN_HEIGHT / 2 + 100);
	InitButton(&menu.mainButtons[2], menu.font, "QUIT", SCREEN_HEIGHT / 2 + 200);

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


void UpdateHover(sfRenderWindow* window)
{
	sfVector2i mouse = sfMouse_getPositionRenderWindow(window);

	menu.hoverActive = sfFalse;
	menu.hoveredIndex = -1;

	Button* buttons = NULL;
	int count = 0;

	if (menu.state == MENU_MAIN)
	{
		buttons = menu.mainButtons;
		count = 3;
	}
	else if (menu.state == MENU_PLAY)
	{
		buttons = menu.saveButtons;
		count = 4;
	}

	for (int i = 0; i < count; i++)
	{
		if (sfFloatRect_contains(&buttons[i].bounds, mouse.x, mouse.y))
		{
			menu.hoverActive = sfTrue;
			menu.hoveredIndex = i;

			sfVector2f pos = sfText_getPosition(buttons[i].text);

			sfText_setPosition(menu.hoverLeft, (sfVector2f) { pos.x - 40, pos.y });
			sfText_setPosition(menu.hoverRight, (sfVector2f) { pos.x + buttons[i].bounds.width + 10, pos.y });

			break;
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


void UpdateMenu(sfRenderWindow* window, float dt)
{
	UpdateHover(window);
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
		count = 3;
	}
	else if (menu.state == MENU_PLAY)
	{
		buttons = menu.saveButtons;
		count = 4;
	}

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

void CleanupMenu(void)
{
	for (int i = 0; i < 3; i++)
	{
		sfText_destroy(menu.mainButtons[i].text);
		sfText_destroy(menu.saveButtons[i].text);
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

			snprintf(menu.buffer, sizeof(menu.buffer),"SAVE %d (%s)", i + 1, save->level);

			sfText_setString(menu.saveButtons[i].text, menu.buffer);
		}
		else
		{
			snprintf(menu.buffer, sizeof(menu.buffer),"SAVE %d (EMPTY)", i + 1);

			sfText_setString(menu.saveButtons[i].text, menu.buffer);
		}
	}
}
