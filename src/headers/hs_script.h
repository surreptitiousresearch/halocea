#pragma once
#include <stdint.h>
/* hs_script — one HaloScript script in the scenario's hs_scripts block (92-byte element). Layout from the
 * database. script_type 3/4 are the value-returning script kinds whose return_type the compiler reads. */

typedef struct hs_script
{
    char    name[32];               /* 0x00 */
    int16_t script_type;            /* 0x20 */
    int16_t return_type;            /* 0x22 */
    int     root_expression_index;  /* 0x24 */
    int     unused[13];             /* 0x28 */
} hs_script;                        /* 92 bytes */
