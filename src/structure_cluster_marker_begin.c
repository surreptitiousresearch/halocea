/* structure_cluster_marker_begin @ 0x837445F0 — start a structure-cluster marking pass: flag it
 * active and bump the marker so all clusters appear unmarked. */

#include "headers/structure_globals.h"

void structure_cluster_marker_begin(void)
{
    structure_globals.cluster_marker_initialized = 1;
    ++structure_globals.cluster_marker;
}
