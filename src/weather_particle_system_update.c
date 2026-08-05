/* weather_particle_system_update @0x8373CD20 — advance one weather particle system by a frame. Accumulates the
 * system clock, then for each particle type in the weather definition: computes a camera-height fade (a near fade
 * that scales the requested density and a far fade that attenuates it), asks
 * weather_particle_system_update_particle_count to (de)spawn particles toward that target, then walks the type's
 * live particle list advancing each particle's animation phase (wrapped by fmod against the type's period) and its
 * lateral sway (a per-particle ± offset alternating by particle-index parity), and runs its physics.
 *
 * Tag-definition internals (weather def particle-type array @ +40, stride 604; per-type fade heights @ +52/+56
 * near and +60/+64 far; particle-type tag index @ +416; the type tag's period table @ +88) and the 84-byte
 * particle datum fields (phase index @+40, phase @+44, sway @+48, sway amplitude @+72, phase velocity @+76, next
 * index @+80) are read at raw offsets per the corpus convention. DEVIATION: the fmod dividend/divisor the
 * decompiler punned into a two-double struct are (phase, (double)period) from disasm (0x8373CEF0-836CF00); the
 * ±1 sway sign `(i & 1) == 0 ? 1 : -1` was confirmed from the subfic/subfe/clrrwi/addi idiom (0x8373CF14). */

#include <stdint.h>
#include "headers/weather_particle_system_globals.h"
#include "headers/weather_particle_datum.h"
#include "headers/weather_particle_type_definition.h"
#include "headers/weather_particle_system_definition.h"
#include "headers/bitmap_group.h"
#include "headers/bitmap_group_sequence.h"
#include "headers/render_globals.h"
#include "headers/data_array.h"
#include "headers/global_tag_instances.h"
#include <math.h>
#include "headers/blam_data_globals.h"


extern void weather_particle_system_update_particle_count(int16_t system_index, int16_t type_index, float scale);
extern void weather_particle_update_physics(int16_t system_index, int16_t type_index, uint16_t particle_index);

static float weather_fade_fraction(float value, float start, float end)
{
    float fraction = (value - start) / (end - start);
    if ( fraction < 0.0f )
        return 0.0f;
    if ( fraction > 1.0f )
        return 1.0f;
    return fraction;
}

void weather_particle_system_update(int16_t system_index)
{
    weather_particle_system_datum *system = &weather_particle_system_globals.systems[system_index];
    weather_particle_system_definition *definition = TAG_GET(weather_particle_system_definition, system->definition_index);

    system->time += render.time_delta_since_frame_sec;
    system->time_delta_sec = render.time_delta_since_frame_sec;

    int type_count = definition->particle_types.count;
    for ( int type_index = 0; type_index < type_count; ++type_index )
    {
        weather_particle_type_definition *type_def = (weather_particle_type_definition *)definition->particle_types.address + type_index;
        bitmap_group *type_tag = TAG_GET(bitmap_group, type_def->bitmap.index);

        float camera_z = render.camera.position.n[2];
        float near_fade = weather_fade_fraction(camera_z, type_def->height_fadein_start, type_def->height_fadein_end);
        float scaled_density = near_fade * system->scale;
        float far_fade = weather_fade_fraction(camera_z, type_def->height_fadeout_start, type_def->height_fadeout_end);
        weather_particle_system_update_particle_count(system_index, type_index,
                (1.0f - far_fade) * scaled_density);

        int i = system->types[type_index].first_particle_index;
        while ( i != -1 )
        {
            weather_particle_datum *particle = DATA_ARRAY_ELEMENT(weather_particle_data, weather_particle_datum, i);

            int16_t phase_index = particle->sequence_index;
            float new_phase = particle->animation_rate * system->time_delta_sec + particle->sprite_index;
            /* period = sequence[phase_index].sprites.count (bitmap_group_sequence stride 64, sprites count@+52) */
            int period = ((bitmap_group_sequence *)type_tag->sequences.address)[phase_index].sprites.count;
            particle->sprite_index = (float)fmod(new_phase, (double)period);

            int sway_sign = (i & 1) == 0 ? 1 : -1;
            particle->rotation =
                    (float)sway_sign * particle->rotation_rate * system->time_delta_sec + particle->rotation;

            weather_particle_update_physics(system_index, type_index, i);

            i = particle->next_particle_index;
        }
    }
}
