#pragma once
/* object_function_definition — one element of an object definition's functions tag block (360 bytes):
 * a periodic/transition function spec plus its precomputed runtime reciprocals. Layout from the
 * database (types_members). */

typedef struct object_function_definition
{
    unsigned int flags;                              /* 0x000 — 0x1 invert, 0x2 additive (turning),
                                                      *         0x4 "always active when off" */
    float        period;                             /* 0x004 */
    __int16      scale_period_by_function_index;     /* 0x008 — 1-based incoming function index */
    __int16      function_type;                      /* 0x00A */
    __int16      scale_function_by_function_index;   /* 0x00C */
    __int16      wobble_function_type;               /* 0x00E */
    float        wobble_period;                      /* 0x010 */
    float        wobble_magnitude;                   /* 0x014 */
    float        square_wave_threshold;              /* 0x018 */
    __int16      step_count;                         /* 0x01C */
    __int16      map_result_to_transition_function;  /* 0x01E */
    __int16      sawtooth_count;                     /* 0x020 */
    __int16      add_function_index;                 /* 0x022 */
    __int16      scale_result_by_function_index;     /* 0x024 */
    __int16      bounds_mode;                        /* 0x026 */
    float        lower_bound;                        /* 0x028 */
    float        upper_bound;                        /* 0x02C */
    float        pad2;                               /* 0x030 */
    unsigned __int16 pad3;                           /* 0x034 */
    __int16      turn_off_with_function_index;       /* 0x036 */
    float        scale_by;                           /* 0x038 */
    int          unused1[63];                        /* 0x03C */
    float        runtime_reciprocal_bounds_range;    /* 0x138 */
    float        runtime_reciprocal_sawtooth_count;  /* 0x13C */
    float        runtime_reciprocal_step_count;      /* 0x140 */
    float        runtime_one_over_period;            /* 0x144 */
    char         usage[32];                          /* 0x148 */
} object_function_definition;                        /* 360 bytes */
