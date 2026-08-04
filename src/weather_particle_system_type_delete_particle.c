#include "headers/data_array.h"
#include "headers/weather_particle_system_globals.h"
#include "headers/weather_particle_datum.h"
#include "headers/blam_data_globals.h"

extern void datum_delete(data_array *data, int index);

void weather_particle_system_type_delete_particle(weather_particle_type_datum *type)
{
    weather_particle_datum *particle =
        (weather_particle_datum *)weather_particle_data->data + (unsigned __int16)type->first_particle_index;
    int next_particle_index = particle->next_particle_index;

    datum_delete(weather_particle_data, type->first_particle_index);
    type->first_particle_index = next_particle_index;
    type->particle_count--;
}
