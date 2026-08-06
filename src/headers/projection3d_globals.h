#ifndef PROJECTION3D_GLOBALS_H
#define PROJECTION3D_GLOBALS_H

#include <stdint.h>

/* projection3d globals — axis-index lookup used when projecting a 3D point onto
 * a 2D plane. Indexed [projection][sign][coord]: for each of the 3 axis-aligned
 * projections and 2 sign orientations, gives the two source axis indices to
 * copy into the 2D result (x-axis, y-axis).
 *
 * Data @0x821145B4 (24 bytes, 3*2*2 shorts); DB applied type
 * `const __int16 global_projection3d_mappings[3][2][2]`:
 *   proj0: {{2,1},{1,2}}  proj1: {{0,2},{2,0}}  proj2: {{1,0},{0,1}} */
#ifdef __cplusplus
extern "C" {
#endif

extern const int16_t global_projection3d_mappings[3][2][2];

#ifdef __cplusplus
}
#endif

#endif /* PROJECTION3D_GLOBALS_H */
