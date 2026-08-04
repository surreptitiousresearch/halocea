/* tag_data_resize @0x836F7060 — no-op in the cache (release) build: tag data is fixed at cache-build time.
 * Always returns 0 (failure). */

#include <stdint.h>
#include "headers/tag_data.h"

/* attested: uint8_t return — callers byte-normalize (clrlwi r11, r3, 24 @ 8372EBF8 et al.) */
uint8_t tag_data_resize(tag_data *data, int size)
{
    return 0;
}
