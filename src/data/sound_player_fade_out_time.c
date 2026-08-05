/* sound_player_fade_out_time @ 0x82114954 — .rdata, 4 bytes, float. Initializer reconstructed
 * from the binary (big-endian):
 *   +0x0000 0x3E99999A -> 0.3f (seconds of fade-out passed to sound_start_fade)
 * Consumed by src/sound_stop_impulse.c.
 *
 * DEVIATION: src/sound_stop_impulse.c declares it non-const (`extern float
 * sound_player_fade_out_time;`). The object is in .rdata (segment perm 4, read-only), so the
 * original was const-qualified; the definition follows the segment and the consumer decl is
 * untouched. */

const float sound_player_fade_out_time = 0.3f;
