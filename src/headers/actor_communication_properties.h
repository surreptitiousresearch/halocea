#pragma once
/* actor_communication_properties — actor_definition.communication (64 bytes), per an authoritative
 * structure dump (C:\halo\headers_ref\actor_communication_properties.h). Sits at absolute byte 1016.
 * The four time bounds resolve the doc's "character tag's non-combat (floats [254..255]) or combat
 * ([256..257]) idle-vocalization range" citation from actor_reset_idle_vocalization_timer.c — float
 * index 254 = byte 1016 = idle_noncombat_time_lower_bound, confirmed exactly through all four. */

typedef struct actor_communication_properties
{
    float         idle_noncombat_time_lower_bound;   /* 0x00 (abs 1016) — float[254] */
    float         idle_noncombat_time_upper_bound;    /* 0x04 (abs 1020) — float[255] */
    float         idle_combat_time_lower_bound;         /* 0x08 (abs 1024) — float[256] */
    float         idle_combat_time_upper_bound;           /* 0x0C (abs 1028) — float[257] */
    unsigned int  unused_communication[12];                 /* 0x10 */
} actor_communication_properties; /* 0x40 = 64 bytes */
