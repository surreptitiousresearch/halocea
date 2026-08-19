/* disconnect_from_structure_bsp_procs @0x84177C78 — the 10-entry table of per-subsystem
 * "disconnect from the current structure BSP" handlers. Invoked in order by
 * scenario_switch_structure_bsp (and scenario_call_disconnect_from_structure_bsp_procs) before
 * the old BSP is unloaded.
 *
 * Recovered from the binary's .data pointer table (idapython read of the 10 slots at
 * 0x84177C78..0x84177C9C, memory order). Previously the array was extern-only in the tree with
 * its order unknown; this definition records the authoritative order.
 *
 * Every handler takes no arguments. The old tree modeled this array as int(*)(int) with an
 * `s` accumulator threaded through the calls — that was a decompiler artifact of r3 carrying a
 * dead value (scenario_switch_structure_bsp seeds it from main_stop_time() and never reads it
 * after the loop). The true prototype is void(*)(void); return values (a couple are non-void)
 * are ignored. */

extern void effects_disconnect_from_structure_bsp(void);
extern void ai_disconnect_from_structure_bsp(void);
extern void objects_disconnect_from_structure_bsp(void);
extern void lights_disconnect_from_structure_bsp(void);
extern void object_types_disconnect_from_structure_bsp(void);
extern void particle_systems_disconnect_from_structure_bsp(void);
extern void particles_disconnect_from_structure_bsp(void);
extern void contrails_disconnect_from_structure_bsp(void);
extern void decals_disconnect_from_structure_bsp(void);
extern void structure_decals_disconnect_from_structure_bsp(void);

void (*disconnect_from_structure_bsp_procs[10])(void) =
{
    object_types_disconnect_from_structure_bsp,   /* +0x00 */
    objects_disconnect_from_structure_bsp,        /* +0x04 */
    lights_disconnect_from_structure_bsp,         /* +0x08 */
    ai_disconnect_from_structure_bsp,             /* +0x0C */
    effects_disconnect_from_structure_bsp,        /* +0x10 */
    particles_disconnect_from_structure_bsp,      /* +0x14 */
    particle_systems_disconnect_from_structure_bsp, /* +0x18 */
    contrails_disconnect_from_structure_bsp,      /* +0x1C */
    structure_decals_disconnect_from_structure_bsp, /* +0x20 */
    decals_disconnect_from_structure_bsp,         /* +0x24 */
};
