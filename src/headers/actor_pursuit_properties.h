#pragma once
/* actor_pursuit_properties — actor_definition.pursuit (60 bytes), per an authoritative structure dump
 * (C:\halo\headers_ref\actor_pursuit_properties.h). Sits at absolute byte 828, immediately before
 * `berserk`. Timing bounds for the uncover/search/wait pursuit trio (Part 3/4's lost-contact handling)
 * — not yet cross-referenced against any specific raw-offset doc citation, but fills what was
 * previously an unlabeled 60-byte gap between `defensive` and `berserk`. */

typedef struct actor_pursuit_properties
{
    float             uncover_delay_time_lower_bound;      /* 0x00 (abs 828) */
    float             uncover_delay_time_upper_bound;       /* 0x04 (abs 832) */
    float             target_location_time_lower_bound;      /* 0x08 (abs 836) */
    float             target_location_time_upper_bound;       /* 0x0C (abs 840) */
    float             pursuit_position_time_lower_bound;        /* 0x10 (abs 844) */
    float             pursuit_position_time_upper_bound;         /* 0x14 (abs 848) */
    __int16           coordinated_pursuit_positions;               /* 0x18 (abs 852) */
    __int16           uncoordinated_pursuit_positions;               /* 0x1A (abs 854) */
    unsigned int      unused[8];                                       /* 0x1C */
} actor_pursuit_properties; /* 0x3C = 60 bytes */
