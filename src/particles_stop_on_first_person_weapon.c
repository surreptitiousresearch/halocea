/* particles_stop_on_first_person_weapon @0x8373D9D8 — delete every active particle owned by a given local
 * player's first-person weapon. Walks the particle data array (112-byte stride); a particle qualifies when
 * its local_player_index matches, its "first-person" flag (_particle_attached_to_local_player_bit) is set, and its attached
 * object_index is valid. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/particle_datum.h"
#include "headers/particle_flags.h"
#include "headers/blam_data_globals.h"


#include "headers/data_array.h"
extern int data_next_index(const data_array *data, int16_t index);
extern void datum_delete(data_array *data, int index);

void particles_stop_on_first_person_weapon(int16_t local_player_index)
{
    for ( int i = data_next_index(particle_data, -1); i != -1; i = data_next_index(particle_data, i) )
    {
        particle_datum *particle = DATA_ARRAY_ELEMENT(particle_data, particle_datum, i);
        if ( particle->local_player_index == local_player_index
             && (particle->flags & (1u << _particle_attached_to_local_player_bit)) != 0
             && particle->object_index != -1 )
        {
            datum_delete(particle_data, i);
        }
    }
}
