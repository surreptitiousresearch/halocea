#ifndef SOUND_DEFINITION_FLAGS_H
#define SOUND_DEFINITION_FLAGS_H

/* sound_definition_flags — bit indices for sound_definition.flags (int @0x0).
 * Source: reference enum _FC298451D9ED45CF787C9E2C0EFA3D21 (headers_ref). Values are BIT INDICES — use as (1u << _bit). */
enum sound_definition_flags {
	_sound_definition_fit_to_compression_block_size_bit = 0x0,
	_sound_definition_linked_permutations_bit = 0x1,
	NUMBER_OF_SOUND_DEFINITION_FLAGS = 0x2,
};

#endif /* SOUND_DEFINITION_FLAGS_H */

// sound_definition_flags: DB-verified via types_enum_values _FC298451D9ED45CF787C9E2C0EFA3D21 (anonymous compiled enum, names sans leading _)
