#pragma once
/* actor_meta_data — actor bookkeeping block (92 bytes), embedded in actor_datum at +0x4. Full DB layout. */

typedef struct actor_meta_data
{
    __int16         type;                          /* 0x00 */
    unsigned __int8 swarm;                          /* 0x02 */
    unsigned __int8 frozen;                         /* 0x03 */
    unsigned __int8 active;                         /* 0x04 */
    unsigned __int8 encounterless;                  /* 0x05 */
    unsigned __int8 force_active;                   /* 0x06 */
    unsigned __int8 erase_next_tick;                /* 0x07 */
    int             last_active_time;               /* 0x08 */
    __int16         encounterless_active_timer;     /* 0x0C */
    unsigned __int8 dormant_desire;                 /* 0x0E */
    unsigned __int8 dormant;                        /* 0x0F */
    __int16         become_dormant_timer;           /* 0x10 */
    unsigned char _pad0[2]; /* db-verified padding */
    int             unit_index;                     /* 0x14 */
    unsigned __int8 unique_leader;                  /* 0x18 */
    unsigned char _pad1[1]; /* db-verified padding */
    __int16         swarm_unit_count;               /* 0x1A */
    __int16         swarm_original_unit_count;      /* 0x1C */
    unsigned char _pad2[2]; /* db-verified padding */
    int             swarm_unit_index;               /* 0x20 */
    int             swarm_cache_index;              /* 0x24 — index into swarm_data */
    int             next_actor_index;               /* 0x28 */
    int             disconnected_encounter_index;   /* 0x2C */
    int             encounter_index;                /* 0x30 */
    __int16         disconnected_squad_index;       /* 0x34 */
    __int16         squad_index;                    /* 0x36 */
    __int16         platoon_index;                  /* 0x38 */
    __int16         team_index;                     /* 0x3A */
    unsigned __int8 stored_prevehicle_encounter;    /* 0x3C */
    unsigned char _pad3[3]; /* db-verified padding */
    int             prevehicle_encounter_index;     /* 0x40 */
    __int16         prevehicle_squad_index;         /* 0x44 */
    __int16         service_timer;                  /* 0x46 */
    unsigned __int8 timeslice;                      /* 0x48 */
    unsigned char _pad4[1]; /* db-verified padding */
    __int16         highest_prop_timer;             /* 0x4A */
    int             first_prop_index;               /* 0x4C */
    int             interesting_orphan_index;       /* 0x50 */
    int             definition_index;               /* 0x54 */
    int             variant_definition_index;       /* 0x58 */
} actor_meta_data;                                  /* 92 bytes */
