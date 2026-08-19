/* biped_update_jumping @0x837B2F48 — while not airborne (flags bit 0 at +1228) and not in unit-control
 * mode 1 (+1288), count up ticks spent in the current ground contact state (+1284, saturating at 127);
 * once flag bit 0x2 (+520) is set and the tick count exceeds 5, trigger biped_jump (see biped_jump.c for
 * the +1284 reset-to-0-on-jump counterpart). */

#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/biped_datum.h"
#include "headers/unit_animation_update_data.h"
#include "headers/unit_control_flags.h"
#include "headers/biped_datum_flags.h"
#include "headers/blam_data_globals.h"

extern int biped_jump(int biped_index);

void biped_update_jumping(int biped_index, unit_animation_update_data *animation)
{
    biped_datum *biped =
        (biped_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, biped_index)->datum;

    if ( (biped->biped.flags & (1u << _biped_airborne_bit)) == 0 && biped->biped.landing != 1 )
    {
        if ( biped->biped.jump_recovery_timer != 127 )
            ++biped->biped.jump_recovery_timer;

        if ( (biped->unit.control_flags & (1u << _unit_control_jump_bit)) != 0 && biped->biped.jump_recovery_timer > 5 )
            biped_jump(biped_index);
    }
}
