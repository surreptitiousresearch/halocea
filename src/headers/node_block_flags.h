#ifndef NODE_BLOCK_FLAGS_H
#define NODE_BLOCK_FLAGS_H

/* node_block_flags — bit indices for animation_graph_node.flags (the "joint type" bits of an
 * animation-graph node block, used by the limp-body ragdoll joint solver). Use as (1u << _bit):
 *   bit 0 (0x1) ball-and-socket, bit 1 (0x2) hinge, bit 2 (0x4) no-movement (node disabled).
 * The default (no hinge / no no-movement bit) is the ball-and-socket / conical constraint.
 * Source: reference enum _B1E9B83275B192C171B506B4C69C15A5 (types_enum_values; anonymous compiled
 * enum, count NUMBER_OF_NODE_BLOCK_FLAGS = 3). */
enum node_block_flags {
	_joint_type_ball_and_socket = 0x0,
	_joint_type_hinge           = 0x1,
	_joint_type_no_movement     = 0x2,
	NUMBER_OF_NODE_BLOCK_FLAGS  = 0x3,
};

#endif /* NODE_BLOCK_FLAGS_H */
