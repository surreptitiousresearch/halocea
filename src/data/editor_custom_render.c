/* editor_custom_render @ 0x84184C8C (.data, 4 bytes)
 * DB applied_types: render_globals *editor_custom_render;
 * Image bytes (big-endian), decoded from the binary .data record:
 *   +0x0000 = 0x846DD100 -> render
 */
#include "../headers/render_globals.h"
/* points to named global render (render_globals) at 0x846DD100 */
extern render_globals render;
render_globals *editor_custom_render = &render;
