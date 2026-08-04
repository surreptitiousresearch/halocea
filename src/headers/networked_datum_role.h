#pragma once
/* NetworkedDatumRole — the multiplayer ownership role of a networked datum (object, etc.). Values from the
 * database. */

typedef enum NetworkedDatumRole
{
    _networked_datum_master = 0,
    _networked_datum_puppet = 1,
    _networked_datum_puppet_controlled_by_local_player = 2,
    _networked_datum_autonomous = 3,
} NetworkedDatumRole;
