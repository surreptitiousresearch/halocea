/* motion_sensor_update @0x837BCE98 — advance the radar/motion-sensor state by one tick. Rotates the
 * active sensor-history slot (mod 10). On ticks that aren't a 15-tick boundary, just shifts each local
 * player's previous-slot snapshot into the new slot (cheap). On a 15-tick boundary (a full rescan),
 * resets every local player's current sensor datum (blips off, count 0), snapshots each local player's
 * camera position, then walks every unit-type object via `object_iterator`, and for objects that are
 * visible/drawable and pass the per-player admission test, records a blip (type via `blip_type_get`,
 * size from the object's own vehicle tag if it is itself a vehicle) into that player's current sensor
 * datum.
 *
 * FAITHFUL QUIRK: the admission test is NOT simply "within `motion_sensor_range`". `game_engine_running`
 * short-circuits it: `bl game_engine_running` @0x837BD254 then `bne cr6, loc_837BD2A4` @0x837BD260 jumps
 * over the entire squared-distance computation (0x837BD264-0x837BD2A0). Whenever a game engine is running
 * — i.e. in multiplayer — every drawable unit blips regardless of `motion_sensor_range`; the range test
 * only runs in single-player. Reproduced as the `game_engine_running() || ...` disjunction below.
 *
 * DEVIATION: the decompiler renders the per-tick stack scratch as one bogus `object_iterator v53[2]`
 * (an array of TWO iterators). Only `v53[0]` is ever used as a real iterator; `v53[1]` is decompiler
 * noise from a mis-sized adjacent local — the real second local, confirmed via disasm address math
 * (`&v53[1].type_flags`/`.flags`/`.index` land exactly 0/4/8 bytes into it, each then offset by
 * `12*player_slot`), is a `real_point3d camera_position[2]` sitting immediately after the iterator on
 * the stack. Rewritten as two separate locals below.
 *
 * FAITHFUL QUIRK: in the per-object range check, the "Z" term of the squared-distance sum
 * (`camera_position[player].z - camera_position[player].z`) reads the SAME address for both operands —
 * confirmed via disasm, not a transcription slip — so it always evaluates to 0.0 and the range check is
 * effectively 2D (X/Y only). The object's own Z position is computed (`object_position.z`) but never
 * consulted anywhere in the function; kept as a dead read for fidelity.
 *
 * DEVIATION: attested void — the decompiler's `void *` return is the r3 left over from the loop-exit
 * `bl object_iterator_next` (837BD3B4) surviving the epilogue; it is always the NULL that ended the walk,
 * and both callers (motion_sensor_tick @0x837BD7AC/0x837BD7D0) discard r3. */

#include <stdint.h>
#include <string.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/object_datum.h"
#include "headers/player_datum.h"
#include "headers/motion_sensor_globals.h"
#include "headers/object_iterator.h"
#include "headers/hud_globals.h"
#include "headers/global_tag_instances.h"
#include "headers/unit_definition.h"
#include "headers/real_point3d.h"
#include "headers/object_damage_flags.h"
#include "headers/object_type.h"
#include "headers/blip_type.h"
#include "headers/blam_data_globals.h"


extern uint8_t game_engine_running(void);
extern int game_time_get(void);
extern int16_t local_player_count(void);
extern int16_t local_player_get_next(int16_t local_player_index);
extern int local_player_get_player_index(int16_t local_player_index);
extern void unit_get_camera_position(int unit_index, real_point3d *camera_position);
extern void object_iterator_new(object_iterator *iterator, uint32_t type_flags, uint8_t flags);
extern void *object_iterator_next(object_iterator *iterator);
extern void *object_try_and_get_and_verify_type(int object_index, unsigned int valid_type_flags);
extern uint8_t should_draw_object(int object_index);
extern uint8_t blip_type_get(int object_index, int local_player_index);

