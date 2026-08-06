/* hcex_tag_get_name @0x83681E28 — thunk: return the tag path string for a tag index.
 * tag_get_name is a Blam boundary. */

#include <stdint.h>

extern "C" char *tag_get_name(int16_t tag_index);

extern "C" char *hcex_tag_get_name(int tag_index)
{
    return tag_get_name(tag_index);
}
