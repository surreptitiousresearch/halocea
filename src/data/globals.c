/* globals @ 0x8446010C (.data, 4 bytes)
 * DB applied_types: breakable_surface_globals *globals;
 * Image bytes (big-endian), decoded from the binary .data record:
 *   +0x0000 = 0x00000000
 * data-bss zero-fill; DB applied-type `breakable_surface_globals *globals;`.
 */
#include "../headers/breakable_surface_globals.h"

breakable_surface_globals *globals;