void motion_sensor_update(void)
{
    game_engine_running();
    int current_time = game_time_get();

    motion_sensor_globals->update = 1;
    motion_sensor_globals->last_update_time = current_time;

    int16_t new_active_index = (motion_sensor_globals->sensor_active_index + 1) % 10;
    motion_sensor_globals->sensor_active_index = new_active_index;

    uint8_t done = 0;

    if (current_time % 15 && current_time)
    {
        /* shift tick: duplicate the previous slot's snapshot into the new slot for every active player */
        int16_t old_active_index = (new_active_index + 9) % 10;
        int16_t player_count = local_player_count();
        int16_t next = local_player_get_next(-1);

        for (int16_t i = 0; i < player_count; ++i)
        {
            /* recovered: (char*)&motion_sensors[next] + 132*idx -> .sensor_data[idx] (stride = sizeof(motion_sensor_datum)) */
            memcpy(&motion_sensor_globals->motion_sensors[next].sensor_data[new_active_index],
                  &motion_sensor_globals->motion_sensors[next].sensor_data[old_active_index],
                  sizeof(motion_sensor_datum));
            next = local_player_get_next(next);
        }
    }
    else
    {
        /* rescan tick */
        int16_t local_player_slots[4]; /* [0..1] = per-local-player blip count this pass, [2..3] = local
                                           player index found at each scan position */
        local_player_slots[0] = 0;
        local_player_slots[1] = 0;

        int16_t player_count = local_player_count();
        int16_t next = local_player_get_next(-1);

        object_iterator iter;
        real_point3d camera_position[2];

        if (player_count > 0)
        {
            for (int16_t i = 0; i < player_count; ++i)
            {
                motion_sensor_datum *current_datum =
                    &motion_sensor_globals->motion_sensors[next].sensor_data[motion_sensor_globals->sensor_active_index];

                int player_index = local_player_get_player_index(next);
                if (player_index != -1)
                    player_index = DATA_ARRAY_ELEMENT(player_data, player_datum,
                        local_player_get_player_index(next))->unit_index;

                camera_position[next].x = 0.0f;
                local_player_slots[i + 2] = next;
                camera_position[next].y = 0.0f;
                camera_position[next].z = 0.0f;
                if (player_index != -1)
                    unit_get_camera_position(player_index, &camera_position[next]);

                current_datum->num_blips_active = 0;
                for (int blip_index = 0; blip_index < 16; ++blip_index)
                    current_datum->blips[blip_index].type = _blip_type_none;

                next = local_player_get_next(next);
            }
        }

        object_iterator_new(&iter, object_mask_unit, 1u);
        for (void *object_entry = object_iterator_next(&iter); object_entry; object_entry = object_iterator_next(&iter))
        {
            if (done)
                break;

            object_datum *vehicle = object_try_and_get_and_verify_type(iter.index, object_mask_unit);
            uint8_t passes_flag_check = 0;
            if (vehicle)
                passes_flag_check = (vehicle->object.damage_flags & (1u << _object_dead_bit)) == 0;

            if (passes_flag_check && should_draw_object(iter.index))
            {
                int players_full = 0;
                object_datum *object = DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, iter.index)->datum;

                real_point3d object_position;
                object_position.x = object->object.bounding_sphere_center.n[0];
                object_position.y = object->object.bounding_sphere_center.n[1];
                object_position.z = object->object.bounding_sphere_center.n[2]; /* dead read: never consulted below */

                if (player_count > 0)
                {
                    for (int scan = 0; scan < player_count; ++scan)
                    {
                        int16_t scan_player = local_player_slots[scan + 2];

                        if (local_player_get_player_index(scan_player) != -1
                            && DATA_ARRAY_ELEMENT(player_data, player_datum,
                                local_player_get_player_index(scan_player))->unit_index != -1)
                        {
                            int16_t blip_count = local_player_slots[scan_player];
                            if (blip_count >= 16)
                            {
                                ++players_full;
                            }
                            else
                            {
                                float dx = object_position.x - camera_position[scan_player].x;
                                float dy = object_position.y - camera_position[scan_player].y;
                                float dz = camera_position[scan_player].z - camera_position[scan_player].z;

                                if (game_engine_running()
                                    || (double)(dx * dx + dz * dz + dy * dy)
                                        <= (double)(hud_globals->defaults.motion_sensor_range * hud_globals->defaults.motion_sensor_range))
                                {
                                    motion_sensor_player_datum *player_sensors = &motion_sensor_globals->motion_sensors[scan_player];
                                    motion_sensor_datum *sensor_datum = &player_sensors->sensor_data[motion_sensor_globals->sensor_active_index];

                                    char blip_type = blip_type_get(iter.index, scan_player);
                                    motion_sensor_blip *blip = &sensor_datum->blips[blip_count];
                                    blip->type = blip_type;

                                    int16_t blip_size; /* DEVIATION: was uint8_t; binary range-checks the SIGNED halfword (lhz+extsh @0x837BD32C-30, cmpwi 0 AND cmpwi 3 @0x837BD334-40) */
                                    if (iter.index == -1 || !object_try_and_get_and_verify_type(iter.index, object_mask_unit))
                                    {
                                        blip_size = 0;
                                    }
                                    else
                                    {
                                        int vehicle_tag_index = object->definition_index;
                                        unit_definition *vehicle_definition = TAG_GET(unit_definition, vehicle_tag_index);
                                        blip_size = vehicle_definition->unit.blip_type; /* hud_blip_type, int16_t */
                                        if (blip_size < 0 || blip_size >= NUMBER_OF_HUD_BLIP_TYPES)
                                            blip_size = _hud_blip_type_medium;
                                    }
                                    blip->size = blip_size;

                                    player_sensors->current_unit_indices[blip_count] = iter.index;
                                    sensor_datum->num_blips_active++;
                                    local_player_slots[scan_player] = blip_count + 1;
                                }
                            }
                        }
                    }
                }

                if (players_full == player_count)
                    done = 1;
            }
        }
    }
}
