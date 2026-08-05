/* vehicle_scripting_find_available_seats @0x836CF120 — collect the indices of a vehicle's seats that match
 * an optional label substring and a desired-seat-type filter, and that are currently empty. Fills up to
 * seat_max_indices entries into seat_indices and returns how many were found.
 *
 * seat_desire_type (only applied when 0..3): 0 = any non-driver seat, 1 = seats flagged "AI can enter",
 * 2 = seats that are neither driver nor gunner, 3 = driver seats. A negative/large value disables the
 * filter (every labeled, empty seat qualifies). */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/object_datum.h"
#include "headers/vehicle_definition.h"
#include "headers/unit_seat.h"
#include "headers/global_tag_instances.h"
#include "headers/unit_seat_flags.h"
#include "headers/blam_data_globals.h"
#include "headers/ai_atom_vehicle_modifier.h"

extern char *strlwr(char *string);
extern char *strstr(const char *haystack, const char *needle);
extern uint8_t unit_seat_filled(int parent_object_index, int16_t seat_index);

int vehicle_scripting_find_available_seats(int unit_index, const char *seat_substring_name,
                                           int16_t seat_desire_type, int16_t *seat_indices, int16_t seat_max_indices)
{
    object_datum *object = DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum;
    vehicle_definition *definition = TAG_GET(vehicle_definition, object->definition_index);

    /* No filter substring (null or empty) => accept all labels. */
    char accept_all_labels = 0;
    if ( !seat_substring_name )
    {
        accept_all_labels = 1;
    }
    else
    {
        const char *p = seat_substring_name;
        while ( *p++ )
            ;
        if ( p - seat_substring_name == 1 )   /* empty string */
            accept_all_labels = 1;
    }

    int found = 0;
    int seat_count = definition->unit.seats.count;
    if ( seat_count > 0 )
    {
        unit_seat *seats = (unit_seat *)definition->unit.seats.address;
        int seat_index = 0;
        do
        {
            int output_slot = (int16_t)found;
            unit_seat *seat = &seats[seat_index];
            if ( (int16_t)found >= seat_max_indices )
                break;

            /* Lowercase a copy of the seat's label for case-insensitive substring matching. */
            char label[368];
            char *src = seat->label;   /* seat label at +4 */
            int i = 0;
            char c;
            do
            {
                c = src[i];
                label[i] = c;
                ++i;
            }
            while ( c );
            strlwr(label);

            if ( accept_all_labels || strstr(label, seat_substring_name) )
            {
                unsigned int qualifies = 1;
                if ( (unsigned int)seat_desire_type <= _ai_atom_vehicle_modifier_driver )
                {
                    if ( seat_desire_type == _ai_atom_vehicle_modifier_gunner )
                        qualifies = (seat->flags >> _unit_seat_is_gunner_bit) & 1;                 /* AI-enterable */
                    else if ( seat_desire_type == _ai_atom_vehicle_modifier_passenger )
                        qualifies = ((seat->flags & (1u << _unit_seat_is_driver_bit)) == 0 && (seat->flags & (1u << _unit_seat_is_gunner_bit)) == 0);
                    else if ( seat_desire_type == _ai_atom_vehicle_modifier_driver )
                        qualifies = (seat->flags >> _unit_seat_is_driver_bit) & 1;                 /* driver */
                    else
                        qualifies = (~seat->flags >> _unit_seat_is_driver_bit) & 1;               /* non-driver */
                }
                if ( (unsigned char)qualifies && !unit_seat_filled(unit_index, seat_index) )
                {
                    found = (int16_t)(found + 1);
                    seat_indices[output_slot] = seat_index;
                }
            }
            seat_index = (int16_t)(seat_index + 1);
        }
        while ( seat_index < seat_count );
    }
    return found;
}
