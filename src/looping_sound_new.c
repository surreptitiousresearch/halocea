/* looping_sound_new @0x83716028 — allocate a new looping-sound datum for an identifier and seed each
 * track's randomized start delay. Returns the datum index, or -1 if the sound manager is inactive or the
 * pool is full.
 *
 * looping_sound datum (228 bytes): definition dword[1], identifier dword[2], muted flag word[40] (@80)=0,
 * needs-stop byte @78=0, per-track start time int[] @84 (dword[21 + track]). Looping sound definition
 * (v12): scale-low @4, scale-high @16, track count @72, track block @76 (104-byte records: delay random
 * range floats @16/@20). */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/looping_sound_datum.h"
#include "headers/looping_sound_definition.h"
#include "headers/looping_sound_detail.h"
#include "headers/global_tag_instances.h"
#include "headers/sound_manager_globals.h"
#include "headers/sound_source.h"
#include "headers/looping_sound_refresh_state.h"
#include "headers/blam_data_globals.h"


extern int datum_new(data_array *data);
extern uint32_t *get_global_local_random_seed_address(void);
extern float real_seed_random_range(uint32_t *seed, float lower_bound, float upper_bound);

uint16_t looping_sound_new(int definition_index, int identifier, const sound_source *source)
{
    int index;
    looping_sound_datum *datum;
    looping_sound_definition *definition;
    int track;

    if ( !sound_manager_globals.initialized
      || !sound_manager_globals.active
      || sound_manager_globals.disable_from_commandline )
        return -1;

    index = datum_new(looping_sound_data);
    if ( index == -1 )
        return -1;

    datum = DATA_ARRAY_ELEMENT(looping_sound_data, looping_sound_datum, index);
    definition = TAG_GET(looping_sound_definition, definition_index);
    datum->definition_index = definition_index;
    datum->loop_identifier = identifier;
    datum->state = _looping_sound_refresh_start;
    datum->ordered_permutations_finished = 0;

    for ( track = 0; track < definition->details.count; track = (__int16)(track + 1) )
    {
        float scale = source->scale;
        float scale_high = definition->scale_upper_bound.detail_period;
        float scale_low = definition->scale_lower_bound.detail_period;
        looping_sound_detail *detail =
            &((looping_sound_detail *)definition->details.address)[track];
        float delay_random = real_seed_random_range(get_global_local_random_seed_address(),
                                                    detail->period_lower_bound, detail->period_upper_bound);
        datum->details[track].next_play_time =
            (int)((((((scale_high - scale_low) * scale) + scale_low)
                         * delay_random) * 1000.0f) + (float)sound_manager_globals.render_time);
    }
    return index;
}
