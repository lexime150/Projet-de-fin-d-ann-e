#include "Talkbox.h"

extern Camera camera;   /* défini dans Camera.c */

static Talkbox  s_tb;
static sfView* s_uiView = NULL;


static int WrapText(const char* _text,
    char out[TB_MAX_LINES][TB_LINE_LEN])
{
    float max_w = TB_W - TB_PAD_X * 2.f;
    int   n = 0;
    int   i = 0;
    int   len = (int)strlen(_text);

    while (i < len && n < TB_MAX_LINES)
    {
        int line_start = i;
        int last_space = -1;

        while (i < len && _text[i] != '\n')
        {
            int  seg_len = i - line_start + 1;
            char seg[TB_LINE_LEN];
            if (seg_len >= TB_LINE_LEN) seg_len = TB_LINE_LEN - 1;
            strncpy_s(seg, sizeof(seg), _text + line_start, seg_len);
            seg[seg_len] = '\0';

            sfText* tmp = sfText_create();
            sfText_setFont(tmp, s_tb.font);
            sfText_setCharacterSize(tmp, TB_FONT_SIZE);
            sfText_setString(tmp, seg);
            float w = sfText_getLocalBounds(tmp).width;
            sfText_destroy(tmp);

            if (w > max_w && (i - line_start) > 0)
            {
                int cut = (last_space >= 0) ? (last_space - line_start)
                    : (i - line_start);
                if (cut >= TB_LINE_LEN) cut = TB_LINE_LEN - 1;
                strncpy_s(out[n], TB_LINE_LEN, _text + line_start, cut);
                out[n][cut] = '\0';
                n++;
                i = (last_space >= 0) ? (last_space + 1) : i;
                line_start = i;
                last_space = -1;
                continue;
            }
            if (_text[i] == ' ') last_space = i;
            i++;
        }

        int cut = i - line_start;
        if (cut > 0 && n < TB_MAX_LINES)
        {
            if (cut >= TB_LINE_LEN) cut = TB_LINE_LEN - 1;
            strncpy_s(out[n], TB_LINE_LEN, _text + line_start, cut);
            out[n][cut] = '\0';
            n++;
        }
        if (i < len && _text[i] == '\n') i++;
    }
    return n;
}


static void BeginUI(sfRenderWindow* _rw)
{
    if (!s_uiView)
    {
        s_uiView = sfView_create();
        sfView_setCenter(s_uiView,
            (sfVector2f) {
            SCREEN_WIDTH / 2.f, SCREEN_HEIGHT / 2.f
        });
        sfView_setSize(s_uiView,
            (sfVector2f) {
            SCREEN_WIDTH, SCREEN_HEIGHT
        });
    }
    sfRenderWindow_setView(_rw, s_uiView);
}

/* Restaure la vue caméra */
static void EndUI(sfRenderWindow* _rw)
{
    sfRenderWindow_setView(_rw, camera.cameraView);
}



void LoadTalkbox(void)
{
    s_tb.font = sfFont_createFromFile("Assets/Fonts/Arial.ttf");
    if (!s_tb.font)
        s_tb.font = sfFont_createFromFile("C:/Windows/Fonts/arial.ttf");

    s_tb.is_open = sfFalse;
    s_tb.done = sfFalse;
    s_tb.visible_chars = 0;
    s_tb.char_timer = 0.f;
    s_tb.blink_timer = 0.f;
    s_tb.cursor_visible = sfTrue;
    s_tb.full_text = "";
}

void OpenTalkbox(const char* _text)
{
    s_tb.full_text = _text;
    s_tb.visible_chars = 0;
    s_tb.char_timer = 0.f;
    s_tb.done = sfFalse;
    s_tb.blink_timer = 0.f;
    s_tb.cursor_visible = sfTrue;
    s_tb.is_open = sfTrue;
}

sfBool IsTalkboxOpen(void)
{
    return s_tb.is_open;
}

void UpdateTalkbox(float _dt)
{
    if (!s_tb.is_open) return;

    /* Typewriter */
    if (!s_tb.done)
    {
        s_tb.char_timer += _dt;
        while (s_tb.char_timer >= TB_CHAR_DELAY)
        {
            s_tb.char_timer -= TB_CHAR_DELAY;
            if (s_tb.visible_chars < (int)strlen(s_tb.full_text))
                s_tb.visible_chars++;
            else
                s_tb.done = sfTrue;
        }
    }

    /* Fermeture sur E quand le texte est entier */
    if (s_tb.done && sfKeyboard_isKeyPressed(sfKeyE))
        s_tb.is_open = sfFalse;
}

