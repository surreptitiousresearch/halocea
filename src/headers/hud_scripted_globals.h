#pragma once
#include <stdint.h>
/* hud_scripted_globals_definition — script-controlled HUD visibility flags (4 bytes). Layout from the database. */

typedef struct hud_scripted_globals_definition
{
    uint8_t show_hud;           /* 0x00 */
    uint8_t show_hud_help_text; /* 0x01 */
    uint8_t pad[2];             /* 0x02 */
} hud_scripted_globals_definition;

extern hud_scripted_globals_definition *hud_scripted_globals;
