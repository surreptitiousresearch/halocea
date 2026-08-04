#pragma once
/* _biped_update_body — the 16-byte raw wire payload shared by biped_build_update_delta.c (encode) and
 * biped_process_update_delta.c (decode), matching object+1324..+1339 exactly. Must stay contiguous — the
 * message_delta_processor encode/decode calls treat it as one packed block.
 * Rebound to the DB payload type: this wire block IS a biped_datum_network_data (the same layout the
 * biped's baseline / last_network_data snapshots use); the former ad-hoc int/float fields
 * (payload_a / field_224 / crouch_or_scale / payload_b) were raw views of its members. */

#include "biped_datum_network_data.h"

typedef biped_datum_network_data _biped_update_body;
