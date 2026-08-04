#pragma once
/* unicode_string_list_group_header (tag) — DB layout: a single tag_block of string entries. */

#include "tag_block.h"

typedef struct unicode_string_list_group_header
{
    tag_block string_references; /* 0x00 */
} unicode_string_list_group_header; /* 12 bytes */
