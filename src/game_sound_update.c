/* game_sound_update @0x83714B80 — services the game's looping sounds. Roughly every 33 ms (one render
 * tick) it does a full pass: refreshes the sound environment and combined potentially-audible-set,
 * (re)starts or stops the scenario background loop, and for each looping sound either frees it (owner
 * object gone), skips it (owner not audible / out of PAS), or refreshes it as potentially audible.
 * Between full passes it runs a lightweight refresh that re-evaluates each looping sound's on/off state
 * from its object function value and whether it was updated last frame. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/game_looping_sound_flags.h"
#include "headers/game_looping_sound_state.h"
#include "headers/global_tag_instances.h"
#include "headers/location.h"
#include "headers/game_sound_globals.h"
#include "headers/game_looping_sound_datum.h"
#include "headers/looping_sound_definition.h"
#include "headers/object_header_datum.h"
#include "headers/object_flags.h"
#include "headers/object_type.h"
#include "headers/bit_vector.h"
#include "headers/blam_data_globals.h"


#include "headers/data_array.h"
extern uint32_t system_milliseconds(void);
extern void scenario_get_sound_environment(int *background_sound_index, int *sound_environment_tag, uint8_t *crossed_water_boundary);
extern void sound_manager_set_sound_environment(int environment_tag);
extern void compute_combined_pas(void);
extern int unattached_looping_sound_start(int definition_index, int source_object_index, float scale);
extern int data_next_index(const data_array *data, int16_t index);
extern void *object_try_and_get_and_verify_type(int object_index, unsigned int valid_type_flags);
extern void datum_delete(data_array *data, int index);
extern location *object_get_location(int object_index, location *location_out);
extern uint8_t object_get_function_value(int object_index, int16_t function_index, float *value_reference);
extern void update_potentially_audible_looping_sound(int sound_index, const location *location);
extern void sound_refresh_looping_not(int identifier, uint8_t alternate);

void game_sound_update(float dt)  /* caller passes f1=dt (single); body ignores it */
{
    int i;

    if ( system_milliseconds() - game_sound_globals->render_time >= 0x21 )
    {
        int background_def;
        int environment;
        uint8_t out3[4];
        int index;

        scenario_get_sound_environment(&background_def, &environment, out3);
        sound_manager_set_sound_environment(environment);
        compute_combined_pas();

        if ( background_def == -1 )
        {
            if ( game_sound_globals->background_loop_index != -1 )
            {
                game_looping_sound_datum *loop =
                    DATA_ARRAY_ELEMENT(game_looping_sound_data, game_looping_sound_datum,
                                       game_sound_globals->background_loop_index);
                loop->flags |= (1u << _game_looping_sound_unattached_stop_bit);
                game_sound_globals->background_loop_index = -1;
            }
        }
        else
        {
            float gain;
            int start_def = background_def;
            int restart = 1;
            if ( game_sound_globals->background_loop_index == -1 )
            {
                gain = 1.0f;
            }
            else
            {
                game_looping_sound_datum *loop =
                    DATA_ARRAY_ELEMENT(game_looping_sound_data, game_looping_sound_datum,
                                       game_sound_globals->background_loop_index);
                if ( loop->definition_index == background_def )
                    restart = 0;                       /* same background loop already playing */
                else
                {
                    loop->flags |= (1u << _game_looping_sound_unattached_stop_bit);
                    gain = 1.0f;
                }
            }
            if ( restart )
                game_sound_globals->background_loop_index =
                    unattached_looping_sound_start(start_def, -1, gain);
        }

        for ( index = data_next_index(game_looping_sound_data, -1); index != -1;
              index = data_next_index(game_looping_sound_data, index) )
        {
            game_looping_sound_datum *sound =
                DATA_ARRAY_ELEMENT(game_looping_sound_data, game_looping_sound_datum, index);
            int object_index = sound->object_index;
            const location *audible_location;
            location location_storage[2];

            if ( object_index == -1 )
            {
                audible_location = (const location *)0;
            }
            else
            {
                object_datum *object;
                if ( (sound->flags & (1u << _game_looping_sound_unattached_bit)) != 0 && !object_try_and_get_and_verify_type(object_index, object_mask_all) )
                {
                    looping_sound_definition *definition =
                        TAG_GET(looping_sound_definition, sound->definition_index);
                    if ( definition->runtime_scripting_sound_index == index )
                        definition->runtime_scripting_sound_index = -1;
                    datum_delete(game_looping_sound_data, index);
                    continue;
                }
                object = DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, object_index)->datum;
                if ( (object->object.flags & (1u << _object_connected_to_map_bit)) == 0 )  /* object not flagged audible */
                    continue;
                object_get_location(object_index, location_storage);
                if ( location_storage[0].cluster_index == -1
                  || !BIT_VECTOR_TEST_FLAG(combined_pas, location_storage[0].cluster_index) )
                    continue;
                audible_location = location_storage;
            }
            update_potentially_audible_looping_sound(index, audible_location);
        }

        ++game_sound_globals->frame_index;
        game_sound_globals->render_time = system_milliseconds();
        return;
    }

    /* lightweight inter-frame refresh */
    for ( i = data_next_index(game_looping_sound_data, -1); i != -1;
          i = data_next_index(game_looping_sound_data, i) )
    {
        game_looping_sound_datum *sound =
            DATA_ARRAY_ELEMENT(game_looping_sound_data, game_looping_sound_datum, i);
        int last_frame = sound->last_audible_frame_index;
        char updated_recently = (last_frame == -1 || last_frame == game_sound_globals->frame_index - 1);
        int flags = sound->flags;
        unsigned int on;

        if ( (flags & (1u << _game_looping_sound_unattached_bit)) != 0 )
            on = ((unsigned int)~flags >> _game_looping_sound_unattached_stop_bit) & 1;  /* on = !unattached_stop */
        else
        {
            float function_value;
            on = object_get_function_value(sound->object_index,
                                                            sound->attachment.function_index,
                                                            &function_value);
        }

        if ( (uint8_t)on || (sound->state != _game_looping_sound_inactive && updated_recently) )
            sound_refresh_looping_not(i, (sound->flags & (1u << _game_looping_sound_alternate_bit)) != 0);
    }
}
