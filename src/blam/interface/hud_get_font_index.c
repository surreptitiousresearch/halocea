/* hud_get_font_index @0x836A4B08 — picks the HUD text font tag: the multiplayer font for the local
 * split-screen/system-link player who isn't the "coop primary" slot, falling back to the single-player
 * font whenever there's only one local player, this is the coop-primary slot, or the multiplayer font
 * tag simply isn't set. */

#include <stdint.h>
#include "headers/hud_globals.h"
#include "headers/hud_messaging_globals.h"
#include "headers/blam_data_globals.h"

extern int16_t local_player_count(void);

int hud_get_font_index(void)
{
    unsigned char use_multi_player_font = local_player_count() > 1 && hcex_coop_local_player_index < 0;

    if (!use_multi_player_font)
        return hud_msg_def->single_player_font.index;

    int index = hud_msg_def->multi_player_font.index;
    if (index == -1)
        return hud_msg_def->single_player_font.index;
    return index;
}
