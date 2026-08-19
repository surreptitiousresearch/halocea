/* find_hill @0x8382BB70 — rebuild the King-of-the-Hill hill geometry into king_globals for the current
 * king_globals.hill_id. Finds the hill's netgame flags, copies their positions, expands a lone flag into
 * a 1-unit square, projects to 2D, computes the convex hull, stores the ordered 3D/2D hull into
 * king_globals.hill_points / .convex_hull, and finally records the hill bounding box (top/bottom/center).
 *
 * Signature note: the DB proto is `int find_hill(int, __int16, __int16)`, but the disasm proves all three
 * params are unused — r3 is overwritten (li r3,0) before any read and r4/r5 are only ever written as
 * post-call scratch. The `int` return is likewise a dead register leftover (r3 from the reorder loop);
 * callers read results from king_globals. Reconstructed as `void find_hill(void)`, matching the two
 * existing corpus externs (a third caller's `find_hill(start_index, 0, 0)` form is spurious).
 *
 * FPR-shadow (catalog class 1): find_netgame_flags takes two float args (distance, height_delta) that
 * burn the r4/r5 GPR shadow slots; the decompiler scrambled the remaining args, rendering the call as
 * `find_netgame_flags(0,0.0,0.0, a2, a3, 8, SLOWORD(hill_id))`. Disasm-confirmed real arguments are
 * type=8, team=hill_id, max=12, flags=<stack buffer>. */

#include <stdint.h>
#include "headers/scenario.h"
#include "headers/scenario_netgame_flag.h"
#include "headers/netgame_flag_type.h"
#include "headers/king_globals.h"
#include "headers/real_point2d.h"
#include "headers/real_point3d.h"
#include "headers/blam_data_globals.h"

extern int find_netgame_flags(const real_point3d *location, float distance, float height_delta, int16_t type, int16_t team, int max, int *flags);
extern int16_t convex_hull2d(int16_t vertex_count, const real_point2d *points, int16_t *hull_vertex_indices);

void find_hill(void)
{
    scenario *current_scenario = global_scenario;

    int flag_indices[12];
    int flag_count = find_netgame_flags(nullptr, 0.0f, 0.0f, _netgame_flag_hill, (int16_t)king_globals.hill_id, 12, flag_indices);
    king_globals.hill_point_count = flag_count;

    if ( flag_count == 0 )
        return;

    real_point3d local_points[12];
    if ( flag_count > 0 )
    {
        scenario_netgame_flag *netgame_flags =
            (scenario_netgame_flag *)current_scenario->netgame_flags.address;
        for ( int i = 0; i < flag_count; ++i )
        {
            /* recovered: (scenario_netgame_flag*)&netgame_flags[148*idx] -> typed array index */
            scenario_netgame_flag *flag = &netgame_flags[flag_indices[i]];
            local_points[i] = flag->position;
        }
    }

    int point_count = flag_count;
    if ( flag_count == 1 )
    {
        /* A single hill flag: expand it into a 1-unit square (4 corners). All corners are derived from
         * the original center before local_points[0] is overwritten. */
        point_count = 4;
        float center_x = local_points[0].n[0];
        float center_y = local_points[0].n[1];
        float center_z = local_points[0].n[2];
        local_points[1].n[0] = center_x + 1.0f;
        local_points[1].n[1] = center_y - 1.0f;
        local_points[1].n[2] = center_z;
        local_points[2].n[0] = center_x - 1.0f;
        local_points[2].n[1] = center_y + 1.0f;
        local_points[2].n[2] = center_z;
        local_points[3].n[0] = center_x + 1.0f;
        local_points[3].n[1] = center_y + 1.0f;
        local_points[3].n[2] = center_z;
        local_points[0].n[0] = center_x - 1.0f;
        local_points[0].n[1] = center_y - 1.0f;
        /* local_points[0].n[2] left unchanged */
    }

    /* Project to 2D (drop z) for the convex-hull computation.
     * DEVIATION: the disasm hand-unrolls this 4 points at a time (plus a 1-at-a-time remainder loop) with
     * base-pointer offset tricks; the net effect is this plain strided x,y copy. */
    real_point2d hull_input_points[16];
    for ( int i = 0; i < point_count; ++i )
    {
        hull_input_points[i].n[0] = local_points[i].n[0];
        hull_input_points[i].n[1] = local_points[i].n[1];
    }

    int16_t hull_vertex_indices[16];
    int16_t hull_count = convex_hull2d((int16_t)point_count, hull_input_points, hull_vertex_indices);

    if ( hull_count > 0 )
    {
        for ( int out = 0; out < hull_count; ++out )
        {
            int hull_index = hull_vertex_indices[out];
            king_globals.hill_points[out] = local_points[hull_index];
            king_globals.convex_hull[out].n[0] = hull_input_points[hull_index].n[0];
            king_globals.convex_hull[out].n[1] = hull_input_points[hull_index].n[1];
        }
    }

    king_globals.hill_point_count = hull_count;

    /* Bounding box over the ordered hill points. Initialized from hill_points[0] and stored
     * unconditionally — if hull_count==0 this reads a stale hill_points[0] (faithful quirk). */
    float min_x = king_globals.hill_points[0].n[0], max_x = king_globals.hill_points[0].n[0];
    float min_y = king_globals.hill_points[0].n[1], max_y = king_globals.hill_points[0].n[1];
    float min_z = king_globals.hill_points[0].n[2], max_z = king_globals.hill_points[0].n[2];
    if ( hull_count > 0 )
    {
        for ( int i = 0; i < hull_count; ++i )
        {
            float x = king_globals.hill_points[i].n[0];
            float y = king_globals.hill_points[i].n[1];
            float z = king_globals.hill_points[i].n[2];
            if ( min_x > x ) min_x = x;
            if ( min_y > y ) min_y = y;
            if ( min_z > z ) min_z = z;
            if ( max_x <= x ) max_x = x;
            if ( max_y <= y ) max_y = y;
            if ( max_z <= z ) max_z = z;
        }
    }
    king_globals.hill_bottom = min_z - 0.1f;
    king_globals.hill_top = max_z + 0.80000001f;
    king_globals.hill_center.n[0] = (max_x + min_x) * 0.5f;
    king_globals.hill_center.n[1] = (max_y + min_y) * 0.5f;
    king_globals.hill_center.n[2] = (max_z + min_z) * 0.5f;
}
