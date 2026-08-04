#ifndef WEATHER_PARTICLE_TYPE_FLAGS_H
#define WEATHER_PARTICLE_TYPE_FLAGS_H

/* weather_particle_type_flags — bit indices for the weather_particle_type flags word.
 * DB-verified: types_enum_values _09BE6EC1240FD4B30B1868EBD141C577 carries these named bit
 * indices verbatim. Values are BIT INDICES — use as (1u << _bit). */
enum weather_particle_type_flags {
	_weather_particle_type_random_rotation_bit = 2,
	NUMBER_OF_WEATHER_PARTICLE_TYPE_FLAGS = 3,
};

#endif /* WEATHER_PARTICLE_TYPE_FLAGS_H */

// weather_particle_type_flags: DB-verified via types_enum_values _09BE6EC1240FD4B30B1868EBD141C577
