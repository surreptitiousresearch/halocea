#pragma once
/* _biped_spawn_message — the by-ref decode destination for a biped-spawn message-delta
 * (biped_new_from_network). The wire payload is the DB type biped_new_data (see message_biped_new.h,
 * the canonical definition); this alias keeps the decoder TU's historical local name. */

#include "message_biped_new.h"

typedef biped_new_data _biped_spawn_message;
