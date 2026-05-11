#include "Talkbox.h"

extern Camera camera;   /* défini dans Camera.c */

static Talkbox  s_tb;
static sfView* s_uiView = NULL;

/* ════════════════════════════════════════════════════════════
   Utilitaires internes
   ════════════════════════════════════════════════════════════ */

   /* Découpe le texte visible en lignes qui tiennent dans TB_W - 2*TB_PAD_X */
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

/* Passe en vue écran fixe pour dessiner l'UI */
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

/* ════════════════════════════════════════════════════════════
   API publique
   ════════════════════════════════════════════════════════════ */

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

    /* Curseur clignotant */
    s_tb.blink_timer += _dt;
    if (s_tb.blink_timer >= TB_BLINK_SPEED)
    {
        s_tb.blink_timer -= TB_BLINK_SPEED;
        s_tb.cursor_visible = !s_tb.cursor_visible;
    }

    /* Fermeture sur E quand le texte est entier */
    if (s_tb.done && sfKeyboard_isKeyPressed(sfKeyE))
        s_tb.is_open = sfFalse;
}

void DrawTalkbox(sfRenderWindow* _rw)
{
    if (!s_tb.is_open || !s_tb.font) return;

    BeginUI(_rw);

    /* ── Ombre portée (décalée de 6px) ── */
    sfRectangleShape* shadow = sfRectangleShape_create();
    sfRectangleShape_setSize(shadow, (sfVector2f) { TB_W, TB_H });
    sfRectangleShape_setPosition(shadow, (sfVector2f) { TB_X + 6.f, TB_Y + 6.f });
    sfRectangleShape_setFillColor(shadow, sfColor_fromRGBA(0, 0, 0, 180));
    sfRenderWindow_drawRectangleShape(_rw, shadow, NULL);
    sfRectangleShape_destroy(shadow);

    /* ── Fond blanc cassé ── */
    sfRectangleShape* bg = sfRectangleShape_create();
    sfRectangleShape_setSize(bg, (sfVector2f) { TB_W, TB_H });
    sfRectangleShape_setPosition(bg, (sfVector2f) { TB_X, TB_Y });
    sfRectangleShape_setFillColor(bg, sfColor_fromRGB(245, 242, 235));
    sfRectangleShape_setOutlineColor(bg, sfColor_fromRGB(10, 10, 10));
    sfRectangleShape_setOutlineThickness(bg, 3.f);
    sfRenderWindow_drawRectangleShape(_rw, bg, NULL);
    sfRectangleShape_destroy(bg);

    /* ── Double bordure intérieure ── */
    sfRectangleShape* inner = sfRectangleShape_create();
    sfRectangleShape_setSize(inner, (sfVector2f) { TB_W - 14.f, TB_H - 14.f });
    sfRectangleShape_setPosition(inner, (sfVector2f) { TB_X + 7.f, TB_Y + 7.f });
    sfRectangleShape_setFillColor(inner, sfColor_fromRGBA(0, 0, 0, 0));
    sfRectangleShape_setOutlineColor(inner, sfColor_fromRGB(10, 10, 10));
    sfRectangleShape_setOutlineThickness(inner, 1.f);
    sfRenderWindow_drawRectangleShape(_rw, inner, NULL);
    sfRectangleShape_destroy(inner);

    /* ── Bande noire haute (titre/déco) ── */
    sfRectangleShape* band = sfRectangleShape_create();
    sfRectangleShape_setSize(band, (sfVector2f) { TB_W, 28.f });
    sfRectangleShape_setPosition(band, (sfVector2f) { TB_X, TB_Y });
    sfRectangleShape_setFillColor(band, sfColor_fromRGB(10, 10, 10));
    sfRenderWindow_drawRectangleShape(_rw, band, NULL);
    sfRectangleShape_destroy(band);

    /* ── Label dans la bande (nom du PNJ) ── */
    sfText* label = sfText_create();
    sfText_setFont(label, s_tb.font);
    sfText_setCharacterSize(label, 14);
    sfText_setString(label, "Ange de Pierre");
    sfText_setFillColor(label, sfColor_fromRGB(245, 242, 235));
    sfText_setStyle(label, sfTextBold);
    sfText_setPosition(label, (sfVector2f) { TB_X + TB_PAD_X, TB_Y + 5.f });
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
        sfText_setFillColor(txt, sfColor_fromRGB(15, 15, 15));
        sfText_setPosition(txt, (sfVector2f) { TB_X + TB_PAD_X, y });
        sfRenderWindow_drawText(_rw, txt, NULL);
        sfText_destroy(txt);
    }

    /* ── Curseur clignotant (trait vertical noir) ── */
    if (!s_tb.done || s_tb.cursor_visible)
    {
        int   last = (n > 0) ? n - 1 : 0;
        float cy = text_top + (last - first_line) * TB_LINE_H;
        float cx = TB_X + TB_PAD_X;

        if (n > 0)
        {
            sfText* tmp = sfText_create();
            sfText_setFont(tmp, s_tb.font);
            sfText_setCharacterSize(tmp, TB_FONT_SIZE);
            sfText_setString(tmp, lines[last]);
            cx += sfText_getLocalBounds(tmp).width + 2.f;
            sfText_destroy(tmp);
            if (cx > TB_X + TB_W - TB_PAD_X - 6.f)
                cx = TB_X + TB_W - TB_PAD_X - 6.f;
        }

        sfRectangleShape* cursor = sfRectangleShape_create();
        sfRectangleShape_setSize(cursor, (sfVector2f) { 2.f, (float)TB_FONT_SIZE });
        sfRectangleShape_setPosition(cursor, (sfVector2f) { cx, cy + 2.f });
        sfRectangleShape_setFillColor(cursor, sfColor_fromRGB(15, 15, 15));
        sfRenderWindow_drawRectangleShape(_rw, cursor, NULL);
        sfRectangleShape_destroy(cursor);
    }

    /* ── Indicateur "Appuie sur E" quand le texte est fini ── */
    if (s_tb.done)
    {
        sfText* hint = sfText_create();
        sfText_setFont(hint, s_tb.font);
        sfText_setCharacterSize(hint, 13);
        sfText_setString(hint, "[ E ] Continuer");
        sfText_setFillColor(hint, sfColor_fromRGB(80, 80, 80));
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