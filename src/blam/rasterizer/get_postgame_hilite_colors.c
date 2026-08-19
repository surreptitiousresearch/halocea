/* get_postgame_hilite_colors @0x83746628 */
#include "headers/real_argb_color.h"

void get_postgame_hilite_colors(real_argb_color *normal, real_argb_color *column_hilite, real_argb_color *winner)
{
    normal->n[0] = 1.0f;
    normal->n[1] = 0.45882353f;
    normal->n[2] = 0.72941178f;
    normal->n[3] = 1.0f;

    column_hilite->n[0] = 1.0f;
    column_hilite->n[1] = 1.0f;
    column_hilite->n[2] = 1.0f;
    column_hilite->n[3] = 0.0f;

    winner->n[0] = 1.0f;
    winner->n[1] = 0.98000002f;
    winner->n[2] = 0.95999998f;
    winner->n[3] = 0.95999998f;
}
