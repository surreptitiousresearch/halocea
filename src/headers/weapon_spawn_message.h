#pragma once
/* _weapon_spawn_message — the by-ref decode destination for a weapon-spawn message-delta
 * (weapon_new_from_network). The wire payload is the DB type weapon_new_data (see message_weapon_new.h,
 * the canonical definition); this alias keeps the decoder TU's historical local name. */

#include "message_weapon_new.h"

typedef weapon_new_data _weapon_spawn_message;
