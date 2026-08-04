#ifndef CLIENT_UPDATE_FLAGS_H
#define CLIENT_UPDATE_FLAGS_H

/* client_update_flags — client_update flags.
 * DB-verified: types_enum_values _1BCBDE291150EA4AD3CD2D8DF91585AD carries these named
 * values verbatim. Bit-index members use as (1u << _bit); *_MASK / *_MAXIMUM
 * / large members are composite masks or constants, used as-is. */
enum client_update_flags {
	_client_out_of_sync_bit = -2147483648,
	_client_update_number_mask = 2147483647,
};

#endif /* CLIENT_UPDATE_FLAGS_H */

// client_update_flags: DB-verified via types_enum_values _1BCBDE291150EA4AD3CD2D8DF91585AD
