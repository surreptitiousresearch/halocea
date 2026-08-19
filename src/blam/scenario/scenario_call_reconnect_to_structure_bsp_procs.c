/* scenario_call_reconnect_to_structure_bsp_procs @0x837032C0 — calls all 13 reconnect_to_structure_bsp_procs
 * entries in order; the array's established `void (*)(void)` prototype (scenario_switch_structure_bsp.c
 * precedent) means the decompiler's per-iteration "result" capture reads whatever garbage r3 held after a
 * void call, not a real return value — dropped here in favor of the true void-void call. */

extern void (*reconnect_to_structure_bsp_procs[13])(void);

void scenario_call_reconnect_to_structure_bsp_procs(void)
{
    for ( int i = 0; i < 13; i++ )
        reconnect_to_structure_bsp_procs[i]();
}
