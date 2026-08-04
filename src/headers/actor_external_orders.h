#pragma once
/* actor_external_orders — script-imposed overrides on an actor's behaviour (36 bytes). Layout from the
 * database type of the same name. Lives at offset 0x1C8 inside actor_datum. */

typedef struct actor_external_orders
{
    unsigned __int8 stand_down;                    /* 0x00 */
    unsigned __int8 defending;                     /* 0x01 */
    unsigned __int8 playfighting;                  /* 0x02 */
    unsigned __int8 disable_charging;              /* 0x03 */
    unsigned __int8 pursuit_is_coordinator;        /* 0x04 */
    unsigned char _pad0[3]; /* db-verified padding */
    /* 0x05..0x07 padding */
    int             pursuit_group_prop_index;      /* 0x08 */
    __int16         desired_target_type;           /* 0x0C */
    unsigned char _pad1[2]; /* db-verified padding */
    /* 0x0E..0x0F padding */
    int             desired_target_ai_index;       /* 0x10 */
    int             conversation_index;            /* 0x14 */
    int             conversation_attention_unit_index; /* 0x18 */
    __int16         postcombat_type;               /* 0x1C */
    unsigned char _pad2[2]; /* db-verified padding */
    /* 0x1E..0x1F padding */
    int             postcombat_prop_index;         /* 0x20 */
} actor_external_orders;                            /* 36 bytes */
