/* build_sprite_compute_basis @0x837EBE78 — compute the orientation basis (a real_matrix4x3's forward/left/up
 * rows) for one sprite/billboard according to its facing mode. Skipped entirely when the sprite group is
 * screen-space (data->flags bit 0). Mode 0 lays down the fixed screen-aligned basis. Mode 1 takes the sprite's
 * facing direction as forward and builds left = up_reference x forward. Mode 2 (velocity/axis aligned) picks a
 * world reference axis (world_up, or world_forward when the facing direction is nearly parallel to world_up),
 * forms forward = facing x reference and up = facing, then rotates the provisional left 90 degrees about up to
 * finish the frame.
 *
 * DEVIATION: the DB prototype lists a 3rd `unsigned int flags` and a trailing 6th `real_matrix4x3 *basis`
 * argument. Disasm (prologue mr r28,r5 / r31,r6 / r29,r7; no use of r8) shows r5 is the up-reference vector,
 * r6 the facing direction, r7 the output matrix, and r8 (`basis`) is never referenced (phantom) — reconstructed
 * as five parameters. The output `basis` is walked as a real_vector3d* alias over the real_matrix4x3: skipping
 * the leading scale float via `&basis->j`, so basis[0].j..=forward, basis[1].j..=left, basis[2].j..=up. */

#include <stdint.h>
#include "headers/build_sprite_data.h"
#include "headers/build_sprites_flags.h"
#include "headers/build_sprite_orientation.h"
#include "headers/build_sprite_globals.h"
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"

extern float normalize3d(real_vector3d *v);
extern void rotate_vector_about_axis(real_vector3d *v, const real_vector3d *n, float sine, float cosine);

void build_sprite_compute_basis(const build_sprite_data *data, int16_t mode, const real_vector3d *up_reference, const real_point3d *facing_direction, real_vector3d *basis)
{
    if ( (data->flags & (1u << _build_sprites_screen_space_bit)) != 0 )
        return;

    if ( mode == _build_sprite_normal )
    {
        basis->n[2] = 0.0f;
        basis[1].n[0] = 0.0f;
        basis[1].n[2] = 1.0f;
        basis[2].n[0] = 0.0f;
        basis->n[1] = 1.0f;
        basis[1].n[1] = 0.0f;
    }
    else if ( mode == _build_sprite_parallel )
    {
        *(real_point3d *)&basis->j = *facing_direction;
        normalize3d((real_vector3d *)&basis->j);

        float up_reference_y = up_reference->n[1];
        float forward_x = basis->n[1];
        float up_reference_z = up_reference->n[2];
        float forward_z = basis[1].n[0];
        float up_z_times_forward_y = (up_reference->n[2] * basis->n[2]);
        float forward_z_times_up_x = (basis[1].n[0] * up_reference->n[0]);

        basis[2].n[0] = (up_reference->n[0] * basis->n[2]) - (basis->n[1] * up_reference->n[1]);
        basis[1].n[1] = (forward_z * up_reference_y) - up_z_times_forward_y;
        basis[1].n[2] = (up_reference_z * forward_x) - forward_z_times_up_x;
        normalize3d((real_vector3d *)&basis[1].j);
    }
    else if ( mode == _build_sprite_perpendicular )
    {
        const real_vector3d *reference = &build_sprite_globals.viewer_space_world_up;
        float facing_dot_reference =
            ((facing_direction->n[2] * build_sprite_globals.viewer_space_world_up.n[2])
                + ((facing_direction->n[0] * build_sprite_globals.viewer_space_world_up.n[0])
                        + (facing_direction->n[1] * build_sprite_globals.viewer_space_world_up.n[1])));
        if ( (facing_dot_reference * facing_dot_reference)
             > (((facing_direction->n[2] * facing_direction->n[2])
                     + ((facing_direction->n[0] * facing_direction->n[0])
                             + (facing_direction->n[1] * facing_direction->n[1])))
                 * 0.99000001f) )
        {
            reference = &build_sprite_globals.viewer_space_world_forward;
        }

        float facing_y = facing_direction->n[1];
        float reference_x = reference->n[0];
        float facing_z = facing_direction->n[2];
        float reference_z = reference->n[2];
        float facing_z_times_reference_y = (facing_direction->n[2] * reference->n[1]);
        float facing_x_times_reference_z = (facing_direction->n[0] * reference->n[2]);

        basis[1].n[0] = (facing_direction->n[0] * reference->n[1])
                      - (facing_direction->n[1] * reference->n[0]);
        basis->n[1] = (facing_y * reference_z) - facing_z_times_reference_y;
        basis->n[2] = (facing_z * reference_x) - facing_x_times_reference_z;
        normalize3d((real_vector3d *)&basis->j);

        float forward_y = basis->n[2];
        float forward_z = basis[1].n[0];
        basis[1].n[1] = basis->n[1];
        basis[1].n[2] = forward_y;
        basis[2].n[0] = forward_z;
        basis[2].n[1] = facing_direction->n[0];
        basis[2].n[2] = facing_direction->n[1];
        basis[3].n[0] = facing_direction->n[2];
        normalize3d((real_vector3d *)&basis[2].j);

        rotate_vector_about_axis(
            (real_vector3d *)&basis[1].j, (const real_vector3d *)&basis[2].j, -1.0f, 0.0f);
    }
}
