#ifndef ANIMATION_TYPE_H
#define ANIMATION_TYPE_H

/* animation_type — how a model animation combines with the skeleton (animation.type).
 * Source: reference enum _A96164D19D7F9B75E88B376D81AC0F73 (headers_ref); the IDA DB keeps only the
 * anonymized member-less enum shell. */
enum animation_type {
	animation_base = 0,
	animation_overlay = 1,
	animation_replacement = 2,
	number_of_animation_types = 3
} ;

#endif /* ANIMATION_TYPE_H */

// animation_type: DB-verified via types_enum_values _A96164D19D7F9B75E88B376D81AC0F73 (anonymous compiled enum, names sans leading _)
