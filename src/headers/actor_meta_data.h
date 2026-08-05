#pragma once
#include <stdint.h>
/* actor_meta_data — actor bookkeeping block (92 bytes), embedded in actor_datum at +0x4. Full DB layout. */

typedef struct actor_meta_data
{
    int16_t         type;                          /* 0x00 */
    uint8_t swarm;                          /* 0x02 */
    uint8_t frozen;                         /* 0x03 */
    uint8_t active;                         /* 0x04 */
    uint8_t encounterless;                  /* 0x05 */
    uint8_t force_active;                   /* 0x06 */
    uint8_t erase_next_tick;                /* 0x07 */
    int             last_active_time;               /* 0x08 */
    int16_t         encounterless_active_timer;     /* 0x0C */
    uint8_t dormant_desire;                 /* 0x0E */
    uint8_t dormant;                        /* 0x0F */
    int16_t         become_dormant_timer;           /* 0x10 */
    unsigned char _pad0[2]; /* db-verified padding */
    int             unit_index;                     /* 0x14 */
    uint8_t unique_leader;                  /* 0x18 */
    unsigned char _pad1[1]; /* db-verified padding */
    int16_t         swarm_unit_count;               /* 0x1A */
    int16_t         swarm_original_unit_count;      /* 0x1C */
    unsigned char _pad2[2]; /* db-verified padding */
    int             swarm_unit_index;               /* 0x20 */
    int             swarm_cache_index;              /* 0x24 — index into swarm_data */
    int             next_actor_index;               /* 0x28 */
    int             disconnected_encounter_index;   /* 0x2C */
    int             encounter_index;                /* 0x30 */
    int16_t         disconnected_squad_index;       /* 0x34 */
    int16_t         squad_index;                    /* 0x36 */
    int16_t         platoon_index;                  /* 0x38 */
    int16_t         team_index;                     /* 0x3A */
    uint8_t stored_prevehicle_encounter;    /* 0x3C */
    unsigned char _pad3[3]; /* db-verified padding */
    int             prevehicle_encounter_index;     /* 0x40 */
    int16_t         prevehicle_squad_index;         /* 0x44 */
    int16_t         service_timer;                  /* 0x46 */
    uint8_t timeslice;                      /* 0x48 */
    unsigned char _pad4[1]; /* db-verified padding */
    int16_t         highest_prop_timer;             /* 0x4A */
    int             first_prop_index;               /* 0x4C */
    int             interesting_orphan_index;       /* 0x50 */
    int             definition_index;               /* 0x54 */
    int             variant_definition_index;       /* 0x58 */
} actor_meta_data;                                  /* 92 bytes */
