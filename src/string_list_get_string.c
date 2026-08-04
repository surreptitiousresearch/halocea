/* string_list_get_string @0x8377A300 — fetch a string by index from a string_list tag, returning
 * "<missing string>" for an invalid tag or out-of-range index. The returned buffer is null-terminated in place
 * at its stored length. */

#include <stdint.h>
#include "headers/global_tag_instances.h"
#include "headers/string_list_group_header.h"
#include "headers/string_list_string_reference.h"

char *string_list_get_string(int tag_index, int16_t string_index)
{
    char *result = "<missing string>";
    if ( tag_index != -1 )
    {
        string_list_group_header *string_list = TAG_GET(string_list_group_header, tag_index);
        if ( string_index >= 0 && string_index < string_list->string_references.count )
        {
            string_list_string_reference *entry =
                &((string_list_string_reference *)string_list->string_references.address)[string_index];
            if ( entry->string.size > 0 )
            {
                result = (char *)entry->string.address;
                result[entry->string.size - 1] = 0;
            }
        }
    }
    return result;
}
