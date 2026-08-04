#pragma once
/* string_list_string_reference — one string_list entry: a tag_data holding the string bytes.
 * Layout from headers_ref string_list_string_reference.h (0x14 bytes). */

#include "tag_data.h"

typedef struct string_list_string_reference
{
    tag_data string; /* 0x00 */
} string_list_string_reference; /* 0x14 */
