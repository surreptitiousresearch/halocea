#include <stdint.h>
#include "headers/blam_data_globals.h"
/* trouble_is_brewing_load_icon @0x83732BC0 — optionally load the "trouble brewing" UI bitmap tag, then
 * cache its resolved tag index (or -1 if not loaded) in trouble_is_brewing_bitmap_tag for the renderer. */


extern int tag_load(unsigned int group_tag, const char *name, unsigned int flags);
extern int tag_loaded(uint32_t group_tag, const char *name);

void trouble_is_brewing_load_icon(uint8_t do_load)
{
    if (do_load)
        tag_load(0x6269746Du /* 'bitm' */, "ui\\shell\\bitmaps\\trouble_brewing", 0);
    trouble_is_brewing_bitmap_tag = tag_loaded(0x6269746Du /* 'bitm' */, "ui\\shell\\bitmaps\\trouble_brewing");
}
