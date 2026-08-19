/* static_array_resize @0x83784A18 — resize a fixed-capacity inline array's live element count, provided
 * new_count is in [0, maximum_count). No-op (still returns success) if new_count already equals *count.
 *
 * Surprising-but-verified asymmetry (confirmed via disasm): shrinking poisons the vacated slots with 0xFF
 * bytes, while growing zero-fills the newly added slots — not the same fill value in both directions. */

#include <stdint.h>
#include <string.h>

uint8_t static_array_resize(uint8_t *count, void *elements, int16_t element_size, int16_t maximum_count, int16_t new_count)
{
    if ( new_count < 0 || new_count >= maximum_count )
        return 0;

    if ( *count != new_count )
    {
        char *old_end = (char *)elements + element_size * *count;
        char *new_end = (char *)elements + element_size * new_count;

        if ( new_end <= old_end )
            memset(new_end, 0xFF, old_end - new_end);
        else
            memset(old_end, 0, new_end - old_end);

        *count = (uint8_t)new_count;
    }

    return 1;
}
