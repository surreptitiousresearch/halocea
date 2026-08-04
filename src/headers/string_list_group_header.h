#pragma once
/* string_list_group_header — a 'str#' string_list tag: one block of string references.
 * Layout from headers_ref string_list_group_header.h (0xC bytes). */

#include "tag_block.h"

typedef struct string_list_group_header
{
    tag_block string_references; /* 0x00 — string_list_string_reference elements */
} string_list_group_header;      /* 0x0C */
