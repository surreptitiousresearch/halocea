/* local_player_is_first_person @0x837EB2B8 — whether a local player's view is first-person. True when the
 * director perspective is 0 (first-person camera). When the camera is not perspective 0, it is still
 * treated as first-person if the player's unit is seated in a parent unit whose seat record has flag 0x8
 * set in its unit_seat flags.
 *
 * DEVIATION from prior draft: unit +284/+752 are object.parent_object_index / unit.parent_seat_index
 * (DB types_members), not a held-weapon index — the tag block indexed by 284-byte records is the parent
 * unit definition's seats block. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/unit_datum.h"
#include "headers/player_datum.h"
#include "headers/unit_definition.h"
#include "headers/unit_seat.h"
#include "headers/unit_seat_flags.h"
#include "headers/director_perspective.h"
#include "headers/global_tag_instances.h"
#include "headers/blam_data_globals.h"


extern int16_t director_get_perspective(int16_t local_player_index);
extern int local_player_get_player_index(int16_t local_player_index);

uint8_t local_player_is_first_person(int16_t local_player_index)
{
    int16_t perspective = director_get_perspective(local_player_index);
    if ( perspective == _director_perspective_first_person )
        return 1;

    player_datum *player =
        DATA_ARRAY_ELEMENT(player_data, player_datum, local_player_get_player_index(local_player_index));
    if ( player->unit_index == -1 )
        return 0;

    unit_datum *unit = (unit_datum *)
        DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, player->unit_index)->datum;
    int parent_object_index = unit->object.parent_object_index;
    if ( parent_object_index == -1 )
        return 0;

    int16_t parent_seat_index = unit->unit.parent_seat_index;
    if ( parent_seat_index == -1 )
        return 0;

    unit_datum *parent_unit = (unit_datum *)
        DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, parent_object_index)->datum;
    unit_definition *parent_definition =
        TAG_GET(unit_definition, parent_unit->definition_index);
    unit_seat *seat = (unit_seat *)parent_definition->unit.seats.address + parent_seat_index;

    return (seat->flags & (1u << _unit_seat_is_gunner_bit)) != 0;
}
