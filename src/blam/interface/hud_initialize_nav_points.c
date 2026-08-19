#include "headers/blam_data_globals.h"
/* hud_initialize_nav_points @ 0x837ECF08 — allocate nav_point_data (96 bytes) */

typedef struct hud_nav_point_player_datum hud_nav_point_player_datum;
extern void *game_state_malloc(const char *name, const char *type, int size);

/* Returns void: r3 at blr is game_state_malloc residue (stored to nav_point_data), no caller consumes it. */
void hud_initialize_nav_points(void)
{
    nav_point_data = game_state_malloc("hud nav points", 0, 96);
}
