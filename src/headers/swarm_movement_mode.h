#ifndef SWARM_MOVEMENT_MODE_H
#define SWARM_MOVEMENT_MODE_H

/* swarm_movement_mode — the local movement-mode vocabulary of infection_swarm_control (not stored
 * in any datum; purely the per-member steering selector derived from the actor's action).
 * GUESS (marked): no enum exists anywhere (the value never leaves the function); names are taken
 * from the producing actor_action and the observed steering behaviour:
 *   1/2/3 are the wander family (identical steering, different move/pause timer ranges),
 *   4 pursues the move-target prop, 5 moves directly away from it, 6 follows the obey
 *   direct-movement work-area. */
/* layout bound to DB enum: types_enum_values _5F9722FF666D268AD4D3846D0318AB1C */
enum swarm_movement_mode {
	_swarm_movement_none = 0x0,
	_swarm_movement_wander_noncombat = 0x1,
	_swarm_movement_wander_idle = 0x2,
	_swarm_movement_wander_combat = 0x3,
	_swarm_movement_towards_prop = 0x4,
	_swarm_movement_away_from_prop = 0x5,
	_swarm_movement_obey = 0x6,
	NUMBER_OF_SWARM_MOVEMENT_TYPES = 0x7,
};

#endif /* SWARM_MOVEMENT_MODE_H */
