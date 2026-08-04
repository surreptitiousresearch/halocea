#ifndef FIRING_POSITION_GROUP_SELECTION_H
#define FIRING_POSITION_GROUP_SELECTION_H

/* firing_position_group_selection — group_selection_mode argument of
 * actor_get_firing_position_group (forces the searching/not-searching group choice).
 * DB-verified via types_enum_values $D353FFB9F05DFB749B1B04CDFB3D5B4D. */
enum firing_position_group_selection {
	_firing_position_group_normal = 0,
	_firing_position_group_when_searching = 1,
	_firing_position_group_when_not_searching = 2,
};

#endif /* FIRING_POSITION_GROUP_SELECTION_H */
