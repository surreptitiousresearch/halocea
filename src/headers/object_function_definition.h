#pragma once
#include <stdint.h>
/* object_function_definition — one element of an object definition's functions tag block (360 bytes):
 * a periodic/transition function spec plus its precomputed runtime reciprocals. Layout from the
 * database (types_members). */

typedef struct object_function_definition
{
    unsigned int flags;                              /* 0x000 — 0x1 invert, 0x2 additive (turning),
                                                      *         0x4 "always active when off" */
    float        period;                             /* 0x004 */
    int16_t      scale_period_by_function_index;     /* 0x008 — 1-based incoming function index */
    int16_t      function_type;                      /* 0x00A */
    int16_t      scale_function_by_function_index;   /* 0x00C */
    int16_t      wobble_function_type;               /* 0x00E */
    float        wobble_period;                      /* 0x010 */
    float        wobble_magnitude;                   /* 0x014 */
    float        square_wave_threshold;              /* 0x018 */
    int16_t      step_count;                         /* 0x01C */
    int16_t      map_result_to_transition_function;  /* 0x01E */
    int16_t      sawtooth_count;                     /* 0x020 */
    int16_t      add_function_index;                 /* 0x022 */
    int16_t      scale_result_by_function_index;     /* 0x024 */
    int16_t      bounds_mode;                        /* 0x026 */
    float        lower_bound;                        /* 0x028 */
    float        upper_bound;                        /* 0x02C */
    float        pad2;                               /* 0x030 */
    uint16_t pad3;                           /* 0x034 */
    int16_t      turn_off_with_function_index;       /* 0x036 */
    float        scale_by;                           /* 0x038 */
    int          unused1[63];                        /* 0x03C */
    float        runtime_reciprocal_bounds_range;    /* 0x138 */
    float        runtime_reciprocal_sawtooth_count;  /* 0x13C */
    float        runtime_reciprocal_step_count;      /* 0x140 */
    float        runtime_one_over_period;            /* 0x144 */
    char         usage[32];                          /* 0x148 */
} object_function_definition;                        /* 360 bytes */
