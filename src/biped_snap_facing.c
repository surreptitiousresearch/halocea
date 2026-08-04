/* biped_snap_facing @0x837B0638 — re-orthogonalize a biped's forward/up frame to whatever surface it is
 * standing on, gated by definition flags and whether the biped is dead. Four mutually exclusive modes:
 *
 *   _biped_flying_bit (0x04, alive)          — "bank" mode: rebuild up by rolling the up axis around forward
 *                            by biped.bank.
 *   _biped_climbs_anything_bit (0x40, alive) — "align to plane" mode: snap up toward the support surface normal
 *                            (or the current up if unsupported), nudging by 10 degrees when the hemisphere test
 *                            fails, then re-derive forward orthogonal to that up.
 *   dead, not airborne     — tilt forward+up onto the ground-plane normal by the angle between them.
 *   otherwise              — flatten forward to horizontal and set up to global up.
 *
 * Reconstructed from disassembly (0x837B0638-0x837B0C34). Deviations from the decompiler: the trailing
 * `long double a3` parameter is a phantom (the DB prototype is 2-arg) — it was only ever used as an FPR scratch
 * slot for cos/sin/acos arguments, restored to plain double calls here. The `__int64`/`_QWORD` copies of
 * `forward.{j,k}` are transcribed as component copies. The object forward/up/damage_flags fields are now
 * reached through the modeled biped->object (_object_datum) sub-record; the biped/definition sub-records use
 * their modeled fields. Per-step (float) casts are kept for single-precision bit-exactness of the cross/dot
 * products. */

#include "headers/biped_datum.h"
#include "headers/biped_definition.h"
#include "headers/real_vector3d.h"
#include "headers/real_plane3d.h"
#include <math.h>
#include "headers/biped_datum_flags.h"
#include "headers/biped_definition_flags.h"
#include "headers/object_damage_flags.h"
#include "headers/blam_data_globals.h"

extern float __fabs(float x);
extern float normalize3d(real_vector3d *v);
extern void rotate_vector_about_axis(real_vector3d *v, const real_vector3d *n, float sine, float cosine);

