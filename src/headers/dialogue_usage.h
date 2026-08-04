#pragma once
/* dialogue_usage — one row of the global dialogue/communication table (global_dialogue_table). Maps a
 * communication_type + matching conditions to a vocalization/animation and the priorities/weight used when
 * selecting which line an AI actor speaks. 40 bytes, full DB layout. */

typedef struct dialogue_usage
{
    __int16 communication_type;        /* 0x00 */
    __int16 communication_priority;    /* 0x02 */
    __int16 vocalization_type;         /* 0x04 */
    __int16 animation_type;            /* 0x06 */
    __int16 protagonist_type;          /* 0x08 */
    __int16 protagonist_look_priority; /* 0x0A */
    __int16 recipient_look_direction;  /* 0x0C */
    __int16 recipient_look_priority;   /* 0x0E */
    float   weight;                    /* 0x10 */
    float   repeat_delay;              /* 0x14 */
    __int16 flags;                     /* 0x18 */
    __int16 required_group;            /* 0x1A */
    __int16 required_hostility;        /* 0x1C */
    __int16 required_enemy_status;     /* 0x1E */
    __int16 required_subject_race;     /* 0x20 */
    __int16 required_cause_race;       /* 0x22 */
    __int16 required_damage;           /* 0x24 */
} dialogue_usage;                      /* 40 bytes */
