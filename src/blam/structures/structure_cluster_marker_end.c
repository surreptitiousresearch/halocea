/* structure_cluster_marker_end @ 0x83744678 — end a structure-cluster marking pass. */

#include "headers/structure_globals.h"

void structure_cluster_marker_end(void)
{
    structure_globals.cluster_marker_initialized = 0;
}
