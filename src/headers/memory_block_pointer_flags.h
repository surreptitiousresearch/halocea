#ifndef MEMORY_BLOCK_POINTER_FLAGS_H
#define MEMORY_BLOCK_POINTER_FLAGS_H

/* memory_block_pointer_flags — memory_block_pointer flags.
 * DB-verified: types_enum_values _48F215E0EAE2325CC042E1844516444D carries these named
 * values verbatim. Bit-index members use as (1u << _bit); *_MASK / *_MAXIMUM
 * / large members are composite masks or constants, used as-is. */
enum memory_block_pointer_flags {
	_pointer_locked_bit = -2147483648,
	_pointer_size_mask = 2147483647,
	MAXIMUM_BLOCK_SIZE = 2147483647,
	ALIGNMENT_BYTE_BOUNDARY = 4,
};

#endif /* MEMORY_BLOCK_POINTER_FLAGS_H */

// memory_block_pointer_flags: DB-verified via types_enum_values _48F215E0EAE2325CC042E1844516444D
