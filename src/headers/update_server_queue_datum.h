#pragma once
/* update_server_queue_datum — one per-machine element of update_server_globals.queues (100-byte
 * stride), spelled with the `_datum` suffix most call sites prefer.
 *
 * This header used to carry its OWN copy of `struct update_server_queue`, byte-identical to the
 * one in update_server_queue.h. Two headers defining the same struct tag is a redefinition error
 * waiting for the first translation unit that includes both — it only stayed quiet because no TU
 * did. The DB attests the name `update_server_queue` (types_members, 100 bytes), so that header
 * is canonical and this one is now purely the alias. */

#include "update_server_queue.h"

typedef update_server_queue update_server_queue_datum;
