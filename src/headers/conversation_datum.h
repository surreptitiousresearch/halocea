#pragma once
/* conversation_datum — one runtime AI conversation (100 bytes). Full DB layout. */

typedef struct conversation_datum
{
    __int16          identifier;                    /* 0x00 */
    __int16          conversation_definition_index;  /* 0x02 — index into scenario ai_conversations */
    unsigned __int8  scripted;                       /* 0x04 */
    unsigned __int8  any_line_spoken;                /* 0x05 */
    unsigned __int8  begun;                          /* 0x06 */
    unsigned __int8  finished;                        /* 0x07 */
    unsigned __int8  waiting_to_advance;             /* 0x08 */
    unsigned __int8  told_to_advance;                /* 0x09 */
    unsigned char    _pad0A[2];                       /* 0x0A */
    int              creation_time;                  /* 0x0C */
    int              triggering_player_unit_index;   /* 0x10 */
    unsigned int     participant_bitmask;            /* 0x14 */
    __int16          dialogue_indices[8];            /* 0x18 */
    int              actor_indices[8];               /* 0x28 */
    __int16          line_index;                     /* 0x48 */
    __int16          line_participant_index;         /* 0x4A */
    __int16          line_delay_timer;               /* 0x4C */
    unsigned __int16 line_flags;                     /* 0x4E */
    int              line_actor_index;               /* 0x50 */
    int              line_unit_index;                /* 0x54 */
    int              line_address_unit_index;        /* 0x58 */
    int              line_sound_index;               /* 0x5C */
    unsigned __int8  line_unspatialized;             /* 0x60 */
    unsigned __int8  line_spoken;                     /* 0x61 */
    unsigned __int8  line_finished;                  /* 0x62 */
    unsigned __int8  line_advance;                   /* 0x63 */
} conversation_datum;                                /* 100 bytes */
