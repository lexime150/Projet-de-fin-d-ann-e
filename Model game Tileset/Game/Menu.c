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
	sfRenderWindow_setView(_renderWindow, sfRenderWindow_getDefaultView(_renderWindow));
	DrawText(_renderWindow);
}

void CleanupMenu(void)
{
	sfText_destroy(menu.playText);
}

void LoadText(void)
{
	menu.hoverSelected = sfFalse;

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

	menu.hoverSelectionLeftText = sfText_create();
	sfText_setFont(menu.hoverSelectionLeftText, menu.font);
	sfText_setCharacterSize(menu.hoverSelectionLeftText, 48);
	sfText_setOutlineColor(menu.hoverSelectionLeftText, sfBlack);
	sfText_setOutlineThickness(menu.hoverSelectionLeftText, 2);
	sfText_setString(menu.hoverSelectionLeftText, "[");

	menu.hoverSelectionRightText = sfText_create();
	sfText_setFont(menu.hoverSelectionRightText, menu.font);
	sfText_setCharacterSize(menu.hoverSelectionRightText, 48);
	sfText_setOutlineColor(menu.hoverSelectionRightText, sfBlack);
	sfText_setOutlineThickness(menu.hoverSelectionRightText, 2);
	sfText_setString(menu.hoverSelectionRightText, "]");

	menu.settingTextBound = sfText_getGlobalBounds(menu.settingText);
	menu.quitTextBound = sfText_getGlobalBounds(menu.quitText);
	menu.playTextBound = sfText_getGlobalBounds(menu.playText);
	menu.hoverSelectionLeftTextBound = sfText_getGlobalBounds(menu.hoverSelectionLeftText);
	menu.hoverSelectionRightTextBound = sfText_getGlobalBounds(menu.hoverSelectionRightText);

}

void DrawText(sfRenderWindow* _renderWindow)
{
	sfRenderWindow_drawText(_renderWindow, menu.playText, NULL);
	sfRenderWindow_drawText(_renderWindow, menu.settingText, NULL);
	sfRenderWindow_drawText(_renderWindow, menu.quitText, NULL);
	if (menu.hoverSelected)
	{
		sfRenderWindow_drawText(_renderWindow, menu.hoverSelectionLeftText, NULL);
		sfRenderWindow_drawText(_renderWindow, menu.hoverSelectionRightText, NULL);

	}
}

void CleanupText(void)
{
	sfText_destroy(menu.playText);
	sfText_destroy(menu.settingText);
	sfText_destroy(menu.quitText);
	sfText_destroy(menu.hoverSelectionLeftText);
	sfText_destroy(menu.hoverSelectionRightText);
}

void CheckMouseHoverMenu(sfRenderWindow* _renderWindow)
{
	menu.hoverSelected = sfFalse;
	sfVector2i mousePos = sfMouse_getPositionRenderWindow(_renderWindow);
	if (sfFloatRect_contains(&menu.playTextBound, (float)mousePos.x, (float)mousePos.y))
	{
		menu.hoverSelected = sfTrue;

		sfText_setPosition(menu.hoverSelectionLeftText, (sfVector2f) { menu.playTextBound.left - 30, menu.playTextBound.top - menu.hoverSelectionLeftTextBound.height / 2 + 10 });
		sfText_setPosition(menu.hoverSelectionRightText, (sfVector2f) { menu.playTextBound.left + menu.playTextBound.width + 10, menu.playTextBound.top - menu.hoverSelectionRightTextBound.height / 2 + 10 });
	}
	else if (sfFloatRect_contains(&menu.settingTextBound, (float)mousePos.x, (float)mousePos.y))
	{
		menu.hoverSelected = sfTrue;
		sfText_setPosition(menu.hoverSelectionLeftText, (sfVector2f) { menu.settingTextBound.left - 30, menu.settingTextBound.top - menu.hoverSelectionLeftTextBound.height / 2 + 10 });
		sfText_setPosition(menu.hoverSelectionRightText, (sfVector2f) { menu.settingTextBound.left + menu.settingTextBound.width + 10, menu.settingTextBound.top - menu.hoverSelectionRightTextBound.height / 2 + 10 });

	}
	else if (sfFloatRect_contains(&menu.quitTextBound, (float)mousePos.x, (float)mousePos.y))
	{
		menu.hoverSelected = sfTrue;
		sfText_setPosition(menu.hoverSelectionLeftText, (sfVector2f) { menu.quitTextBound.left - 30, menu.quitTextBound.top - menu.hoverSelectionLeftTextBound.height / 2 + 10 });
		sfText_setPosition(menu.hoverSelectionRightText, (sfVector2f) { menu.quitTextBound.left + menu.quitTextBound.width + 10, menu.quitTextBound.top - menu.hoverSelectionRightTextBound.height / 2 + 10 });
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
