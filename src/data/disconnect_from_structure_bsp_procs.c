/* disconnect_from_structure_bsp_procs @ 0x84177C78 — .data data-init.
 * DB: void (*[10])(void); per-subsystem BSP disconnect callbacks (reloc order). */
extern void object_types_disconnect_from_structure_bsp(void);
extern void objects_disconnect_from_structure_bsp(void);
extern void lights_disconnect_from_structure_bsp(void);
extern void ai_disconnect_from_structure_bsp(void);
extern void effects_disconnect_from_structure_bsp(void);
extern void particles_disconnect_from_structure_bsp(void);
extern void particle_systems_disconnect_from_structure_bsp(void);
extern void contrails_disconnect_from_structure_bsp(void);
extern void structure_decals_disconnect_from_structure_bsp(void);
extern void decals_disconnect_from_structure_bsp(void);

void (*disconnect_from_structure_bsp_procs[10])(void) = {
    object_types_disconnect_from_structure_bsp,
    objects_disconnect_from_structure_bsp,
    lights_disconnect_from_structure_bsp,
    ai_disconnect_from_structure_bsp,
    effects_disconnect_from_structure_bsp,
    particles_disconnect_from_structure_bsp,
    particle_systems_disconnect_from_structure_bsp,
    contrails_disconnect_from_structure_bsp,
    structure_decals_disconnect_from_structure_bsp,
    decals_disconnect_from_structure_bsp,
};
