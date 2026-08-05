#pragma once
#include <stdint.h>
/* conversation_datum — one runtime AI conversation (100 bytes). Full DB layout. */

typedef struct conversation_datum
{
    int16_t          identifier;                    /* 0x00 */
    int16_t          conversation_definition_index;  /* 0x02 — index into scenario ai_conversations */
    uint8_t  scripted;                       /* 0x04 */
    uint8_t  any_line_spoken;                /* 0x05 */
    uint8_t  begun;                          /* 0x06 */
    uint8_t  finished;                        /* 0x07 */
    uint8_t  waiting_to_advance;             /* 0x08 */
    uint8_t  told_to_advance;                /* 0x09 */
    unsigned char    _pad0A[2];                       /* 0x0A */
    int              creation_time;                  /* 0x0C */
    int              triggering_player_unit_index;   /* 0x10 */
    unsigned int     participant_bitmask;            /* 0x14 */
    int16_t          dialogue_indices[8];            /* 0x18 */
    int              actor_indices[8];               /* 0x28 */
    int16_t          line_index;                     /* 0x48 */
    int16_t          line_participant_index;         /* 0x4A */
    int16_t          line_delay_timer;               /* 0x4C */
    uint16_t line_flags;                     /* 0x4E */
    int              line_actor_index;               /* 0x50 */
    int              line_unit_index;                /* 0x54 */
    int              line_address_unit_index;        /* 0x58 */
    int              line_sound_index;               /* 0x5C */
    uint8_t  line_unspatialized;             /* 0x60 */
    uint8_t  line_spoken;                     /* 0x61 */
    uint8_t  line_finished;                  /* 0x62 */
    uint8_t  line_advance;                   /* 0x63 */
} conversation_datum;                                /* 100 bytes */
