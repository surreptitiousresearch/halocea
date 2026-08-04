#pragma once
/* hud_state_message_definition — one HUD message definition record (64 bytes) inside the scenario HUD
 * messages tag. DB layout (partial; only the fields read by the scripted-HUD builtins are named). */

typedef struct hud_state_message_definition
{
    char             name[32];            /* 0x00 */
    unsigned __int16 text_start_index;    /* 0x20 */
    unsigned __int16 element_start_index; /* 0x22 */
    unsigned char    element_count;       /* 0x24 */
    unsigned char    pad[3];              /* 0x25 */
    int              unused[6];           /* 0x28 */
} hud_state_message_definition;           /* 0x40 (64 bytes) */
