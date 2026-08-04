/* sound_reconnect_to_structure_bsp @0x837151F0 — after a structure BSP switch, re-resolve every active
 * sound's cluster/leaf location from its world position: for each live sound (176-byte records) and
 * looping sound (228-byte records) whose "connected to structure" state word is 1, recompute its cached
 * location from its position. No-op unless sound is initialized, active, and not disabled from the
 * command line. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/sound_manager_globals.h"
#include "headers/location.h"
#include "headers/real_point3d.h"
#include "headers/sound_datum.h"
#include "headers/looping_sound_datum.h"
#include "headers/spatialization_mode.h"
#include "headers/blam_data_globals.h"


#include "headers/data_array.h"
extern int data_next_index(const data_array *data, int16_t index);
extern void scenario_location_from_point(location *location, const real_point3d *point);

void sound_reconnect_to_structure_bsp(void)
{
    if ( !sound_manager_globals.initialized || !sound_manager_globals.active
      || sound_manager_globals.disable_from_commandline )
    {
        return;
    }

    for ( int i = data_next_index(sound_data, -1); i != -1; i = data_next_index(sound_data, i) )
    {
        sound_datum *sound = DATA_ARRAY_ELEMENT(sound_data, sound_datum, i);
        if ( sound->source.spatialization_mode == _sound_spatialization_mode_absolute )
            scenario_location_from_point(&sound->source.location.game_location, &sound->source.location.position);
    }

    for ( int j = data_next_index(looping_sound_data, -1); j != -1; j = data_next_index(looping_sound_data, j) )
    {
        looping_sound_datum *looping_sound =
            DATA_ARRAY_ELEMENT(looping_sound_data, looping_sound_datum, j);
        if ( looping_sound->source.spatialization_mode == _sound_spatialization_mode_absolute )
            scenario_location_from_point(&looping_sound->source.location.game_location,
                                         &looping_sound->source.location.position);
    }
}
