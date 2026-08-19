/* reconnect_to_structure_bsp_procs @0x84177C44 — the 13-entry table of per-subsystem
 * "reconnect to the newly-loaded structure BSP" handlers. Invoked in order by
 * scenario_switch_structure_bsp (and scenario_call_reconnect_to_structure_bsp_procs) after a
 * new BSP is loaded and the global structure/collision/bsp3d pointers are rebound.
 *
 * Recovered from the binary's .data pointer table (idapython read of the 13 slots at
 * 0x84177C44..0x84177C74, memory order). Previously the array was extern-only in the tree with
 * its order unknown; this definition records the authoritative order.
 *
 * Each handler takes no arguments; a few have non-void return types (int / void* /
 * object_datum*) but the caller ignores the return, so the table is typed void(*)(void). The
 * local prototypes below intentionally declare them as void(void) to match that use; the
 * ignored return in their real definitions is a documented reconstruction latitude. */

extern void objects_reconnect_to_structure_bsp(void);
extern void lights_reconnect_to_structure_bsp(void);
extern void ai_reconnect_to_structure_bsp(void);
extern void effects_reconnect_to_structure_bsp(void);
extern void particles_reconnect_to_structure_bsp(void);
extern void particle_systems_reconnect_to_structure_bsp(void);
extern void contrails_reconnect_to_structure_bsp(void);
extern void decals_reconnect_to_structure_bsp(void);
extern void structure_decals_reconnect_to_structure_bsp(void);
extern int observer_reconnect_to_structure_bsp(void);
extern void players_reconnect_to_structure_bsp(void);
extern void sound_reconnect_to_structure_bsp(void);
extern void object_types_reconnect_to_structure_bsp(void);

void (*reconnect_to_structure_bsp_procs[13])(void) =
{
    objects_reconnect_to_structure_bsp,           /* +0x00 */
    lights_reconnect_to_structure_bsp,            /* +0x04 */
    ai_reconnect_to_structure_bsp,                /* +0x08 */
    effects_reconnect_to_structure_bsp,           /* +0x0C */
    particles_reconnect_to_structure_bsp,         /* +0x10 */
    particle_systems_reconnect_to_structure_bsp,  /* +0x14 */
    contrails_reconnect_to_structure_bsp,         /* +0x18 */
    decals_reconnect_to_structure_bsp,            /* +0x1C */
    structure_decals_reconnect_to_structure_bsp,  /* +0x20 */
    observer_reconnect_to_structure_bsp,          /* +0x24 */
    players_reconnect_to_structure_bsp,           /* +0x28 */
    sound_reconnect_to_structure_bsp,             /* +0x2C */
    object_types_reconnect_to_structure_bsp,      /* +0x30 */
};
