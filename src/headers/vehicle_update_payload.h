#pragma once
/* _vehicle_update_payload — the incremental-mode wire payload for vehicle_build_update_delta.c /
 * vehicle_process_update_delta.c (64 bytes). Rebound to the DB payload type: it IS a
 * vehicle_datum_network_data (the same layout the vehicle's baseline / last_network_data snapshots
 * use). The formerly unmapped leading byte (bit 5 of object.flags) is the DB's at_rest_bit. */

#include "vehicle_datum_network_data.h"

typedef vehicle_datum_network_data _vehicle_update_payload;
