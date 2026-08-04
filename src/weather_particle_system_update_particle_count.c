/* weather_particle_system_update_particle_count @0x8373C8E8 — resize one weather particle type's live
 * particle count toward its target: target = max(0, (int)(density*box_width^3*scale / active_system_count)).
 * Spawns new particles while under target, or frees the oldest ones (walking the type's own free-list
 * head, first_particle_index) while over target.
 *
 * DEVIATION: the decompiler folds `type_index` and `scale` (2 real DB params) into one bogus `__int64
 * type_index`, and separately shows `weather_particle_system_globals.active_system_count` overwriting the
 * `type_index` local; disasm confirmed both — `active_system_count` (offset 0) is a genuine global field
 * read (not a phantom), it just happens to reuse the same decompiler-invented local as the garbled
 * type_index/scale pair. Reproduced using the real 3-param DB signature and the already-headered
 * weather_particle_system_globals fields directly. */

#include <stdint.h>
#include "headers/weather_particle_system_globals.h"
#include "headers/weather_particle_datum.h"
#include "headers/data_array.h"
#include "headers/blam_data_globals.h"


extern int weather_particle_system_new_particle(int16_t system_index, int16_t type_index);
extern void datum_delete(data_array *data, int index);

void weather_particle_system_update_particle_count(int16_t system_index, int16_t type_index, float scale)
{
    weather_particle_type_datum *type = &weather_particle_system_globals.systems[system_index].types[type_index];

    float raw_target = type->density * type->box_width * type->box_width * type->box_width * scale
                      / (float)weather_particle_system_globals.active_system_count;
    __int16 target_count = (int)raw_target < 0 ? 0 : (__int16)(int)raw_target;

    while ( type->particle_count < target_count )
    {
        if ( weather_particle_system_new_particle(system_index, type_index) == -1 )
            break;
    }

    while ( type->particle_count > target_count )
    {
        weather_particle_datum *particle =
            (weather_particle_datum *)weather_particle_data->data + (unsigned __int16)type->first_particle_index;
        int next_free_particle_index = particle->next_particle_index;
        datum_delete(weather_particle_data, type->first_particle_index);
        type->first_particle_index = next_free_particle_index;
        type->particle_count = type->particle_count - 1;
    }
}
