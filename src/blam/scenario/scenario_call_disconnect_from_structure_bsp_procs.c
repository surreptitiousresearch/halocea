/* scenario_call_disconnect_from_structure_bsp_procs @0x83703260 — calls all 10
 * disconnect_from_structure_bsp_procs entries in order.
 *
 * DEVIATION: the decompiler rendered each iteration threading an `s`/`result` accumulator (r3
 * carried from the previous call), matching a stale int(*)(int) model of the array. The array's
 * true prototype is void(*)(void) (all 10 handlers take no arguments — see
 * disconnect_from_structure_bsp_procs.c), so the accumulator is a decompiler artifact and is
 * dropped here. No xrefs to this wrapper were found in the database (the live caller is
 * scenario_switch_structure_bsp), so its own return convention is unconfirmed; reproduced as a
 * plain void sweep. */

extern void (*disconnect_from_structure_bsp_procs[10])(void);

void scenario_call_disconnect_from_structure_bsp_procs(void)
{
    for ( int i = 0; i < 10; i++ )
        disconnect_from_structure_bsp_procs[i]();
}
