/* hcex_calculate_compass @0x836AE898 — compute the [0,1] compass-needle position for an object (a vehicle
 * or biped), measuring its facing heading against the scenario's north and normalising one full turn to the
 * [0,1] range. When the object faces nearly straight up or down (|forward.z| >= 0.995) the heading is
 * unstable, so the previously cached value is returned instead.
 *
 * DEVIATION: the decompiler rendered the int<->float conversions and the atan2 argument packing as
 * __PAIR64__ / long-double shuffles; restored to plain float math. atan2 here is called atan2(forward.x,
 * forward.y) (verified from the disassembly: f1<-matrix+4, f2<-matrix+8). */

#include <stdint.h>
#include "../headers/real_matrix4x3.h"
#include "../headers/scenario.h"
#include "../headers/math_constants.h"

extern scenario *global_scenario;

extern real_matrix4x3 *object_get_node_matrix(int object_index, int16_t node_index);
extern double atan2(double y, double x);

/* Cached needle position, retained when the heading can't be measured (object pointing up/down). */
static float magnitude;

extern "C" float hcex_calculate_compass(int object_index)
{
    real_matrix4x3 *node_matrix = object_get_node_matrix(object_index, 0);

    if (__fabs(node_matrix->n[0][2]) >= 0.9950000047683716f)
        return magnitude;

    float heading = (float)atan2(node_matrix->n[0][0], node_matrix->n[0][1]);
    float relative = heading - global_scenario->local_north;
    if (relative >= PI)
        relative -= TWO_PI;
    if (relative <= -PI)
        relative += TWO_PI;

    float needle = relative * INV_TWO_PI + 0.5f;
    if (needle < 0.0f)
        needle = 0.0f;
    else if (needle > 1.0f)
        needle = 1.0f;

    magnitude = needle;
    return needle;
}
