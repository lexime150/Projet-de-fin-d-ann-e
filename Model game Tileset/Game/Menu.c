#include "Menu.h"

Menu menu;
void LoadText(void);
void DrawText(sfRenderWindow* _renderWindow);
void CleanupText(void);
void CheckMouseHoverMenu(sfRenderWindow* _renderWindow);
void CheckMouseClickMenu(sfRenderWindow* _renderWindow, sfMouseButtonEvent _mouseButtonEvent);
void LoadMenu(void)
{
	LoadText();

}

void PollEventMenu(sfRenderWindow* _renderWindow)
{
	sfEvent event;

	while (sfRenderWindow_pollEvent(_renderWindow, &event))
	{
		switch (event.type)
		{
		case sfEvtClosed:
			sfRenderWindow_close(_renderWindow);
			break;
		case sfEvtKeyPressed:
			KeyPressedMenu(_renderWindow, event.key);
			break;
		case sfEvtMouseButtonPressed:
			CheckMouseClickMenu(_renderWindow, event.mouseButton);
			break;
		default:
			break;
		}
	}
}

void KeyPressedMenu(sfRenderWindow* _renderWindow, sfKeyEvent _keyEvent)
{
	switch (_keyEvent.code)
	{
	case sfKeyEscape:
		sfRenderWindow_close(_renderWindow);
		break;
	case sfKeySpace:
		SetGameState(GAME);
		break;
	default:
		break;
	}
}

void UpdateMenu(sfRenderWindow* _renderWindow, float _dt)
{
	CheckMouseHoverMenu(_renderWindow);
}

void DrawMenu(sfRenderWindow* _renderWindow)
{
	DrawText(_renderWindow);
}

void CleanupMenu(void)
{
	sfText_destroy(menu.playText);
}

void LoadText(void)
{
	menu.playText = sfText_create();
	menu.font = sfFont_createFromFile("Assets/Fonts/Arcade.ttf");
	sfText_setFont(menu.playText, menu.font);
	sfText_setCharacterSize(menu.playText, 48);
	sfText_setOutlineColor(menu.playText, sfBlack);
	sfText_setOutlineThickness(menu.playText, 2);
	sfText_setString(menu.playText, "PLAY");
	menu.playTextBound = sfText_getGlobalBounds(menu.playText);
	sfText_setPosition(menu.playText, (sfVector2f) { (SCREEN_WIDTH - menu.playTextBound.width) / 2, (SCREEN_HEIGHT / 2 - menu.playTextBound.height) });

	menu.settingText = sfText_create();
	sfText_setFont(menu.settingText, menu.font);
	sfText_setCharacterSize(menu.settingText, 48);
	sfText_setOutlineColor(menu.settingText, sfBlack);
	sfText_setOutlineThickness(menu.settingText, 2);
	sfText_setString(menu.settingText, "SETTING");
	menu.settingTextBound = sfText_getGlobalBounds(menu.settingText);
	sfText_setPosition(menu.settingText, (sfVector2f) { (SCREEN_WIDTH - menu.settingTextBound.width) / 2, (SCREEN_HEIGHT / 2 - menu.settingTextBound.height) + 100 });

	menu.quitText = sfText_create();
	sfText_setFont(menu.quitText, menu.font);
	sfText_setCharacterSize(menu.quitText, 48);
	sfText_setOutlineColor(menu.quitText, sfBlack);
	sfText_setOutlineThickness(menu.quitText, 2);
	sfText_setString(menu.quitText, "QUIT");
	menu.quitTextBound = sfText_getGlobalBounds(menu.quitText);
	sfText_setPosition(menu.quitText, (sfVector2f) { (SCREEN_WIDTH - menu.quitTextBound.width) / 2, (SCREEN_HEIGHT / 2 - menu.quitTextBound.height) + 200 });


	menu.settingTextBound = sfText_getGlobalBounds(menu.settingText);
	menu.quitTextBound = sfText_getGlobalBounds(menu.quitText);
	menu.playTextBound = sfText_getGlobalBounds(menu.playText);

}

void DrawText(sfRenderWindow* _renderWindow)
{
	sfRenderWindow_drawText(_renderWindow, menu.playText, NULL);
	sfRenderWindow_drawText(_renderWindow, menu.settingText, NULL);
	sfRenderWindow_drawText(_renderWindow, menu.quitText, NULL);
}

void CleanupText(void)
{
	sfText_destroy(menu.playText);
	sfText_destroy(menu.settingText);
	sfText_destroy(menu.quitText);
}

void CheckMouseHoverMenu(sfRenderWindow* _renderWindow)
{
	sfVector2i mousePos = sfMouse_getPositionRenderWindow(_renderWindow);
	if (sfFloatRect_contains(&menu.playTextBound, (float)mousePos.x, (float)mousePos.y))
	{
		sfText_setFillColor(menu.playText, sfRed);
	}
	else
	{
		sfText_setFillColor(menu.playText, sfWhite);

	}

	if (sfFloatRect_contains(&menu.settingTextBound, (float)mousePos.x, (float)mousePos.y))
	{
		sfText_setFillColor(menu.settingText, sfRed);
	}
	else
	{
		sfText_setFillColor(menu.settingText, sfWhite);
	}

	if (sfFloatRect_contains(&menu.quitTextBound, (float)mousePos.x, (float)mousePos.y))
	{
		sfText_setFillColor(menu.quitText, sfRed);
	}
	else
	{
		sfText_setFillColor(menu.quitText, sfWhite);
	}
}

void CheckMouseClickMenu(sfRenderWindow* _renderWindow, sfMouseButtonEvent _mouseButtonEvent)
{
	sfVector2i mousePos = sfMouse_getPositionRenderWindow(_renderWindow);
	if (sfFloatRect_contains(&menu.playTextBound, (float)mousePos.x, (float)mousePos.y))
	{
		SetGameState(GAME);
	}
	else if (sfFloatRect_contains(&menu.settingTextBound, (float)mousePos.x, (float)mousePos.y))
	{
		printf("Setting clicked\n");
	}
	else if (sfFloatRect_contains(&menu.quitTextBound, (float)mousePos.x, (float)mousePos.y))
	{
		sfRenderWindow_close(_renderWindow);
	}
}
