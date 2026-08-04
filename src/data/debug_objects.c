/* debug_objects @ 0x8477ACF0 - data-bss zero-fill; DB unsigned __int8, consumers unsigned char (1 byte).
 * The DB object and the rasterizer consumers all treat it as a 1-byte flag; defined at DB width.
 * (A previously-stale int-typed extern in hcex_process_frame_boundary.h was corrected to 1 byte.) */

unsigned char debug_objects;
