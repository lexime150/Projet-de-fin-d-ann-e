#ifndef MENU_H
#define MENU_H

#include "Common.h"
#include "GameState.h"


typedef struct
{
    sfText* text;
    sfFloatRect bounds;
} Button;


typedef enum
{
    MENU_MAIN,
    MENU_PLAY,
    MENU_SETTINGS
} MenuState;


typedef struct Menu
{
    sfSprite* backgroundSprite;
    sfTexture* backgroundTexture;

    // Boutons
    Button mainButtons[3]; // PLAY, SETTING, QUIT
    Button saveButtons[4]; // SAVE 1,2,3, BACK

    sfFont* font;

    sfText* hoverLeft;
    sfText* hoverRight;

    sfBool hoverActive;
    int hoveredIndex;

    MenuState state;
    char buffer[20];
} Menu;


void LoadMenu(void);
void PollEventMenu(sfRenderWindow* _renderWindow);
void UpdateMenu(sfRenderWindow* _renderWindow, float _dt);
void DrawMenu(sfRenderWindow* _renderWindow);
void CleanupMenu(void);

#endif // MENU_H

