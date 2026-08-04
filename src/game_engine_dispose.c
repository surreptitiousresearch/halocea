/* game_engine_dispose @ 0x8374B4E8 — dispose the active engine variant and clear the
 * per-player score baseline table. game_engine points at a vtable-like definition whose
 * `dispose` slot (offset 0x8, per DB disasm) is called. */
/* blam_data_globals.h first: supplies wchar_t before dependent headers are parsed (migration) */
#include "headers/blam_data_globals.h"
/* Use canonical `struct game_engine *game_engine` (DB: game_engine *game_engine; 156B tag).
 * Deleted stale local `extern game_engine_class *game_engine;` that forked the type. */
extern void *memset(void *, int, unsigned int);
/* ATTESTED void: r3 at blr is memset residue (decompiler r3-threading); 0/5 callers consume. */
void game_engine_dispose(void)
{
    if ( game_engine )
    {
        /* dispose slot returns void (struct game_engine, DB); cannot cast to void*. */
        if ( game_engine->dispose )
            game_engine->dispose();
        game_engine = 0;
    }
    if ( g_player_score_is_initialized == 1 )
    {
        memset(g_player_score_baselines, 0, sizeof(g_player_score_baselines));
        g_player_score_is_initialized = 0;
    }
}
