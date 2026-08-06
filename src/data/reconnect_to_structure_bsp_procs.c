/* reconnect_to_structure_bsp_procs @ 0x84177C44 (.data, 52 bytes)
 * DB applied_types: void (__fastcall *reconnect_to_structure_bsp_procs[13])();
 * Image bytes (big-endian), decoded from the binary .data record:
 *   +0x0000 = 0x836F0F40 -> objects_reconnect_to_structure_bsp
 *   +0x0004 = 0x836F8A18 -> lights_reconnect_to_structure_bsp
 *   +0x0008 = 0x836EA1E0 -> ai_reconnect_to_structure_bsp
 *   +0x000C = 0x836E2170 -> effects_reconnect_to_structure_bsp
 *   +0x0010 = 0x8373DA78 -> particles_reconnect_to_structure_bsp
 *   +0x0014 = 0x8373A318 -> particle_systems_reconnect_to_structure_bsp
 *   +0x0018 = 0x8373F100 -> contrails_reconnect_to_structure_bsp
 *   +0x001C = 0x83741640 -> decals_reconnect_to_structure_bsp
 *   +0x0020 = 0x837E2D48 -> structure_decals_reconnect_to_structure_bsp
 *   +0x0024 = 0x8370EBE0 -> observer_reconnect_to_structure_bsp
 *   +0x0028 = 0x836ACB58 -> players_reconnect_to_structure_bsp
 *   +0x002C = 0x837151F0 -> sound_reconnect_to_structure_bsp
 *   +0x0030 = 0x83703218 -> object_types_reconnect_to_structure_bsp
 * .data data-init.
 * DB: void (*[13])(void); per-subsystem BSP reconnect callbacks (reloc order).
 */
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
