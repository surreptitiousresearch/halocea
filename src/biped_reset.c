/* biped_reset @0x837AC798 — clears a biped's 132-byte _biped_datum block (+1228), resets its ground plane
 * to a sentinel "no ground" plane (`depths_of_hell`), and clears its last-falling-communication timer
 * (+1272) to -1 (none). */

#include <string.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/biped_datum.h"
#include "headers/real_plane3d.h"
#include "headers/blam_data_globals.h"


void biped_reset(int biped_index)
{
    biped_datum *biped_object =
        (biped_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, biped_index)->datum;

    memset(&biped_object->biped, 0, 0x84u);
    biped_object->biped.ground_plane = depths_of_hell;
    biped_object->biped.last_falling_communication_time = -1;
}
