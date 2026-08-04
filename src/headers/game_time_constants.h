#ifndef GAME_TIME_CONSTANTS_H
#define GAME_TIME_CONSTANTS_H

/* Blam runs its simulation at a fixed 30 ticks per second. These name the two float constants
 * the decompiler emits as bare magic numbers for tick<->seconds conversion. Only the clearly
 * derivable values are named here (no fabrication): SECONDS_PER_TICK is exactly 1.0f/30, the
 * single-precision rounding the compiler produced (0x3D088889). The reciprocal, 30.0f as a rate,
 * is context-dependent (also a plain count in non-timing code) so it is NOT blanket-#defined. */

#define TICKS_PER_SECOND 30
#define SECONDS_PER_TICK 0.033333335f   /* 1.0f / 30 */

#endif /* GAME_TIME_CONSTANTS_H */
