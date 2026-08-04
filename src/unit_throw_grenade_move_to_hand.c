/* unit_throw_grenade_move_to_hand @0x836CB698 — during a grenade-throw animation, spawn the thrown grenade
 * object into the unit's left hand. First it consumes a grenade of the current type from the unit's count
 * (unless ammo/grenades are unlimited via cheat, game engine, or actor). Then, on the host/local side
 * (not a pure network client), it creates the grenade object at the "left hand" marker, oriented along the
 * unit's aiming vector with a perpendicular up vector, attaches it to the marker node, and records it as
 * the unit's held grenade (state 2). On a client it just records the held state with no object.
 *
 * DEVIATION: object_marker.matrix is a real_matrix4x3 whose translation row is n[3]; the decompiler names
 * that row .position. The placement position is copied from there. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/unit_datum.h"
#include "headers/projectile_datum.h"
#include "headers/game_globals_grenade.h"
#include "headers/game_globals_tag.h"
#include "headers/object_placement_data.h"
#include "headers/object_marker.h"
#include "headers/real_vector3d.h"
#include "headers/real_point3d.h"
#include "headers/cheats.h"
#include "headers/networked_datum_role.h"
#include "headers/game_connection.h"
#include "headers/unit_grenade_throw_state.h"
#include "headers/blam_data_globals.h"
extern float __fsqrts(float);


extern uint8_t game_engine_infinite_grenades(int player_index);
extern uint8_t actor_has_unlimited_grenades(int actor_index);
extern int16_t game_connection(void);
extern int16_t object_get_marker_by_name(int object_index, const char *name, object_marker *markers, int16_t maximum_marker_count);
extern void object_placement_data_new(object_placement_data *data, int definition_index, int owner_object_index);
extern real_vector3d *perpendicular3d(const real_vector3d *a, real_vector3d *result);
extern int object_new_with_datum_role_control(object_placement_data *data, NetworkedDatumRole datum_role);
extern void hcex_obj_set_state(int objId, const char *stateName);
extern void object_attach_to_node(int parent_object_index, int child_object_index, int16_t parent_node_index);

void unit_throw_grenade_move_to_hand(int unit_index)
{
    unit_datum *unit_object = (unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum;
    int controlling_player = unit_object->unit.player_index;
    game_globals_grenade *grenade =
        &((game_globals_grenade *)global_game_globals->grenades.address)[unit_object->unit.current_grenade_index];

    /* Consume a grenade unless ammo is unlimited. */
    if (controlling_player == -1 || (!cheat.infinite_ammo && !game_engine_infinite_grenades(controlling_player)))
    {
        int actor_index = unit_object->unit.actor_index;
        if (actor_index == -1 || !actor_has_unlimited_grenades(actor_index))
        {
            /* +4 is object.datum_role (network ownership), not an object type */
            int datum_role = unit_object->object.datum_role;
            if (datum_role == _networked_datum_autonomous || datum_role == _networked_datum_master)
                --*(unsigned char *)&unit_object->unit.grenade_counts[unit_object->unit.current_grenade_index];
        }
    }

    __int16 connection = game_connection();
    if (connection != _game_connection_network_server
        && connection != _game_connection_local
        && connection != _game_connection_film_playback)
    {
        /* Pure network client: record the held state only. */
        unit_object->unit.grenade_object_index = -1;
        unit_object->unit.grenade_throw_state = _grenade_throw_state_in_hand;
        return;
    }

    object_marker marker;
    object_get_marker_by_name(unit_index, "left hand", &marker, 1);

    object_placement_data placement;
    object_placement_data_new(&placement, grenade->projectile.index, unit_index);
    placement.flags |= 2u;

    placement.forward.n[0] = unit_object->unit.aiming_vector.n[0];
    placement.forward.n[1] = unit_object->unit.aiming_vector.n[1];
    placement.forward.n[2] = unit_object->unit.aiming_vector.n[2];

    real_vector3d *up = perpendicular3d(&placement.forward, &placement.up);
    float length = __fsqrts(up->n[2] * up->n[2] + (up->n[0] * up->n[0] + up->n[1] * up->n[1]));
    if (__fabs(length) >= 0.000099999997f)
    {
        float inverse_length = 1.0f / length;
        up->n[0] = up->n[0] * inverse_length;
        up->n[1] = up->n[1] * inverse_length;
        up->n[2] = up->n[2] * inverse_length;
    }

    placement.position = *(real_point3d *)marker.matrix.n[3];

    int grenade_object = object_new_with_datum_role_control(&placement, _networked_datum_autonomous);
    if (grenade_object == -1)
    {
        unit_object->unit.grenade_throw_state = _grenade_throw_state_released;
    }
    else
    {
        hcex_obj_set_state(grenade_object, "throw");
        object_attach_to_node(unit_index, grenade_object, marker.node_index);
        unit_object->unit.grenade_object_index = grenade_object;
        unit_object->unit.grenade_throw_state = _grenade_throw_state_in_hand;
        ((projectile_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum,
            grenade_object)->datum)->projectile.replicate_detonation = 1;
    }
}
