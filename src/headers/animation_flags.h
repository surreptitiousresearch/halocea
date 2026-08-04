#ifndef ANIMATION_FLAGS_H
#define ANIMATION_FLAGS_H

/* animation_flags — bit indices for animation.flags (model_animation animation block; animation->flags).
 * Source: reference enum _AF69237E20C2B9C1D5D90D996B8F6FE9 (headers_ref). Values are BIT INDICES — use as (1u << _bit). */
enum animation_flags {
	_animation_compressed_bit = 0x0,
	_animation_world_relative_bit = 0x1,
	_animation_25Hz_bit = 0x2,
	NUMBER_OF_ANIMATION_FLAGS = 0x3,
};

#endif /* ANIMATION_FLAGS_H */

// animation_flags: DB-verified via types_enum_values _AF69237E20C2B9C1D5D90D996B8F6FE9 (anonymous compiled enum, names sans leading _)
