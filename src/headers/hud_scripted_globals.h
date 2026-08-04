#pragma once
/* hud_scripted_globals_definition — script-controlled HUD visibility flags (4 bytes). Layout from the database. */

typedef struct hud_scripted_globals_definition
{
    unsigned __int8 show_hud;           /* 0x00 */
    unsigned __int8 show_hud_help_text; /* 0x01 */
    unsigned __int8 pad[2];             /* 0x02 */
} hud_scripted_globals_definition;

extern hud_scripted_globals_definition *hud_scripted_globals;
