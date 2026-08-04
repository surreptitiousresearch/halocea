#pragma once
/* hud_messaging_datum — per-local-player HUD message state (1120 bytes): a 4-slot message ring plus
 * the persistent state-message and line bookkeeping. Layout from the database. */

#include "hud_message.h"
#include "hud_state_message.h"

typedef struct hud_messaging_datum
{
    hud_message       messages[4];            /* 0x000 */
    hud_state_message state_message;          /* 0x230 (560) */
    short             first_message;          /* 0x45C (1116) */
    unsigned char     leave_first_line_blank; /* 0x45E (1118) */
    unsigned char     custom_message;         /* 0x45F (1119) */
} hud_messaging_datum;                        /* 0x460 (1120 bytes) */
