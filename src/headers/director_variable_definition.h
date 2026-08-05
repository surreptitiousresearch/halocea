#pragma once
#include <stdint.h>
/* director_variable_definition — static definition of one debug "director variable": which control
 * bits drive it up/down/reset, the velocity scale, and the value range. Layout from the database. */

typedef struct director_variable_definition
{
    int16_t         negative_bit;     /* 0x00 — control bit that decreases the value (-1 = none) */
    int16_t         positive_bit;     /* 0x02 — control bit that increases the value (-1 = none) */
    int16_t         reset_bit;        /* 0x04 — control bit that resets to initial_value (-1 = none) */
    unsigned char _pad0[2]; /* db-verified padding */
    float           scale;            /* 0x08 */
    float           initial_value;    /* 0x0C */
    float           minimum;          /* 0x10 */
    float           maximum;          /* 0x14 */
    uint8_t has_hyper_scale;  /* 0x18 — apply debug_input_scale to the velocity */
} director_variable_definition;       /* 0x1C bytes */
