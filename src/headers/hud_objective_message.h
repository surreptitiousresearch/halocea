#pragma once
/* hud_objective_message — active HUD objective message pointer + display uptime (8 bytes). DB-verified. */

#include "hud_state_message_definition.h"

typedef struct hud_objective_message
{
    hud_state_message_definition *message;   /* 0x0 */
    __int16                       uptime;    /* 0x4 */
    __int16                       pad;       /* 0x6 */
} hud_objective_message;                     /* 0x8 (8 bytes) */
