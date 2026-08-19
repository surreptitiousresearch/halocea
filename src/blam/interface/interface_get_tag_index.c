/* interface_get_tag_index @0x836A64A0 — resolve one of the global interface tag references by index. When the
 * game globals tag carries an interface_tag_references block, read the tag index from element +12; otherwise
 * fall back to the legacy fixed table at absolute address (16 * index + 0xC). */

#include <stdint.h>
#include "headers/game_globals_tag.h"
#include "headers/tag_reference.h"

int interface_get_tag_index(int16_t interface_tag_index)
{
    if ( global_game_globals->interface_tag_references.count )
        return ((tag_reference *)global_game_globals->interface_tag_references.address)[interface_tag_index].index;
    else
        /* legacy fixed table at absolute base 0: 16-byte tag_reference stride, index field @+0xC */
        return ((tag_reference *)0)[interface_tag_index].index;
}
