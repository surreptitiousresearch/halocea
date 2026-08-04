/* glow_trailing_particle_update_position @0x8380B7E0 — DEVIATION: `particle` is a genuinely dead parameter
 * (disasm-confirmed: no instruction ever reads r4, the register it's passed in). All the work advects
 * marker[0]'s node_matrix position row (n[2]) by marker[0]'s own matrix row-0 velocity components,
 * scaled by elapsed_time — matches the DB's own 3-argument signature, so no phantom argument was added. */

#include "headers/glow_datum.h"

void glow_trailing_particle_update_position(glow_datum *glow, glow_particle *particle, float elapsed_time)
{
    object_marker *marker = &glow->markers[0];
    float row21 = marker->matrix.n[0][0] * elapsed_time + marker->node_matrix.n[2][1];
    float row22 = marker->matrix.n[0][1] * elapsed_time + marker->node_matrix.n[2][2];
    float row30 = marker->matrix.n[0][2] * elapsed_time + marker->node_matrix.n[3][0];
    marker->node_matrix.n[2][1] = row21;
    marker->node_matrix.n[2][2] = row22;
    marker->node_matrix.n[3][0] = row30;
}
