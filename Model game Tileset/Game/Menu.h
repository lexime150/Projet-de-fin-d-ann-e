#ifndef MENU_H
#define MENU_H

#include "Common.h"
#include "GameState.h"

void LoadMenu(void);
void PollEventMenu(sfRenderWindow* _renderWindow);
void UpdateMenu(float _dt);
void DrawMenu(sfRenderWindow* _renderWindow);
void CleanupMenu(void);

void KeyPressedMenu(sfRenderWindow* _renderWindow, sfKeyEvent _keyEvent);

#endif // !MENU_H
