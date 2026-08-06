/* disconnect_from_structure_bsp_procs @ 0x84177C78 (.data, 40 bytes)
 * DB applied_types: void (__fastcall *disconnect_from_structure_bsp_procs[10])();
 * Image bytes (big-endian), decoded from the binary .data record:
 *   +0x0000 = 0x83702BB0 -> object_types_disconnect_from_structure_bsp
 *   +0x0004 = 0x836EF8C8 -> objects_disconnect_from_structure_bsp
 *   +0x0008 = 0x836F8198 -> lights_disconnect_from_structure_bsp
 *   +0x000C = 0x836E9D98 -> ai_disconnect_from_structure_bsp
 *   +0x0010 = 0x836E1208 -> effects_disconnect_from_structure_bsp
 *   +0x0014 = 0x8373DA70 -> particles_disconnect_from_structure_bsp
 *   +0x0018 = 0x8373A310 -> particle_systems_disconnect_from_structure_bsp
 *   +0x001C = 0x8373F0F8 -> contrails_disconnect_from_structure_bsp
 *   +0x0020 = 0x837E2D60 -> structure_decals_disconnect_from_structure_bsp
 *   +0x0024 = 0x83741790 -> decals_disconnect_from_structure_bsp
 * .data data-init.
 * DB: void (*[10])(void); per-subsystem BSP disconnect callbacks (reloc order).
 */
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