void biped_snap_facing(biped_datum *biped, const biped_definition *biped_definition)
{
    /* recovered: (char*)biped+0x74/0x80/0x106 -> biped->object.forward/up/damage_flags (object now modeled) */
    real_vector3d *forward = &biped->object.forward;
    real_vector3d *up = &biped->object.up;
    unsigned int object_damage_flags = biped->object.damage_flags;
    unsigned int flags = biped_definition->biped.flags;

    if ( (flags & (1u << _biped_flying_bit)) != 0 && (object_damage_flags & (1u << _object_dead_bit)) == 0 )
    {
        /* ---- bank mode: roll up around forward by biped.bank ---- */
        float fwd0 = forward->n[0];
        float fwd1 = forward->n[1];
        float fwd2 = forward->n[2];
        float up_yz = global_up3d->n[2];
        float up_y  = global_up3d->n[1];
        float t10 = (global_up3d->n[1] * forward->n[2]);
        float t11 = (forward->n[1] * global_up3d->n[0]);
        float t12 = ((forward->n[2] * global_up3d->n[0])
                            - (global_up3d->n[2] * forward->n[0]));

        real_vector3d side;
        side.n[1] = (forward->n[2] * global_up3d->n[0]) - (global_up3d->n[2] * forward->n[0]);
        side.n[0] = (up_yz * fwd1) - t10;
        side.n[2] = (up_y * fwd0) - t11;

        real_vector3d roll_up;
        roll_up.n[2] = (fwd1 * side.n[0]) - (t12 * fwd0);
        roll_up.n[1] = (side.n[2] * fwd0) - (fwd2 * side.n[0]);
        roll_up.n[0] = (t12 * fwd2) - (side.n[2] * fwd1);

        if ( normalize3d(&roll_up) == 0.0f )
        {
            roll_up = *global_forward3d;
            side = *global_left3d;
        }

        float bank_cos = (float)cos(biped->biped.bank);
        float bank_sin = (float)sin(biped->biped.bank);
        float roll0 = (roll_up.n[0] * bank_cos);
        float roll1 = (roll_up.n[1] * bank_cos);
        float roll2 = (roll_up.n[2] * bank_cos);
        normalize3d(&side);
        up->n[0] = (side.n[0] * bank_sin) + roll0;
        up->n[1] = (side.n[1] * bank_sin) + roll1;
        up->n[2] = (side.n[2] * bank_sin) + roll2;
        return;
    }

    if ( (flags & (1u << _biped_climbs_anything_bit)) != 0 && (object_damage_flags & (1u << _object_dead_bit)) == 0 )
    {
        /* ---- align-to-plane mode ---- */
        int already_aligned = 0;
        real_vector3d reference;          /* the plane normal we snap up onto */
        real_vector3d plane_cross;        /* reference x up, the tilt axis */

        if ( biped->biped.support_surface_index == -1 )
        {
            /* no support surface: reference = current up */
            reference.n[0] = up->n[0];
            reference.n[1] = up->n[1];
            reference.n[2] = up->n[2];
            already_aligned = 1;
        }
        else
        {
            real_vector3d *ground_normal = &biped->biped.ground_plane.n;
            reference.n[0] = ground_normal->n[0];
            reference.n[1] = ground_normal->n[1];
            reference.n[2] = ground_normal->n[2];

            plane_cross.n[1] = (ground_normal->n[0] * up->n[2])
                             - (ground_normal->n[2] * up->n[0]);
            plane_cross.n[0] = (ground_normal->n[2] * up->n[1])
                             - (ground_normal->n[1] * up->n[2]);
            plane_cross.n[2] = (ground_normal->n[1] * up->n[0])
                             - (ground_normal->n[0] * up->n[1]);

            if ( normalize3d(&plane_cross) == 0.0f )
            {
                /* up parallel to the ground normal */
                if ( ((up->n[1] * ground_normal->n[1])
                             + ((ground_normal->n[0] * up->n[0])
                                       + (up->n[2] * ground_normal->n[2]))) <= 0.0f )
                {
                    plane_cross.n[0] = forward->n[0];
                    plane_cross.n[1] = forward->n[1];
                    plane_cross.n[2] = forward->n[2];
                }
                else
                {
                    already_aligned = 1;
                }
            }
        }

        if ( !already_aligned )
        {
            /* nudge up 10 degrees about plane_cross, accept if it improves the hemisphere test */
            real_vector3d rotated_up;
            rotated_up.n[0] = up->n[0];
            rotated_up.n[1] = up->n[1];
            rotated_up.n[2] = up->n[2];
            rotate_vector_about_axis(&rotated_up, &plane_cross,
                                     (float)sin(0.1745329201221466), (float)cos(0.1745329201221466));

            if ( ((((rotated_up.n[1] * reference.n[2])
                                         - (rotated_up.n[2] * reference.n[1])) * plane_cross.n[0])
                         + ((((reference.n[1] * rotated_up.n[0])
                                                   - (rotated_up.n[1] * reference.n[0])) * plane_cross.n[2])
                                   + (((rotated_up.n[2] * reference.n[0])
                                                     - (reference.n[2] * rotated_up.n[0])) * plane_cross.n[1]))) > 0.0f )
            {
                reference.n[0] = rotated_up.n[0];
                reference.n[1] = rotated_up.n[1];
                reference.n[2] = rotated_up.n[2];
            }
        }

        /* re-derive forward = reference x forward (fall back to reference x up, then to the globals) */
        real_vector3d oriented_forward;
        {
            float c46 = ((reference.n[1] * forward->n[0])
                                - (reference.n[0] * forward->n[1]));
            float c47 = (((reference.n[0] * forward->n[2])
                                        - (reference.n[2] * forward->n[0])) * reference.n[2]);
            oriented_forward.n[2] = (((reference.n[0] * forward->n[2])
                                                    - (reference.n[2] * forward->n[0])) * reference.n[0])
                                  - (reference.n[1]
                                            * ((reference.n[2] * forward->n[1])
                                                      - (reference.n[1] * forward->n[2])));
            oriented_forward.n[1] = (reference.n[2]
                                            * ((reference.n[2] * forward->n[1])
                                                      - (reference.n[1] * forward->n[2])))
                                  - (((reference.n[1] * forward->n[0])
                                                    - (reference.n[0] * forward->n[1])) * reference.n[0]);
            oriented_forward.n[0] = (c46 * reference.n[1]) - c47;
        }

        int total_failure = 0;
        if ( normalize3d(&oriented_forward) == 0.0f )
        {
            float d49 = ((reference.n[1] * up->n[2])
                                - (reference.n[2] * up->n[1]));
            float d50 = (((up->n[1] * reference.n[0])
                                        - (reference.n[1] * up->n[0])) * reference.n[0]);
            oriented_forward.n[0] = (((up->n[1] * reference.n[0])
                                                    - (reference.n[1] * up->n[0])) * reference.n[1])
                                  - (((reference.n[2] * up->n[0])
                                                    - (reference.n[0] * up->n[2])) * reference.n[2]);
            oriented_forward.n[2] = (((reference.n[2] * up->n[0])
                                                    - (reference.n[0] * up->n[2])) * reference.n[0])
                                  - (reference.n[1] * d49);
            oriented_forward.n[1] = (reference.n[2] * d49) - d50;

            if ( normalize3d(&oriented_forward) == 0.0f )
                total_failure = 1;
        }

        if ( total_failure )
        {
            reference.n[0] = global_up3d->n[0];
            reference.n[1] = global_up3d->n[1];
            reference.n[2] = global_up3d->n[2];
            forward->n[0] = global_forward3d->n[0];
            forward->n[1] = global_forward3d->n[1];
            forward->n[2] = global_forward3d->n[2];
        }
        else
        {
            forward->n[0] = oriented_forward.n[0];
            forward->n[1] = oriented_forward.n[1];
            forward->n[2] = oriented_forward.n[2];
        }

        up->n[0] = reference.n[0];
        up->n[1] = reference.n[1];
        up->n[2] = reference.n[2];
        return;
    }

    if ( (object_damage_flags & (1u << _object_dead_bit)) != 0 && (biped->biped.flags & (1u << _biped_airborne_bit)) == 0 )
    {
        /* ---- dead on a slope: rotate forward+up onto the ground normal ---- */
        real_vector3d *ground_normal = &biped->biped.ground_plane.n;
        float alignment = ((up->n[0] * ground_normal->n[0])
                                  + ((ground_normal->n[2] * up->n[2])
                                            + (ground_normal->n[1] * up->n[1])));
        if ( __fabs((alignment - 1.0f)) >= 0.000099999997f )
        {
            float angle = (float)acos((double)alignment);
            if ( angle != 0.0f )
            {
                real_vector3d axis;
                axis.n[0] = (ground_normal->n[2] * up->n[1]) - (ground_normal->n[1] * up->n[2]);
                axis.n[1] = (up->n[2] * ground_normal->n[0])
                          - (ground_normal->n[2] * up->n[0]);
                axis.n[2] = (ground_normal->n[1] * up->n[0]) - (up->n[1] * ground_normal->n[0]);
                if ( normalize3d(&axis) != 0.0f )
                {
                    rotate_vector_about_axis(up, &axis, (float)sin((double)angle), (float)cos((double)angle));
                    rotate_vector_about_axis(forward, &axis, (float)sin((double)angle), (float)cos((double)angle));
                    normalize3d(up);
                    normalize3d(forward);
                }
            }
        }
        return;
    }

    /* ---- default: flatten forward to horizontal, up = global up ---- */
    forward->n[2] = 0.0f;
    if ( normalize3d(forward) == 0.0f )
    {
        forward->n[0] = global_forward3d->n[0];
        forward->n[1] = global_forward3d->n[1];
        forward->n[2] = global_forward3d->n[2];
    }
    *up = *global_up3d;
}
