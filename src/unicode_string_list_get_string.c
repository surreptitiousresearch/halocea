/* unicode_string_list_get_string @0x8377A378 — fetch a wide string by index from a unicode_string_list
 * ('ustr') tag, with the MCC override (hcex_unicode_string_list_get_string) taking precedence. Returns the
 * literal L"<missing string>" for any invalid tag/index/empty entry.
 *
 * Deviation: the function force-null-terminates the returned string at (length & ~1) - 2 bytes. */

#include <stdint.h>
#include "headers/global_tag_instances.h"
#include "headers/unicode_string_list_group_header.h"
#include "headers/unicode_string_list_string_reference.h"

extern unsigned short *hcex_unicode_string_list_get_string(int tag_index, int16_t string_index);

unsigned short *unicode_string_list_get_string(int tag_index, int16_t string_index)
{
    unsigned short *result = hcex_unicode_string_list_get_string(tag_index, string_index);
    if ( result )
        return result;

    if ( tag_index == -1 )
        return L"<missing string>";

    unicode_string_list_group_header *definition = TAG_GET(unicode_string_list_group_header, tag_index);
    if ( string_index < 0 || string_index >= definition->string_references.count )
        return L"<missing string>";

    unicode_string_list_string_reference *entry =
        &((unicode_string_list_string_reference *)definition->string_references.address)[string_index];
    if ( entry->string.size <= 0 )
        return L"<missing string>";

    result = (unsigned short *)entry->string.address;
    /* DEVIATION: `(size & 0xFFFFFFFE) - 2` in pseudocode is the compiler's lowering of the wide-char
     * element index `size / sizeof(wchar) - 1` (clrrwi r11,r10,1 @0x8377A3F8 then sth r10,-2(r9)
     * @0x8377A404). The divisor is cast to int deliberately: size_t promotion would turn the size==1
     * case from element -1 (what the disasm computes: address + 0 - 2) into SIZE_MAX. */
    result[entry->string.size / (int)sizeof(unsigned short) - 1] = 0;
    return result;
}
