#include "headers/blam_data_globals.h"
/* hud_messaging_initialize @ 0x836A3F68 — allocate hud_messaging_globals (2280 bytes) */

typedef struct hud_messaging_globals_definition hud_messaging_globals_definition;
extern void *game_state_malloc(const char *name, const char *type, int size);

void hud_messaging_initialize(void)
{
    hud_messaging_globals = game_state_malloc("hud messaging", 0, 2280);
}
