#pragma once
/* actor_state_data — actor behaviour/state block (192 bytes), embedded in actor_datum at +0x60.
 * Full DB layout; action_data is the 132-byte action working-state union (modeled opaque — cast a pointer
 * to the relevant <action>_state_data to read its fields). */

#include <stdint.h>
#include "action_state_data.h"

typedef struct actor_state_data
{
    int16_t          initial_state;            /* 0x00 */
    int16_t          default_state;            /* 0x02 */
    int              last_default_state_time;  /* 0x04 */
    char             noncombat_sequence_id;    /* 0x08 */
    unsigned char    _pad09;                   /* 0x09 */
    int16_t          mode;                     /* 0x0A — current actor mode (>0 = updatable) */
    int16_t          action;                   /* 0x0C — current action class */
    int16_t          combat_status;            /* 0x0E */
    uint8_t  action_changed;           /* 0x10 */
    unsigned char    _pad11;                   /* 0x11 */
    int16_t          artificial_combat_status; /* 0x12 */
    int16_t          suspicion_combat_status;  /* 0x14 */
    unsigned char    _pad16[2];                /* 0x16 */
    int              suspicion_timer;          /* 0x18 */
    int              combat_mode_timer;        /* 0x1C */
    int              in_combat_timer;          /* 0x20 */
    int              certain_combat_timer;     /* 0x24 */
    int              uncertain_combat_timer;   /* 0x28 */
    uint8_t  had_visible_enemy;        /* 0x2C */
    uint8_t  been_in_combat;           /* 0x2D */
    uint8_t  command_list_immediate;   /* 0x2E */
    unsigned char    _pad2F;                   /* 0x2F */
    int16_t          command_list_index;       /* 0x30 */
    int16_t          command_list_delay_timer; /* 0x32 */
    int              last_command_list_time;   /* 0x34 */
    uint8_t  searching;                /* 0x38 */
    uint8_t  flying;                   /* 0x39 */
    unsigned char    _pad3A[2];                /* 0x3A */
    action_state_data action_data;             /* 0x3C — action working-state union (132 bytes) */
} actor_state_data;                            /* 192 bytes */
