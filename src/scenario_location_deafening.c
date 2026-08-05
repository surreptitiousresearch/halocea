/* scenario_location_deafening @0x83703508 — true if the BSP cluster containing `location` plays a background
 * sound whose tag is flagged "deafening" (flags bit 0). Walks cluster -> background sound palette entry ->
 * sound tag. */

#include "headers/structure_bsp.h"
#include "headers/structure_cluster.h"
#include "headers/structure_background_sound_palette_entry.h"
#include "headers/global_tag_instances.h"
#include "headers/location.h"
#include "headers/looping_sound_definition_flags.h"
#include "headers/blam_data_globals.h"
#include <stdint.h>

/* attested: uint8_t return — clrlwi r3,r6,31 at 0x83703574; both callers byte-normalize (clrlwi r11,r3,24) */
uint8_t scenario_location_deafening(const location *location)
{
    int16_t cluster_index = location->cluster_index;
    int palette_index = ((structure_cluster *)global_structure_bsp->clusters.address)[cluster_index].background_sound_palette_index;
    if ( palette_index != -1 && palette_index < global_structure_bsp->background_sound_palette.count )
    {
        int sound_tag = ((structure_background_sound_palette_entry *)global_structure_bsp->background_sound_palette.address)[palette_index].background_sound.index;
        if ( sound_tag != -1 )
            /* background_sound is a looping_sound tag; bit 0 = deafening (NOT sound_definition's bit0). */
            return *TAG_GET(unsigned int, sound_tag) & (1u << _looping_sound_deafening_bit);
    }
    return 0;
}
