#ifndef HS_SYNTAX_NODE_FLAGS_H
#define HS_SYNTAX_NODE_FLAGS_H

/* hs_syntax_node_flags — hs_syntax_node flags.
 * DB-verified: types_enum_values _15C691A9F4EB58220E231E3E6CA13BB4 carries these named
 * values verbatim. Values are BIT INDICES — use as (1u << _bit). */
enum hs_syntax_node_flags {
	_hs_syntax_node_primitive_bit = 0,
	_hs_syntax_node_script_bit = 1,
	_hs_syntax_node_variable_bit = 2,
	_hs_syntax_node_permanent_bit = 3,
	NUMBER_OF_HS_SYNTAX_NODE_FLAGS = 4,
};

#endif /* HS_SYNTAX_NODE_FLAGS_H */

// hs_syntax_node_flags: DB-verified via types_enum_values _15C691A9F4EB58220E231E3E6CA13BB4
