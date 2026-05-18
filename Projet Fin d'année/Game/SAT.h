#ifndef SAT_H
#define SAT_H

#include "Common.h"

/* ─────────────────────────────────────────────
   Nombre maximal de sommets par polygone.
   Les polygones Tiled ont rarement plus de 32 sommets.
───────────────────────────────────────────── */
#define SAT_MAX_VERTS 32

/* ─────────────────────────────────────────────
   Un polygone convexe en coordonnées monde.
   On stocke les sommets transformés (x*GAME_SCALE, y*GAME_SCALE).
───────────────────────────────────────────── */
typedef struct SATPolygon
{
    sfVector2f verts[SAT_MAX_VERTS];
    int        count;           /* nombre de sommets réels */
} SATPolygon;

/* ─────────────────────────────────────────────
   Résultat d'un test SAT.
   Si colliding == sfTrue :
     - normal  = axe de séparation minimum (MTV normal), normalisé,
                 pointant de B vers A
     - depth   = profondeur de pénétration (pixels)
     - contactX/Y = point de contact approximatif
───────────────────────────────────────────── */
typedef struct SATResult
{
    sfBool     colliding;
    sfVector2f normal;      /* MTV normal, de B vers A             */
    float      depth;       /* profondeur de pénétration           */
    float      contactX;    /* point de contact (centre approché)  */
    float      contactY;
} SATResult;

/* ── Construction de polygones ────────────────────────────────────── */

/* Construit un SATPolygon depuis un objet Tiled (polyline ou polygon).
   _ox/_oy = coordonnées monde de l'objet (object->x * GAME_SCALE, ...) */
SATPolygon SAT_PolyFromTiled(const float* _verts, int _vertCount,
    float _ox, float _oy);

/* Construit un SATPolygon rectangle depuis une sfFloatRect             */
SATPolygon SAT_PolyFromRect(sfFloatRect _rect);

/* Construit un SATPolygon rectangle depuis une position + taille       */
SATPolygon SAT_PolyFromBox(float _x, float _y, float _w, float _h);

/* Translate un polygone (utile pour mettre à jour la position)         */
SATPolygon SAT_Translate(SATPolygon _poly, float _dx, float _dy);

/* ── Test de collision ────────────────────────────────────────────── */

/* Test SAT entre deux polygones convexes quelconques.
   Retourne un SATResult (colliding, normal MTV de B→A, depth).        */
SATResult SAT_Test(const SATPolygon* _a, const SATPolygon* _b);

/* Commodité : test SAT entre un polygone et un AABB (sfFloatRect).    */
SATResult SAT_TestRect(const SATPolygon* _poly, sfFloatRect _rect);

/* ── Résolution de collision ──────────────────────────────────────── */

/* Applique la résolution minimale (MTV) sur une position.
   pos = position à corriger.
   res = résultat SAT déjà calculé (doit être colliding == sfTrue).
   Si onGround != NULL, *onGround = sfTrue si la normale pointe vers le haut. */
void SAT_Resolve(sfVector2f* _pos, const SATResult* _res, sfBool* _onGround);

/* ── Debug / dessin ──────────────────────────────────────────────── */

/* Dessine le contour d'un SATPolygon (utile pour le debug).           */
void SAT_DrawPolygon(sfRenderWindow* _rw, const SATPolygon* _poly,
    sfColor _color);

#endif /* SAT_H */