/* observer_maximum_accelerations @0x821147D0 — .rdata, 20 bytes to the next symbol
 * (seconds_per_tick @0x821147E4); DB applied type `const float[5]`.
 * Big-endian words -> IEEE-754 floats:
 *   +0x0000 0x44BB8000 ->   1500.0   (group 0: focus position, 3 reals)
 *   +0x0004 0x44BB8000 ->   1500.0   (group 1: focus offset,   3 reals)
 *   +0x0008 0x47C35000 -> 100000.0   (group 2: focus distance, 1 real)
 *   +0x000C 0x47C35000 -> 100000.0   (group 3: field of view,  1 real)
 *   +0x0010 0x47C35000 -> 100000.0   (group 4: orientation,    3 reals)
 * One acceleration ceiling per observer parameter group — the five groups named by
 * observer_command's parameter_timers overlay (position / focus_offset / distance / field_of_view /
 * orientation), whose real counts are observer_parameter_derivative_real_counts {3,3,1,1,3}.
 * observer_update_accelerations snaps a group's blend to completion when any scalar's second
 * derivative exceeds its entry.
 *
 * DEVIATION: src/observer_update_accelerations.c declares this `extern float
 * observer_maximum_accelerations[5]` with no const; the object is in .rdata and the DB applied type
 * is const. Defined const. */

const float observer_maximum_accelerations[5] =
{
    1500.0f, 1500.0f, 100000.0f, 100000.0f, 100000.0f
};
