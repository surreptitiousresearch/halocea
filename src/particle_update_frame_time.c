/* particle_update_frame_time @0x8373E138 — advance a particle's animation frame timer over dt. For
 * frame-locked particles (definition _particle_animates_at_frame_time_bit) one frame is stepped per call;
 * otherwise the per-frame duration accumulator is wound down by dt, stepping the sequence frame
 * (particle_next_frame) each time it elapses. Returns nonzero while the particle is still alive.
 * (definition flags: _particle_animation_stops_at_rest_bit, _particle_animates_at_frame_time_bit)
 *
 * DEVIATION: the loop body in the decompiler output is a verbatim compiler-inlined copy of
 * particle_next_frame @0x8373E018 (its own DATA_ARRAY_ELEMENT re-fetch as first statement, then the
 * identical backwards-flag / bitmap-sequence rollover stepping); the two outer call sites in this same
 * function stayed real `bl particle_next_frame` calls. Emitted as the call the source had. The
 * decompiler's cached tag-table local (v6, reloaded after each call) and its two SSA particle pointers
 * (v5/v10) belong to that inlining and are gone with it. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/global_tag_instances.h"
#include "headers/particle_datum.h"
#include "headers/particle_definition.h"
#include "headers/particle_definition_flags.h"
#include "headers/particle_flags.h"
#include "headers/blam_data_globals.h"


extern uint8_t particle_next_frame(int particle_index);

uint8_t particle_update_frame_time(int particle_index, float dt)
{
    float remaining = dt;
    particle_datum *particle = DATA_ARRAY_ELEMENT(particle_data, particle_datum, particle_index);
    uint8_t result = 1;
    int definition_flags = TAG_GET(particle_definition, particle->definition_index)->flags;

    if ( (definition_flags & (1u << _particle_animation_stops_at_rest_bit)) != 0
      && (particle->flags & (1u << _particle_at_rest_bit)) != 0 )
        return result;

    if ( (definition_flags & (1u << _particle_animates_at_frame_time_bit)) != 0 )
    {
        if ( dt != 0.0f )
            return particle_next_frame(particle_index);
        return result;
    }

    if ( particle->frame_time == -1.0f )
    {
        result = particle_next_frame(particle_index);
        particle->frame_time = 0.0f;
    }

    if ( remaining <= 0.0f )
        return result;

    while ( result )
    {
        float frame_remaining = particle->frame_span - particle->frame_time;
        if ( frame_remaining > remaining )
        {
            particle->frame_time = particle->frame_time + remaining;
            return result;
        }

        result = particle_next_frame(particle_index);

        remaining = remaining - frame_remaining;
        if ( remaining <= 0.0f )
            return result;
    }

    return result;
}
