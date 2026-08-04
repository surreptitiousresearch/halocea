#pragma once
/* hud_messaging_globals_definition — runtime HUD messaging state (2280 bytes). Reset wholesale to zero on
 * new map. The leading message_data array is the per-local-player hud_messaging_datum[2] block; the trailing
 * flash / objective / timer fields are exposed per the database so the scripted-HUD builtins can address
 * them. Total size and field offsets are authoritative from the database. */

#include "hud_messaging_datum.h"
#include "hud_state_message_definition.h"
#include "hud_objective_message.h"
#include "hud_timer_data_definition.h"

typedef struct hud_messaging_globals_definition
{
    hud_messaging_datum           message_data[2];       /* 0x000 (hud_messaging_datum[2], 0x8C0 bytes) */
    int                           flash_start_time;      /* 0x8C0 */
    unsigned char                 use_flash;             /* 0x8C4 */
    unsigned char                 magic_number;          /* 0x8C5 */
    unsigned char                 pad[3];                /* 0x8C6 (3 bytes named; +3 align pad to 0x8CC) */
    unsigned char _pad0[3]; /* db-verified padding */
    hud_state_message_definition *help_message;          /* 0x8CC */
    hud_objective_message         objective;             /* 0x8D0 */
    hud_timer_data_definition     timer;                 /* 0x8D8 */
} hud_messaging_globals_definition;                      /* 0x8E8 (2280 bytes) */
