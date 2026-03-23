#ifndef MENU_H
#define MENU_H

#include "Common.h"
#include "GameState.h"


typedef struct Menu
{
	sfText* playText;
	sfFont* font;
	sfText* quitText;
	sfText* settingText;
	char buffer[32];
	sfSprite* backgroundSprite;
	sfTexture* backgroundTexture;


	sfFloatRect playTextBound;
	sfFloatRect quitTextBound;
	sfFloatRect settingTextBound;
}Menu;
void LoadMenu(void);
void PollEventMenu(sfRenderWindow* _renderWindow);
void UpdateMenu(sfRenderWindow* _renderWindow, float _dt);
void DrawMenu(sfRenderWindow* _renderWindow);
void CleanupMenu(void);

void KeyPressedMenu(sfRenderWindow* _renderWindow, sfKeyEvent _keyEvent);

#endif // !MENU_H
