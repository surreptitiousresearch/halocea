/* ai_test_line_of_sight @0x836E81C0 — AI visibility/line-of-fire test between two points. Returns:
 *   4 = no line of sight (PVS-blocked, degenerate distance, or mostly-fogged far segment),
 *   1 = blocked by geometry, 0 = clear, 2 = clear but very close, 3 = clear at medium range.
 *
 * First rejects via the BSP potentially-visible-set. Casts the primary vector with a collision mask that
 * depends on test_line_of_fire (line-of-fire uses 0xC2B3, plain sight 0xC2A7) and clears the vehicle bit
 * (0x200) when ignore_vehicles is set. Heavy fog (>0.8) blocks; medium fog (>0.6) downgrades to "obscured".
 * In wide-cone modes (mode 1 = source spread 0.25, mode 2 = destination spread 0.1) it also casts a pair of
 * laterally-offset lines (offset along the horizontal perpendicular of the segment, falling back to
 * global_forward3d when the segment is vertical) plus a downward probe, to model peripheral vision / a body's
 * width. Finally classifies a clear path by total distance and the fraction travelled before the hit.
 *
 * Deviations: Hex-Rays widens the single-precision math to double (fpN) — kept as float here. v44 is the
 * collision_result.t field for the no-hit case (the decompiler reads the uninitialised-looking stack slot
 * that collision_test_vector fills); modelled as v45.t. */

#include <stdint.h>
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
#include "headers/collision_result.h"
#include "headers/collision_test_flags.h"
#include "headers/ai_line_of_sight.h"
#include "headers/location.h"
#include "headers/blam_data_globals.h"
extern float __fsqrts(float);


extern uint8_t scenario_test_pvs(int16_t cluster_index0, int16_t cluster_index1);
extern float scenario_fog_at_point(const location *viewer_location, const real_point3d *viewer_point, const real_point3d *point);
extern uint8_t collision_test_vector(unsigned int flags, const real_point3d *point, const real_vector3d *vector, int ignore_object_index, collision_result *collision);
extern uint8_t collision_test_line(unsigned int flags, const real_point3d *point0, const real_point3d *point1, int ignore_object_index, collision_result *collision);
extern float normalize3d(real_vector3d *v);

