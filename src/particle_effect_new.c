/* particle_effect_new @0x8373DB88 — spawn a secondary effect at a particle (its collision or death
 * effect). Branches on the referenced tag group: an effect tag ('effe') is spawned unattached from the
 * particle's markers, oriented along the particle's facing; a sound tag ('snd!') plays an unattached
 * impulse sound at the particle. The particle's velocity is converted from per-tick to per-second
 * (×1/30) for the spawned effect.
 *
 * group_tag 1701209701 = 'effe', 1936614433 = 'snd!'. */

#include <stdint.h>
#include "headers/particle_datum.h"
#include "headers/sound_source.h"     /* sound_location */
#include "headers/real_vector3d.h"
#include "headers/real_point3d.h"
#include "headers/real_rgb_color.h"
#include "headers/effect_vector_field.h"
#include "headers/location.h"
#include "headers/blam_data_globals.h"
#include "headers/game_time_constants.h"


extern float normalize3d(real_vector3d *v);
extern int effect_new_unattached_from_markers(int definition_index, int owner_object_index, const real_vector3d *translational_velocity, int16_t marker_count, const char **marker_names, real_point3d *marker_points, real_vector3d *marker_forwards, float scale_a, float scale_b, const real_rgb_color *color, const effect_vector_field *impulse_field, uint8_t deterministic);
extern int unattached_impulse_sound_new(int definition_index, const sound_location *location, float scale, uint8_t is_player);

/* Deviation: scale is single-precision (float), not double — the decompiler widens f1. It is forwarded
 * without any frsp to two float-scale callees (effect_new_unattached_from_markers scale_a and
 * unattached_impulse_sound_new scale), proving the incoming value is already single-precision. */
void particle_effect_new(particle_datum *particle, unsigned int group_tag, int effect_index, float scale)
{
    real_vector3d velocity_per_second;
    sound_location effect_location;

    velocity_per_second.n[0] = particle->translational_velocity.n[0] * SECONDS_PER_TICK;
    velocity_per_second.n[1] = (particle->translational_velocity.n[1] * SECONDS_PER_TICK);
    velocity_per_second.n[2] = (particle->translational_velocity.n[2] * SECONDS_PER_TICK);

    if ( group_tag == 0x65666665u /* 'effe' */ )
    {
        real_point3d  marker_points[2];
        real_vector3d marker_forwards[2];
        /* DEVIATION: color is NULL, not a pointer to a stack real_rgb_color. The binary stores 0 to the
         * color slot (`li r11, 0` @0x8373DC5C, `stw r11, 0x5C(r1)` @0x8373DC80 — the slot the callee reads
         * as arg_5C @0x836E42AC), and the *global_down3d copy Hex-Rays aliased onto that argument is really
         * marker_forwards[1], the "gravity" marker: its three stores (var_74/70/6C @0x8373DC40-48) sit
         * directly above marker_forwards[0] (var_80/7C/78), and `addi r9, r1, var_80` @0x8373DC64 hands the
         * pair over. marker_count is 2, so the old single `facing` also under-sized the forwards buffer. */
        marker_forwards[0].n[0] = particle->direction.n[0];
        marker_forwards[0].n[1] = particle->direction.n[1];
        marker_forwards[0].n[2] = particle->direction.n[2];

        marker_points[1].n[0] = particle->position.n[0];
        marker_points[1].n[1] = particle->position.n[1];
        marker_points[1].n[2] = particle->position.n[2];
        marker_points[0].n[0] = particle->position.n[0];
        marker_points[0].n[2] = particle->position.n[2];
        marker_points[0].n[1] = particle->position.n[1];

        marker_forwards[1].n[0] = global_down3d->n[0];
        marker_forwards[1].n[1] = global_down3d->n[1];
        marker_forwards[1].n[2] = global_down3d->n[2];

        normalize3d(&marker_forwards[0]);
        effect_new_unattached_from_markers(
            effect_index,
            -1,
            &velocity_per_second,
            2,
            particle_effect_marker_names,
            marker_points,
            marker_forwards,
            scale,
            0.0f,
            NULL,    /* color */
            NULL,    /* impulse_field */
            0);      /* deterministic */
    }
    else if ( group_tag == 0x736E6421u /* 'snd!' */ )
    {
        location game_location = particle->location;
        game_location.leaf_index = (int)&effect_location;   /* decompiler artifact: reuses the stack slot */

        effect_location.position.n[0] = particle->position.n[0];
        effect_location.position.n[1] = particle->position.n[1];
        effect_location.position.n[2] = particle->position.n[2];

        effect_location.forward.n[0] = global_forward3d->n[0];
        effect_location.translational_velocity = velocity_per_second; /* both real_vector3d; dropped spurious sound_point3d cast */
        effect_location.forward.n[1] = global_forward3d->n[1];
        effect_location.forward.n[2] = global_forward3d->n[2];
        effect_location.game_location = game_location;

        unattached_impulse_sound_new(effect_index, &effect_location, scale, game_location.bonus);
    }
}
