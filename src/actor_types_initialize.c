/* actor_types_initialize @ 0x837F8E00 — iterates the 16 actor types calling each type's `initialize`
 * hook. RESOLVED 2026-07-13: the loop decompiles empty NOT because a registration body was lost, but
 * because actor_type_definitions[] is statically initialized data (.data @0x841852E0 — see
 * actor_type_definitions.c for the recovered table) and every record's `initialize` callback is NULL,
 * so the optimizer deleted the calls and left the bare counter. */

void actor_types_initialize(void)
{
    int i;
    for ( i = 0; i < 16; ++i )
        ;
}
