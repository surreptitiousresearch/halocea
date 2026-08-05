#pragma once
#include <stdint.h>
/* actor_external_orders — script-imposed overrides on an actor's behaviour (36 bytes). Layout from the
 * database type of the same name. Lives at offset 0x1C8 inside actor_datum. */

typedef struct actor_external_orders
{
    uint8_t stand_down;                    /* 0x00 */
    uint8_t defending;                     /* 0x01 */
    uint8_t playfighting;                  /* 0x02 */
    uint8_t disable_charging;              /* 0x03 */
    uint8_t pursuit_is_coordinator;        /* 0x04 */
    unsigned char _pad0[3]; /* db-verified padding */
    /* 0x05..0x07 padding */
    int             pursuit_group_prop_index;      /* 0x08 */
    int16_t         desired_target_type;           /* 0x0C */
    unsigned char _pad1[2]; /* db-verified padding */
    /* 0x0E..0x0F padding */
    int             desired_target_ai_index;       /* 0x10 */
    int             conversation_index;            /* 0x14 */
    int             conversation_attention_unit_index; /* 0x18 */
    int16_t         postcombat_type;               /* 0x1C */
    unsigned char _pad2[2]; /* db-verified padding */
    /* 0x1E..0x1F padding */
    int             postcombat_prop_index;         /* 0x20 */
} actor_external_orders;                            /* 36 bytes */
