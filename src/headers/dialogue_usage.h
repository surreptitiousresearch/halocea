#pragma once
#include <stdint.h>
/* dialogue_usage — one row of the global dialogue/communication table (global_dialogue_table). Maps a
 * communication_type + matching conditions to a vocalization/animation and the priorities/weight used when
 * selecting which line an AI actor speaks. 40 bytes, full DB layout. */

typedef struct dialogue_usage
{
    int16_t communication_type;        /* 0x00 */
    int16_t communication_priority;    /* 0x02 */
    int16_t vocalization_type;         /* 0x04 */
    int16_t animation_type;            /* 0x06 */
    int16_t protagonist_type;          /* 0x08 */
    int16_t protagonist_look_priority; /* 0x0A */
    int16_t recipient_look_direction;  /* 0x0C */
    int16_t recipient_look_priority;   /* 0x0E */
    float   weight;                    /* 0x10 */
    float   repeat_delay;              /* 0x14 */
    int16_t flags;                     /* 0x18 */
    int16_t required_group;            /* 0x1A */
    int16_t required_hostility;        /* 0x1C */
    int16_t required_enemy_status;     /* 0x1E */
    int16_t required_subject_race;     /* 0x20 */
    int16_t required_cause_race;       /* 0x22 */
    int16_t required_damage;           /* 0x24 */
} dialogue_usage;                      /* 40 bytes */
