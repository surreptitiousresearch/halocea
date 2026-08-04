/* reconnect_to_structure_bsp_procs @ 0x84177C44 — .data data-init.
 * DB: void (*[13])(void); per-subsystem BSP reconnect callbacks (reloc order). */
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

void (*reconnect_to_structure_bsp_procs[13])(void) = {
    objects_reconnect_to_structure_bsp,
    lights_reconnect_to_structure_bsp,
    ai_reconnect_to_structure_bsp,
    effects_reconnect_to_structure_bsp,
    particles_reconnect_to_structure_bsp,
    particle_systems_reconnect_to_structure_bsp,
    contrails_reconnect_to_structure_bsp,
    decals_reconnect_to_structure_bsp,
    structure_decals_reconnect_to_structure_bsp,
    observer_reconnect_to_structure_bsp,
    players_reconnect_to_structure_bsp,
    sound_reconnect_to_structure_bsp,
    object_types_reconnect_to_structure_bsp,
};
