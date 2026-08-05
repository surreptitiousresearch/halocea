#pragma once
/* animation_list — a counted array of named animations. Layout from the database. */

#include <stdint.h>
#include "animation_list_entry.h"

typedef struct animation_list
{
    int16_t                count;      /* 0x00 */
    char                   pad_0002[2]; /* 0x02 */
    animation_list_entry  *animations; /* 0x04 */
} animation_list; /* 8 bytes */
