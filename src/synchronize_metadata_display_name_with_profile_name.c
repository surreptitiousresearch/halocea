/* synchronize_metadata_display_name_with_profile_name @0x83727078 — stub in this release: reports success
 * (returns 1) without touching saved-game metadata. */

#include <stdint.h>

/* attested int (was unsigned char): li r3,1 + 0 consumers -> width-agnostic int precedent */
int synchronize_metadata_display_name_with_profile_name(int index, uint16_t *game_display_name)
{
    return 1;
}
