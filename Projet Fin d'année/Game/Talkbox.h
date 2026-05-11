#ifndef TALKBOX_H
#define TALKBOX_H

#include "Common.h"
#include "Camera.h"

/* ── Dimensions & position (coordonnées écran) ───────────── */
#define TB_W            900.f
#define TB_H            180.f
#define TB_X            ((SCREEN_WIDTH  - TB_W) / 2.f)
#define TB_Y            (SCREEN_HEIGHT - TB_H - 24.f)
#define TB_PAD_X        24.f
#define TB_PAD_Y        18.f
#define TB_LINE_H       30.f
#define TB_FONT_SIZE    21
#define TB_MAX_LINES    5
#define TB_LINE_LEN     160

/* ── Typewriter ───────────────────────────────────────────── */
#define TB_CHAR_DELAY   0.04f
#define TB_BLINK_SPEED  0.5f

typedef struct Talkbox
{
    sfFont* font;

    const char* full_text;
    int          visible_chars;
    float        char_timer;
    sfBool       done;

    float        blink_timer;
    sfBool       cursor_visible;

    sfBool       is_open;
} Talkbox;

/* ── API publique ─────────────────────────────────────────── */
void LoadTalkbox(void);
void OpenTalkbox(const char* _text);
void UpdateTalkbox(float _dt);
void DrawTalkbox(sfRenderWindow* _rw);
void CleanupTalkbox(void);

sfBool IsTalkboxOpen(void);

#endif // !TALKBOX_H