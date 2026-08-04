#pragma once
/* actor_panic_properties — actor_definition.panic (56 bytes), per an authoritative structure dump
 * (C:\halo\headers_ref\actor_panic_properties.h). Resolves the doc's "definition+672/676/680"
 * (flee-chance/notable-ally/roll) and "character+688" (surprise_distance) citations — this struct
 * sits at absolute byte 664 within actor_definition, confirmed by all four offsets landing exactly
 * on named fields. */

typedef struct actor_panic_properties
{
    float             cower_time_lower_bound;             /* 0x00 (abs 664) */
    float             cower_time_upper_bound;              /* 0x04 (abs 668) */
    float             panic_chance_friend_killed;           /* 0x08 (abs 672) */
    __int16           panic_leader_type;                     /* 0x0C (abs 676) */
    unsigned __int16  pad;                                    /* 0x0E */
    float             panic_chance_leader_type_killed;         /* 0x10 (abs 680) */
    float             panic_damage_threshold;                  /* 0x14 (abs 684) */
    float             surprise_distance;                        /* 0x18 (abs 688) */
    unsigned int      unused[7];                                 /* 0x1C */
} actor_panic_properties; /* 0x38 = 56 bytes */
