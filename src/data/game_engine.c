/* game_engine @ 0x844613B8 -- third-order .data. Pointer to the active game-engine (variant) object,
 * or NULL when no game-engine variant is running (game_engine_can_score et al. test it). Zero-init.
 * Defined with the typed majority form (`struct game_engine *`); some call sites declare it `void *`
 * (pre-existing extern drift, harmless in C: symbol linkage carries no type). */
struct game_engine;
struct game_engine *game_engine = 0;
