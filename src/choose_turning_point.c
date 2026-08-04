/* choose_turning_point @0x8381E758 — decide whether the path should turn clockwise or counterclockwise around
 * an obstacle: sums the signed turning angle along the "go straight, then bend through the obstructed-path
 * direction" route on each side (clockwise: start->clockwise_turning_point->unobstructed_path_point vs.
 * counterclockwise: start->counterclockwise_turning_point->obstructed_path_point, each compared against the
 * direct start->counterclockwise_turning_point leg) and picks whichever side accumulates the smaller total
 * turn. Returns 1 and writes `clockwise_turning_point` into `*result` if clockwise wins, else returns 0 and
 * writes `counterclockwise_turning_point`. */

#include <stdint.h>
#include "headers/real_point2d.h"
#include "headers/real_vector2d.h"

extern double __fabs(double x);
extern float __fsqrts(float x);
extern float signed_angle_between_vectors2d(const real_vector2d *a, const real_vector2d *b);

static void normalize_direction(real_vector2d *direction)
{
    float length = __fsqrts(direction->n[0] * direction->n[0] + direction->n[1] * direction->n[1]);
    if (__fabs(length) >= 0.000099999997f)
    {
        float inv_length = 1.0f / length;
        direction->n[0] *= inv_length;
        direction->n[1] *= inv_length;
    }
}

uint8_t choose_turning_point(const real_point2d *start_point, const real_point2d *clockwise_turning_point,
        const real_point2d *counterclockwise_turning_point, const real_point2d *unobstructed_path_point,
        const real_point2d *obstructed_path_point, real_point2d *result)
{
    real_vector2d start_to_clockwise;
    start_to_clockwise.n[0] = start_point->n[0] - clockwise_turning_point->n[0];
    start_to_clockwise.n[1] = start_point->n[1] - clockwise_turning_point->n[1];
    normalize_direction(&start_to_clockwise);

    real_vector2d clockwise_to_unobstructed;
    clockwise_to_unobstructed.n[0] = unobstructed_path_point->n[0] - clockwise_turning_point->n[0];
    clockwise_to_unobstructed.n[1] = unobstructed_path_point->n[1] - clockwise_turning_point->n[1];
    normalize_direction(&clockwise_to_unobstructed);

    real_vector2d clockwise_to_obstructed;
    clockwise_to_obstructed.n[0] = obstructed_path_point->n[0] - clockwise_turning_point->n[0];
    clockwise_to_obstructed.n[1] = obstructed_path_point->n[1] - clockwise_turning_point->n[1];
    normalize_direction(&clockwise_to_obstructed);

    real_vector2d start_to_counterclockwise;
    start_to_counterclockwise.n[0] = start_point->n[0] - counterclockwise_turning_point->n[0];
    start_to_counterclockwise.n[1] = start_point->n[1] - counterclockwise_turning_point->n[1];
    normalize_direction(&start_to_counterclockwise);

    real_vector2d counterclockwise_to_unobstructed;
    counterclockwise_to_unobstructed.n[0] = unobstructed_path_point->n[0] - counterclockwise_turning_point->n[0];
    counterclockwise_to_unobstructed.n[1] = unobstructed_path_point->n[1] - counterclockwise_turning_point->n[1];
    normalize_direction(&counterclockwise_to_unobstructed);

    real_vector2d counterclockwise_to_obstructed;
    counterclockwise_to_obstructed.n[0] = obstructed_path_point->n[0] - counterclockwise_turning_point->n[0];
    counterclockwise_to_obstructed.n[1] = obstructed_path_point->n[1] - counterclockwise_turning_point->n[1];
    normalize_direction(&counterclockwise_to_obstructed);

    float clockwise_turn = signed_angle_between_vectors2d(&clockwise_to_unobstructed, &clockwise_to_obstructed)
            + signed_angle_between_vectors2d(&start_to_clockwise, &clockwise_to_unobstructed);
    float counterclockwise_turn = signed_angle_between_vectors2d(&counterclockwise_to_unobstructed, &counterclockwise_to_obstructed);

    if (clockwise_turn <= -(counterclockwise_turn + signed_angle_between_vectors2d(&start_to_counterclockwise, &counterclockwise_to_unobstructed)))
    {
        *result = *counterclockwise_turning_point;
        return 0;
    }

    *result = *clockwise_turning_point;
    return 1;
}
