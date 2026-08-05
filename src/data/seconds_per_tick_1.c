/* seconds_per_tick_1 @ 0x8211DBA0 — .rdata, 4 bytes, float. Initializer reconstructed from
 * the binary (big-endian):
 *   +0x0000 0x3D088889 -> 0.033333335f, the single-precision rounding of 1.0f/30 already
 *                         named SECONDS_PER_TICK in src/headers/game_time_constants.h
 * Consumed by src/projectile_detonate.c for the contrail delta-time.
 *
 * DEVIATION: src/projectile_detonate.c declares it non-const (`extern float
 * seconds_per_tick_1;`). The object is in .rdata (segment perm 4, read-only), so the original
 * was const-qualified; the definition follows the segment and the consumer decl is untouched. */

#include "../headers/game_time_constants.h"

const float seconds_per_tick_1 = SECONDS_PER_TICK;
