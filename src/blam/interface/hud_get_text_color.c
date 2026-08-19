/* hud_get_text_color @0x836A4B70 — copies the active HUD messaging definition's state_color
 * (used for scripted-text/state-message tinting) into the caller's result. */

#include "headers/hud_globals.h"
#include "headers/hud_messaging_globals.h"
#include "headers/real_argb_color.h"

real_argb_color * hud_get_text_color(real_argb_color *result)
{
    result->n[0] = hud_msg_def->state_color.n[0];
    result->n[1] = hud_msg_def->state_color.n[1];
    result->n[2] = hud_msg_def->state_color.n[2];
    result->n[3] = hud_msg_def->state_color.n[3];
    return result;
}
