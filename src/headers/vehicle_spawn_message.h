#pragma once
/* _vehicle_spawn_message — the by-ref decode destination for a vehicle-spawn message-delta
 * (vehicle_new_from_network). The wire payload is the DB type vehicle_new_data (see message_vehicle_new.h,
 * the canonical definition); this alias keeps the decoder TU's historical local name. */

#include "message_vehicle_new.h"

typedef vehicle_new_data _vehicle_spawn_message;
