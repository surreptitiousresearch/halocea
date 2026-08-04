#ifndef CHARGE_GOAL_H
#define CHARGE_GOAL_H

/* charge_goal — what an AI charge action is trying to do (charge_state_data.goal).
 * Source: reference enum _F8842ACDD5026CAC25FFEAFF2A1B2C2D (headers_ref); the IDA DB keeps only the
 * anonymized member-less enum shell. */
enum charge_goal {
	charge_goal_close_range = 0,
	charge_goal_stalking = 1,
	charge_goal_melee = 2,
	charge_goal_melee_leaping = 3,
	charge_goal_vehicle_strafing = 4,
	charge_goal_vehicle_ramming = 5,
	number_of_charge_goals = 6
} ;

#endif /* CHARGE_GOAL_H */

// charge_goal: DB-verified via types_enum_values _F8842ACDD5026CAC25FFEAFF2A1B2C2D (anonymous compiled enum, names sans leading _)
