#pragma once
/* actor_unopposable_properties — actor_definition.unopposable (48 bytes), per an authoritative
 * structure dump (C:\halo\headers_ref\actor_unopposable_properties.h). Backs the tag-tuned
 * hysteresis/escalation tiers (6/7/8/9) described for actor_emotion_unopposable_retreat.c. */

typedef struct actor_unopposable_properties
{
    __int16           trigger_unreachable_level;    /* 0x00 (abs 616) */
    __int16           trigger_vehicle_level;          /* 0x02 (abs 618) */
    __int16           trigger_player_level;            /* 0x04 (abs 620) */
    unsigned __int16  pad;                              /* 0x06 */
    float             trigger_time_lower_bound;          /* 0x08 (abs 624) */
    float             trigger_time_upper_bound;           /* 0x0C (abs 628) */
    __int16           casualties_trigger;                 /* 0x10 (abs 632) */
    __int16           retreating_friends_trigger;           /* 0x12 (abs 634) */
    unsigned int      unused_trigger[3];                     /* 0x14 */
    float             retreat_time_lower_bound;                /* 0x20 (abs 648) */
    float             retreat_time_upper_bound;                 /* 0x24 (abs 652) */
    unsigned int      unused[2];                                  /* 0x28 */
} actor_unopposable_properties; /* 0x30 = 48 bytes */
