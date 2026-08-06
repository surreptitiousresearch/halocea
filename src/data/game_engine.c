/* game_engine @ 0x844613B8 (.data, 4 bytes)
 * DB applied_types: game_engine *game_engine;
 * Image bytes (big-endian), decoded from the binary .data record:
 *   +0x0000 = 0x00000000
 * third-order .data. Pointer to the active game-engine (variant) object,
 * or NULL when no game-engine variant is running (game_engine_can_score et al. test it). Zero-init.
 * Defined with the typed majority form (`struct game_engine *`); some call sites declare it `void *`
 * (pre-existing extern drift, harmless in C: symbol linkage carries no type). applied_types spells
 * the same thing as `game_engine *game_engine;` — IDA elides the `struct` keyword, so that row is a
 * spelling difference, not a width or shape one.
 * DEVIATION: the standalone `struct game_engine;` forward declaration that used to precede the
 * definition has been folded away. tools/gen_canonical_globals_header.py lifts the FIRST file-scope
 * declaration naming the symbol, so it lifted that line and emitted the degenerate
 * `extern struct game_engine;` — which declares no object at all, silently dropping `game_engine`
 * from the canonical globals header. The pointer declaration below declares the incomplete tag by
 * itself, so nothing is lost.
 */
struct game_engine *game_engine = 0;
