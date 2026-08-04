#ifndef UNICODE_STRING_LIST_STRING_REFERENCE_H
#define UNICODE_STRING_LIST_STRING_REFERENCE_H

/* unicode_string_list_string_reference — one element (20 bytes) of a unicode_string_list ('ustr') tag's
 * string_references tag_block. A single tag_data holding the wide-char string bytes.
 * Source: DB type unicode_string_list_string_reference (member 'string' : tag_data @0). */

#include "tag_data.h"

typedef struct unicode_string_list_string_reference
{
    tag_data string;   /* 0x00: size @0, address @12 */
} unicode_string_list_string_reference;

#endif /* UNICODE_STRING_LIST_STRING_REFERENCE_H */