int16_t ai_test_line_of_sight(const real_point3d *p0, int16_t p0_cluster_index,
                          const real_point3d *p1, int16_t p1_cluster_index,
                          int16_t mode, uint8_t test_line_of_fire,
                          int ignore_object_index, uint8_t ignore_vehicles)
{
    uint8_t blocked = 0;

    if ( p0_cluster_index != -1 && p1_cluster_index != -1 && !scenario_test_pvs(p0_cluster_index, p1_cluster_index) )
        return _ai_line_of_sight_obstructed;

    /* line-of-fire mask 49843 = 0xC2B3 (ignore-breakable variant); plain-sight mask 49831 = 0xC2A7
     * (ignore-two-sided variant). Both include structure + vehicles/scenery/machines objects. */
    unsigned int mask = test_line_of_fire
        ? ( (1u << _collision_test_front_facing_surfaces_bit)
          | (1u << _collision_test_back_facing_surfaces_bit)
          | (1u << _collision_test_ignore_breakable_surfaces_bit)
          | (1u << _collision_test_structure_bit)
          | (1u << _collision_test_objects_bit)
          | (1u << _collision_test_objects_vehicles_bit)
          | (1u << _collision_test_objects_scenery_bit)
          | (1u << _collision_test_objects_machines_bit) )
        : ( (1u << _collision_test_front_facing_surfaces_bit)
          | (1u << _collision_test_back_facing_surfaces_bit)
          | (1u << _collision_test_ignore_two_sided_surfaces_bit)
          | (1u << _collision_test_structure_bit)
          | (1u << _collision_test_objects_bit)
          | (1u << _collision_test_objects_vehicles_bit)
          | (1u << _collision_test_objects_scenery_bit)
          | (1u << _collision_test_objects_machines_bit) );
    if ( ignore_vehicles )
        mask &= ~(1u << _collision_test_objects_vehicles_bit);

    real_vector3d delta;
    delta.n[0] = p1->n[0] - p0->n[0];
    delta.n[1] = p1->n[1] - p0->n[1];
    delta.n[2] = p1->n[2] - p0->n[2];

    collision_result result;
    float t;
    char reached_endpoint;
    if ( collision_test_vector(mask, p0, &delta, ignore_object_index, &result) )
    {
        t = result.t;
        reached_endpoint = 0;
    }
    else
    {
        /* Deviation: decompiler reads var_C4, a stack slot just before the collision_result struct, not
         * result.t (struct+0x14). For a no-hit cast this is the full-length fraction (1.0); it is only ever
         * consumed if heavy fog later re-clears reached_endpoint. */
        t = 1.0f;
        reached_endpoint = 1;
    }

    float fog = scenario_fog_at_point(&result.start_location, p0, &result.point);
    if ( fog > 0.80000001f )
    {
        reached_endpoint = 0;
        goto classify;
    }
    if ( fog > 0.60000002f )
    {
        blocked = 1;
        goto classify;
    }
    if ( !mode )
        goto classify;

    /* Horizontal perpendicular of the segment (rotate the XY delta 90°), normalized. */
    real_vector3d perpendicular;
    perpendicular.n[0] = p0->n[1] - p1->n[1];
    perpendicular.n[1] = p1->n[0] - p0->n[0];
    perpendicular.n[2] = 0.0f;
    if ( normalize3d(&perpendicular) == 0.0f )
    {
        perpendicular.n[0] = global_forward3d->n[0];
        perpendicular.n[1] = global_forward3d->n[1];
        perpendicular.n[2] = global_forward3d->n[2];
    }

    if ( mode == 1 )
    {
        /* Source spread: two lines offset ±0.25 around p0, aimed at p1. */
        real_point3d left, right;
        right.n[0] = p0->n[0] + perpendicular.n[0] * 0.25f;
        right.n[1] = p0->n[1] + perpendicular.n[1] * 0.25f;
        right.n[2] = p0->n[2] + perpendicular.n[2] * 0.25f;
        left.n[0]  = p0->n[0] - perpendicular.n[0] * 0.25f;
        left.n[1]  = p0->n[1] - perpendicular.n[1] * 0.25f;
        left.n[2]  = p0->n[2] - perpendicular.n[2] * 0.25f;

        if ( !reached_endpoint )
        {
            uint8_t right_hit = collision_test_line(mask, &right, p1, ignore_object_index, &result);
            blocked = (right_hit == 0);
            if ( right_hit )
                blocked = collision_test_line(mask, &left, p1, ignore_object_index, &result) == 0;
            goto classify;
        }
        blocked = collision_test_line(mask, &right, p1, ignore_object_index, &result);
        if ( !blocked )
            blocked = collision_test_line(mask, &left, p1, ignore_object_index, &result);
        goto classify;
    }

    /* mode 2 — destination spread: ±0.1 around p1, plus a downward 0.1 probe. */
    if ( !reached_endpoint )
        goto classify;

    real_point3d right, left, down;
    right.n[0] = p1->n[0] + perpendicular.n[0] * 0.1f;
    right.n[1] = p1->n[1] + perpendicular.n[1] * 0.1f;
    right.n[2] = p1->n[2] + perpendicular.n[2] * 0.1f;
    left.n[0]  = p1->n[0] - perpendicular.n[0] * 0.1f;
    left.n[1]  = p1->n[1] - perpendicular.n[1] * 0.1f;
    left.n[2]  = p1->n[2] - perpendicular.n[2] * 0.1f;
    down.n[0]  = p1->n[0] + global_down3d->n[0] * 0.1f;
    down.n[1]  = p1->n[1] + global_down3d->n[1] * 0.1f;
    down.n[2]  = p1->n[2] + global_down3d->n[2] * 0.1f;

    blocked = collision_test_line(mask, &right, p0, ignore_object_index, &result);
    if ( !blocked )
        blocked = collision_test_line(mask, &left, p0, ignore_object_index, &result);
    if ( blocked )
        goto classify;
    blocked = collision_test_line(mask, &down, p0, ignore_object_index, &result);

classify:
    if ( blocked )
        return _ai_line_of_sight_occluded;
    if ( reached_endpoint )
        return _ai_line_of_sight_clear;

    float dx = p1->n[0] - p0->n[0];
    float dy = p1->n[1] - p0->n[1];
    float dz = p1->n[2] - p0->n[2];
    float distance = __fsqrts(dx * dx + (dz * dz + dy * dy));
    if ( distance < 1.0f )
        return _ai_line_of_sight_obstructed;
    if ( distance * t < 1.0f )
        return _ai_line_of_sight_from_cover;
    if ( (1.0f - t) * distance >= 4.0f )
        return _ai_line_of_sight_obstructed;
    return _ai_line_of_sight_to_cover;
}
