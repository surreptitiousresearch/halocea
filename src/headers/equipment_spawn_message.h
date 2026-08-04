#pragma once
/* _equipment_spawn_message — the by-ref decode destination for an equipment-spawn message-delta
 * (equipment_new_from_network). The wire payload is the DB type equipment_new_data (see
 * message_equipment_new.h, the canonical definition); this alias keeps the decoder TU's historical name. */

#include "message_equipment_new.h"

typedef equipment_new_data _equipment_spawn_message;