void DrawTalkbox(sfRenderWindow* _rw)
{
    if (!s_tb.is_open || !s_tb.font) return;

    BeginUI(_rw);

    sfRectangleShape* bg = sfRectangleShape_create();
    sfRectangleShape_setSize(bg, (sfVector2f) { TB_W, TB_H });
    sfRectangleShape_setPosition(bg, (sfVector2f) { TB_X, TB_Y });
    sfRectangleShape_setFillColor(bg, sfColor_fromRGB(0, 0, 0));
    sfRectangleShape_setOutlineColor(bg, sfColor_fromRGB(255, 255, 255));
    sfRectangleShape_setOutlineThickness(bg, 3.f);
    sfRenderWindow_drawRectangleShape(_rw, bg, NULL);
    sfRectangleShape_destroy(bg);

    sfRectangleShape* inner = sfRectangleShape_create();
    sfRectangleShape_setSize(inner, (sfVector2f) { TB_W - 14.f, TB_H - 14.f });
    sfRectangleShape_setPosition(inner, (sfVector2f) { TB_X + 7.f, TB_Y + 7.f });
    sfRectangleShape_setFillColor(inner, sfColor_fromRGBA(0, 0, 0, 0));
    sfRectangleShape_setOutlineColor(inner, sfColor_fromRGB(255, 255, 255));
    sfRectangleShape_setOutlineThickness(inner, 1.f);
    sfRenderWindow_drawRectangleShape(_rw, inner, NULL);
    sfRectangleShape_destroy(inner);

    sfRectangleShape* band = sfRectangleShape_create();
    sfRectangleShape_setSize(band, (sfVector2f) { TB_W - 14.f, 28.f });
    sfRectangleShape_setPosition(band, (sfVector2f) { TB_X + 7.f, TB_Y + 7.f });
    sfRectangleShape_setFillColor(band, sfColor_fromRGB(255, 255, 255));
    sfRenderWindow_drawRectangleShape(_rw, band, NULL);
    sfRectangleShape_destroy(band);

    sfText* label = sfText_create();
    sfText_setFont(label, s_tb.font);
    sfText_setCharacterSize(label, 14);
    sfText_setString(label, "Ange de Pierre");
    sfText_setFillColor(label, sfColor_fromRGB(0, 0, 0));
    sfText_setStyle(label, sfTextBold);
    sfText_setPosition(label, (sfVector2f) { TB_X + TB_PAD_X, TB_Y + 11.f });
    sfRenderWindow_drawText(_rw, label, NULL);
    sfText_destroy(label);

    /* ── Texte visible ── */
    char visible[2048];
    int  vlen = s_tb.visible_chars;
    int  tlen = (int)strlen(s_tb.full_text);
    if (vlen > tlen) vlen = tlen;
    strncpy_s(visible, sizeof(visible), s_tb.full_text, vlen);
    visible[vlen] = '\0';

    char lines[TB_MAX_LINES][TB_LINE_LEN];
    int  n = WrapText(visible, lines);

    int   max_lines = (int)((TB_H - 28.f - TB_PAD_Y * 2.f) / TB_LINE_H);
    int   first_line = (n > max_lines) ? (n - max_lines) : 0;
    float text_top = TB_Y + 28.f + TB_PAD_Y;   /* sous la bande */

    for (int l = first_line; l < n; l++)
    {
        float y = text_top + (l - first_line) * TB_LINE_H;
        if (y + TB_FONT_SIZE > TB_Y + TB_H - TB_PAD_Y) break;

        sfText* txt = sfText_create();
        sfText_setFont(txt, s_tb.font);
        sfText_setCharacterSize(txt, TB_FONT_SIZE);
        sfText_setString(txt, lines[l]);
        sfText_setFillColor(txt, sfColor_fromRGB(255, 255, 255));
        sfText_setPosition(txt, (sfVector2f) { TB_X + TB_PAD_X, y });
        sfRenderWindow_drawText(_rw, txt, NULL);
        sfText_destroy(txt);
    }

    /* ── Indicateur "Appuie sur E" quand le texte est fini ── */
    if (s_tb.done)
    {
        sfText* hint = sfText_create();
        sfText_setFont(hint, s_tb.font);
        sfText_setCharacterSize(hint, 13);
        sfText_setString(hint, "[ E ] Continuer");
        sfText_setFillColor(hint, sfColor_fromRGB(200, 200, 200));
        sfText_setStyle(hint, sfTextItalic);

        sfFloatRect hb = sfText_getLocalBounds(hint);
        sfText_setPosition(hint,
            (sfVector2f) {
            TB_X + TB_W - hb.width - TB_PAD_X,
                TB_Y + TB_H - hb.height - TB_PAD_Y
        });
        sfRenderWindow_drawText(_rw, hint, NULL);
        sfText_destroy(hint);
    }

    EndUI(_rw);
}

void CleanupTalkbox(void)
{
    if (s_tb.font)
    {
        sfFont_destroy(s_tb.font);
        s_tb.font = NULL;
    }
    if (s_uiView)
    {
        sfView_destroy(s_uiView);
        s_uiView = NULL;
    }
}