#pragma once
/* actor_situation — actor_datum member at 0x1EC (492), 123 bytes. Full layout from an authoritative
 * actor_datum structure dump (ground truth; supersedes any prior opaque placeholder). Per-type
 * (biped/vehicle-class) tallies of nearby friends/enemies used by combat-status decisions. */

typedef struct actor_situation
{
    char known_enemies;                          /* 0x00 */
    char visible_reachable_enemies;               /* 0x01 */
    char specific_threats[9];                     /* 0x02 */
    char cumulative_threats[9];                   /* 0x0B */
    char area_friends;                            /* 0x14 */
    char area_fighting_friends;                   /* 0x15 */
    char area_fire_support_friends;               /* 0x16 */
    char area_friends_by_type[16];                /* 0x17 */
    char area_fighting_friends_by_type[16];       /* 0x27 */
    char visible_friends;                         /* 0x37 */
    char visible_fighting_friends;                /* 0x38 */
    char visible_friends_by_type[16];             /* 0x39 */
    char visible_fighting_friends_by_type[16];    /* 0x49 */
    char close_friends;                           /* 0x59 */
    char close_fighting_friends;                  /* 0x5A */
    char close_friends_by_type[16];               /* 0x5B */
    char close_fighting_friends_by_type[16];      /* 0x6B */
} actor_situation; /* 0x7B = 123 bytes */
