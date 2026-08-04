#ifndef PATH_CONSTANTS_H
#define PATH_CONSTANTS_H

/* Blam pathfinding working-set sizing constants.
 * DB-verified via types_enum_values _6F55C311F8778D997275B0C4992F8E3B (names verbatim). */
enum path_constants {
	PATH_NODE_LIST_SIZE = 1024,
	PATH_HASH_TABLE_SIZE = 4096,
};

#endif /* PATH_CONSTANTS_H */
