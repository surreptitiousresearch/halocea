#pragma once
/* tag_iterator — a data_iterator paired with the group tag being iterated. */

#include "data_iterator.h"

typedef struct tag_iterator
{
    data_iterator iterator;       /* 0x00 */
    unsigned int  key_group_tag;  /* 0x10 */
} tag_iterator;
