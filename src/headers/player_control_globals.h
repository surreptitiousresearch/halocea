#pragma once
/* player_control_globals @ player_control_globals — per-player control input state
 * (player_control.c). player_control is a 64-byte record. */

#include "player_control.h"

typedef struct player_control_globals_t
{
    unsigned int   action_test_flags;        /* 0x00 */
    unsigned int   testing_for_action_flags; /* 0x04 */
    unsigned int   inhibit_button_flags;     /* 0x08 */
    unsigned int   control_flags;            /* 0x0C */
    player_control players[2];               /* 0x10 */
} player_control_globals_t;

#ifdef __cplusplus
extern "C" {
#endif

extern player_control_globals_t *player_control_globals;

#ifdef __cplusplus
}
#endif
