#ifndef MULTIPLAYER_SPAWN_GAME_TYPE_H
#define MULTIPLAYER_SPAWN_GAME_TYPE_H

/* multiplayer_spawn_game_type — game-type bit indices for a multiplayer vehicle placement's
 * spawn flags (scenario_vehicle_datum.multiplayer_spawn_flags and vehicle_info_s.spawn_flags,
 * both plain unsigned __int16). Values are BIT INDICES — use as ((flags >> _bit) & 1).
 *
 * Reconstruction (NO DB enum: the DB models both fields as bare u16). The game-type -> bit
 * mapping is transcribed verbatim from the engine's own switch(game_engine_index) statements in
 * vehicle_remapper_is_vehicle_default_for_current_variant / _process_by_teams / _process_no_teams
 * (default-vehicle flags at bits 0-3) and vehicle_remapper_add_vehicle (the same game-type set
 * repeated at bits 8-11, tested to accept a placement into the remapper). Note the ordering is the
 * vehicle-remapper's own (slayer,ctf,king,oddball), not game_engine_type's index order. */
enum multiplayer_spawn_game_type {
	_multiplayer_spawn_slayer_bit  = 0,
	_multiplayer_spawn_ctf_bit     = 1,
	_multiplayer_spawn_king_bit    = 2,
	_multiplayer_spawn_oddball_bit = 3,
};

/* the scenario field carries a second copy of the same four game-type flags in the high byte
 * (bits 8-11); add_vehicle tests those to decide whether to accept a placement. */
#define MULTIPLAYER_SPAWN_ACCEPT_SHIFT 8

#endif /* MULTIPLAYER_SPAWN_GAME_TYPE_H */
