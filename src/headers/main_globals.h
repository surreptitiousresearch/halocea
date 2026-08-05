#pragma once
/* _main_globals @ 0x8441DFF8 — top-level standalone-loop control block (main.c) */

#include <stdint.h>
#include "bitmap_data.h"

/* Shadow pattern: object name == type name, so the type must be a bare struct TAG
 * (separate namespace in C) with no typedef. Consumers use `struct main_globals`. */
struct main_globals
{
    int64_t           last_time_clocks;              /* 0x000 */
    unsigned int      last_time_msec;                /* 0x008 */
    unsigned char _pad0[4]; /* db-verified padding */
    int64_t           last_render_clocks;            /* 0x010 */
    int64_t           last_vblank_index;             /* 0x018 */
    int64_t           last_initial_vblank_index;     /* 0x020 */
    int64_t           last_achievable_vblank_index;  /* 0x028 */
    int64_t           last_present_vblank_index;     /* 0x030 */
    unsigned char     did_time_overflow_occur;       /* 0x038 */
    unsigned char _pad1[3]; /* db-verified padding */
    float             seconds_elapsed;               /* 0x03C */
    int16_t           connection;                    /* 0x040 */
    uint16_t  screenshot_identifier;         /* 0x042 */
    bitmap_data      *movie;                         /* 0x044 */
    int               recording_start_tick;          /* 0x048 */
    int               recording_stop_tick;           /* 0x04C */
    int               recording_frame_index;         /* 0x050 */
    float             recording_dt;                  /* 0x054 */
    unsigned char     reset_map;                     /* 0x058 */
    unsigned char     rename_map;                    /* 0x059 */
    unsigned char     revert_map;                    /* 0x05A */
    unsigned char     skip_cinematic;                /* 0x05B */
    unsigned char     save_map;                      /* 0x05C */
    unsigned char     save_map_safely;               /* 0x05D */
    unsigned char     save_map_timeout;              /* 0x05E */
    unsigned char     saving_map;                    /* 0x05F */
    int               ticks_until_next_save_check;   /* 0x060 */
    int               ticks_unable_to_save;          /* 0x064 */
    unsigned int      map_change_load_timer;         /* 0x068 */
    int16_t           safe_intervals;                /* 0x06C */
    unsigned char     won_map;                       /* 0x06E */
    unsigned char     lost_map;                      /* 0x06F */
    unsigned char     respawn;                       /* 0x070 */
    unsigned char     save_core;                     /* 0x071 */
    unsigned char     load_core;                     /* 0x072 */
    unsigned char     load_core_at_startup;          /* 0x073 */
    int16_t           switch_to_structure_bsp_index; /* 0x074 */
    unsigned char     main_menu_scenario_loaded;     /* 0x076 */
    unsigned char     want_to_be_at_main_menu;       /* 0x077 */
    unsigned char     run_xdemos;                    /* 0x078 */
    unsigned char     fade_to_dashboard;             /* 0x079 */
    unsigned char     exit_to_dashboard;             /* 0x07A */
    unsigned char     want_to_exit;                  /* 0x07B */
    int               idle_timeout;                  /* 0x07C */
    int               idle_last_interesting;         /* 0x080 */
    int               idle_last_activity;            /* 0x084 */
    unsigned char     playback_last_recording;       /* 0x088 */
    unsigned char     halt_time_scale;               /* 0x089 */
    unsigned char     restart_time;                  /* 0x08A */
    unsigned char     load_last_solo_level;          /* 0x08B */
    unsigned char     cutscene_skip;                 /* 0x08C */
    unsigned char _pad2[1]; /* db-verified padding */
    int16_t           skip_ticks;                    /* 0x08E */
    int16_t           loss_timer;                    /* 0x090 */
    int16_t           respawn_timer;                 /* 0x092 */
    unsigned char     queue_map;                     /* 0x094 */
    unsigned char     pad0[3];                       /* 0x095 */
    unsigned char     solo_try_and_load_from_persistent_storage; /* 0x098 */
    char              soloplayer_map_name[256];      /* 0x099 */
    char              multiplayer_map_name[256];     /* 0x199 */
    char              queued_map_name[256];          /* 0x299 */
    unsigned char     want_to_connect;               /* 0x399 */
    char              connect_address[32];           /* 0x39A */
    char              connect_password[9];           /* 0x3BA */
    unsigned char _pad3[1]; /* db-verified padding */
    int16_t           vblank_interval_current;       /* 0x3C4 */
    int16_t           vblank_interval_minimum;       /* 0x3C6 */
    unsigned char     vblank_interval_held;          /* 0x3C8 */
    unsigned char _pad4[1]; /* db-verified padding */
    int16_t           vblank_failure_count[6];       /* 0x3CA */
    unsigned char _pad5[2]; /* db-verified padding */
    int64_t           vblank_last_failure_time[6];   /* 0x3D8 */
    unsigned int     *vblank_flip_counter;           /* 0x408 */
    int16_t           vblank_flip_delta_next_index;  /* 0x40C */
    int16_t           vblank_flip_deltas[15];        /* 0x40E */
};

extern struct main_globals main_globals;

/* Screenshot sequence counter (data-bss @ 0x8441E42C); no other header declared it. */
extern int global_screenshot_count;
