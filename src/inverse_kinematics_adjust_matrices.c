/* inverse_kinematics_adjust_matrices @0x83795C00 — analytic two-bone inverse kinematics. Given a desired
 * world position for the hand (only the position row of desired_hand_matrix is used as input) and the current
 * shoulder/elbow/hand bone matrices, solve for a bent-arm pose that reaches the target:
 *   1. Measure the current segment lengths (shoulder->elbow = upper arm, elbow->hand = forearm) and the
 *      distance from the shoulder to the desired hand position.
 *   2. Build a bend plane from the shoulder->target direction and the current shoulder->elbow vector, so the
 *      solved elbow keeps bending the same way it does now.
 *   3. If the target is farther than the arm can reach (0.98 * (upper + forearm)), pull the target in along
 *      the shoulder->target direction until it is reachable.
 *   4. Place the elbow by the law of cosines: project it onto the shoulder->target axis and lift it off the
 *      axis by the triangle height, within the bend plane.
 *   5. Rebuild the shoulder and elbow orientation matrices so each bone's forward axis (row 0) points at the
 *      next joint, then Gram-Schmidt orthonormalize (up = forward x left, left = up x forward) to preserve
 *      each bone's roll. Reposition the elbow, and copy the (clamped) target into the hand matrix.
 *
 * real_matrix4x3 rows: n[0]=forward, n[1]=left, n[2]=up, n[3]=position. The decompiler names the row union
 * $8720DA82371E61A926AF87C1ED38CB66; that is just real_matrix4x3's anonymous row union, so plain ->n[i][j]
 * and ->n[3][k] accesses are used here.
 *
 * Clean decompile (no reg-alloc failure); verified against disasm 0x83795C00-0x83796038.
 *
 * DEVIATIONS:
 *  - Every arithmetic sub-expression keeps the decompiler's (float) casts to preserve the PPC single-precision
 *    rounding at each step.
 *  - The unit shoulder->target direction and 1/target_dist are computed once and reused; the decompiler shows
 *    them re-inlined at each use, but they are single SSA register values (the reach clamp overwrites the
 *    target position row, not the already-computed direction registers).
 *  - The orthonormalization blocks snapshot each row's old components into locals before overwriting matrix
 *    cells in place, matching the disassembly and avoiding read-after-write hazards.
 *  - The new elbow position is computed from the final shoulder frame and buffered (the shipped code stages it
 *    through a stack qword/float) before being stored after the elbow basis is rebuilt. */

#include "headers/real_matrix4x3.h"

extern float __fsqrts(float x);
extern float fabsf(float x);
extern void *memcpy(void *dst, const void *src, unsigned int size);

void inverse_kinematics_adjust_matrices(real_matrix4x3 *desired_hand_matrix, real_matrix4x3 *shoulder_matrix,
        real_matrix4x3 *elbow_matrix, real_matrix4x3 *hand_matrix)
{
    /* --- segment lengths and the shoulder->target distance --- */
    float to_shoulder_y = (shoulder_matrix->n[3][1] - desired_hand_matrix->n[3][1]);
    float to_shoulder_z = (shoulder_matrix->n[3][2] - desired_hand_matrix->n[3][2]);
    float to_shoulder_x = (shoulder_matrix->n[3][0] - desired_hand_matrix->n[3][0]);

    float upper_dy = (elbow_matrix->n[3][1] - shoulder_matrix->n[3][1]);
    float forearm_dy = (hand_matrix->n[3][1] - elbow_matrix->n[3][1]);
    float upper_dz = (elbow_matrix->n[3][2] - shoulder_matrix->n[3][2]);
    float forearm_dz = (hand_matrix->n[3][2] - elbow_matrix->n[3][2]);
    float upper_dx = (elbow_matrix->n[3][0] - shoulder_matrix->n[3][0]);
    float forearm_dx = (hand_matrix->n[3][0] - elbow_matrix->n[3][0]);

    float target_dist = __fsqrts(((to_shoulder_x * to_shoulder_x)
            + ((to_shoulder_z * to_shoulder_z)
                    + (to_shoulder_y * to_shoulder_y))));
    float upper_arm_length = __fsqrts(((upper_dx * upper_dx)
            + ((upper_dz * upper_dz) + (upper_dy * upper_dy))));
    float forearm_length = __fsqrts(((forearm_dx * forearm_dx)
            + ((forearm_dz * forearm_dz) + (forearm_dy * forearm_dy))));

    /* unit direction shoulder -> desired hand (from the original target, before any reach clamp) */
    float inv_target_dist = ((float)1.0 / target_dist);
    float dir_x = ((desired_hand_matrix->n[3][0] - shoulder_matrix->n[3][0]) * inv_target_dist);
    float dir_y = ((desired_hand_matrix->n[3][1] - shoulder_matrix->n[3][1]) * inv_target_dist);
    float dir_z = ((desired_hand_matrix->n[3][2] - shoulder_matrix->n[3][2]) * inv_target_dist);

    /* bend-plane normal = dir x (shoulder->elbow); component order is (x=plane_normal_x, y, z) */
    float plane_normal_x = ((dir_y * upper_dz) - (dir_z * upper_dy));
    float plane_normal_y = ((dir_z * upper_dx) - (dir_x * upper_dz));
    float plane_normal_z = ((dir_x * upper_dy) - (dir_y * upper_dx));

    float normal_length = __fsqrts(((plane_normal_x * plane_normal_x)
            + ((plane_normal_z * plane_normal_z)
                    + (plane_normal_y * plane_normal_y))));
    if ( fabsf(normal_length) >= 0.0001f )
    {
        float inv_normal_length = ((float)1.0 / normal_length);
        plane_normal_x = (inv_normal_length
                * ((dir_y * upper_dz) - (dir_z * upper_dy)));
        plane_normal_y = (((dir_z * upper_dx) - (dir_x * upper_dz))
                * inv_normal_length);
        plane_normal_z = (((dir_x * upper_dy) - (dir_y * upper_dx))
                * inv_normal_length);
    }

    /* in-plane bend axis = plane_normal x dir (perpendicular to dir, in the bend plane) */
    float bend_axis_z = ((plane_normal_x * dir_y) - (plane_normal_y * dir_x));
    float bend_axis_x = ((plane_normal_y * dir_z) - (plane_normal_z * dir_y));
    float bend_axis_y = ((plane_normal_z * dir_x) - (plane_normal_x * dir_z));

    /* reach clamp: if the target is beyond 0.98 of full extension, pull it in along dir */
    if ( ((forearm_length + upper_arm_length) * (float)0.98000002) < target_dist )
    {
        float reachable_dist = ((forearm_length + upper_arm_length) * (float)0.98000002);
        desired_hand_matrix->n[3][0] = (reachable_dist * dir_x) + shoulder_matrix->n[3][0];
        desired_hand_matrix->n[3][1] = (reachable_dist * dir_y) + shoulder_matrix->n[3][1];
        target_dist = reachable_dist;
        desired_hand_matrix->n[3][2] = (reachable_dist * dir_z) + shoulder_matrix->n[3][2];
    }

    /* law of cosines: elbow projects onto the shoulder->target axis at elbow_projection, lifted by elbow_height */
    float elbow_projection = (-((forearm_length * forearm_length)
                    - ((target_dist * target_dist)
                            + (upper_arm_length * upper_arm_length)))
            / (target_dist * (float)2.0));
    float axis_to_hand_remainder = (target_dist - elbow_projection);
    float elbow_height = __fsqrts(-((elbow_projection * elbow_projection)
            - (upper_arm_length * upper_arm_length)));

    /* --- shoulder frame: row 0 (forward) points shoulder -> new elbow = projection*dir + height*bend_axis --- */
    float shoulder_forward_y = ((elbow_projection * dir_y)
            + (bend_axis_y * elbow_height));
    shoulder_matrix->n[0][1] = shoulder_forward_y;
    float shoulder_forward_z = ((elbow_projection * dir_z)
            + (bend_axis_z * elbow_height));
    shoulder_matrix->n[0][2] = shoulder_forward_z;
    float shoulder_forward_x = ((elbow_projection * dir_x)
            + (bend_axis_x * elbow_height));
    shoulder_matrix->n[0][0] = shoulder_forward_x;

    float shoulder_forward_length = __fsqrts(((shoulder_forward_x * shoulder_forward_x)
            + ((shoulder_forward_z * shoulder_forward_z)
                    + (shoulder_forward_y * shoulder_forward_y))));
    if ( fabsf(shoulder_forward_length) >= 0.0001f )
    {
        float inv_forward_length = ((float)1.0 / shoulder_forward_length);
        shoulder_matrix->n[0][0] = shoulder_forward_x * inv_forward_length;
        shoulder_matrix->n[0][1] = shoulder_forward_y * inv_forward_length;
        shoulder_matrix->n[0][2] = shoulder_forward_z * inv_forward_length;
    }

    /* shoulder row 2 (up) = forward x left, using the original row 1 (left); snapshot old cells first */
    float s_forward_x = shoulder_matrix->n[0][0];
    float s_old_left_z = shoulder_matrix->n[1][2];
    float s_old_left_y = shoulder_matrix->n[1][1];
    float s_left_y_forward_z = (shoulder_matrix->n[1][1] * shoulder_matrix->n[0][2]);
    float s_forward_y = shoulder_matrix->n[0][1];
    float s_left_x_forward_y = (shoulder_matrix->n[1][0] * shoulder_matrix->n[0][1]);
    float shoulder_up_y = ((shoulder_matrix->n[1][0] * shoulder_matrix->n[0][2])
            - (shoulder_matrix->n[1][2] * shoulder_matrix->n[0][0]));
    shoulder_matrix->n[2][1] = shoulder_up_y;
    float shoulder_up_x = ((s_old_left_z * s_forward_y) - s_left_y_forward_z);
    shoulder_matrix->n[2][0] = shoulder_up_x;
    float shoulder_up_z = ((s_old_left_y * s_forward_x) - s_left_x_forward_y);
    shoulder_matrix->n[2][2] = shoulder_up_z;

    float shoulder_up_length = __fsqrts(((shoulder_up_z * shoulder_up_z)
            + ((shoulder_up_x * shoulder_up_x) + (shoulder_up_y * shoulder_up_y))));
    if ( fabsf(shoulder_up_length) >= 0.0001f )
    {
        float inv_up_length = ((float)1.0 / shoulder_up_length);
        shoulder_matrix->n[2][0] = shoulder_up_x * inv_up_length;
        shoulder_matrix->n[2][1] = inv_up_length * shoulder_up_y;
        shoulder_matrix->n[2][2] = inv_up_length * shoulder_up_z;
    }

    /* elbow row 0 (forward) points elbow -> hand = remainder*dir - height*bend_axis (computed here, stored below) */
    float elbow_forward_y = ((axis_to_hand_remainder * dir_y)
            - (bend_axis_y * elbow_height));
    float s_up_z = shoulder_matrix->n[2][2];
    float s_forward_y2 = shoulder_matrix->n[0][1];
    float elbow_forward_z = ((axis_to_hand_remainder * dir_z)
            - (bend_axis_z * elbow_height));
    float elbow_forward_x = ((axis_to_hand_remainder * dir_x)
            - (bend_axis_x * elbow_height));

    /* shoulder row 1 (left) = up x forward; snapshot old up/forward cells first */
    float s_forward_x2 = shoulder_matrix->n[0][0];
    float s_up_x = shoulder_matrix->n[2][0];
    float s_upy_forwardx = (shoulder_matrix->n[2][1] * shoulder_matrix->n[0][0]);
    float s_upx_forwardz = (shoulder_matrix->n[2][0] * shoulder_matrix->n[0][2]);
    shoulder_matrix->n[1][0] = (shoulder_matrix->n[2][1] * shoulder_matrix->n[0][2])
            - (shoulder_matrix->n[2][2] * shoulder_matrix->n[0][1]);
    shoulder_matrix->n[1][2] = (s_up_x * s_forward_y2) - s_upy_forwardx;
    shoulder_matrix->n[1][1] = (s_up_z * s_forward_x2) - s_upx_forwardz;

    /* stage the new elbow position from the final shoulder frame: elbow = shoulder_pos + forward * upper_arm */
    float s_final_forward_x = shoulder_matrix->n[0][0];
    float shoulder_pos_x = shoulder_matrix->n[3][0];
    float s_final_forward_y = shoulder_matrix->n[0][1];
    float shoulder_pos_y = shoulder_matrix->n[3][1];
    float s_final_forward_z = shoulder_matrix->n[0][2];
    float shoulder_pos_z = shoulder_matrix->n[3][2];

    /* store elbow row 0 (forward), then normalize */
    elbow_matrix->n[0][0] = elbow_forward_x;
    elbow_matrix->n[0][1] = elbow_forward_y;
    elbow_matrix->n[0][2] = elbow_forward_z;
    float elbow_forward_length = __fsqrts(((elbow_forward_x * elbow_forward_x)
            + ((elbow_forward_z * elbow_forward_z) + (elbow_forward_y * elbow_forward_y))));

    float new_elbow_pos_x = (s_final_forward_x * upper_arm_length) + shoulder_pos_x;
    float new_elbow_pos_y = (s_final_forward_y * upper_arm_length) + shoulder_pos_y;
    float new_elbow_pos_z = (upper_arm_length * s_final_forward_z) + shoulder_pos_z;

    if ( fabsf(elbow_forward_length) >= 0.0001f )
    {
        float inv_elbow_forward_length = ((float)1.0 / elbow_forward_length);
        elbow_matrix->n[0][0] = elbow_forward_x * inv_elbow_forward_length;
        elbow_matrix->n[0][1] = elbow_forward_y * inv_elbow_forward_length;
        elbow_matrix->n[0][2] = elbow_forward_z * inv_elbow_forward_length;
    }

    /* elbow row 2 (up) = forward x left, using the original row 1 (left); snapshot old cells first */
    float e_forward_y = elbow_matrix->n[0][1];
    float e_old_left_x = elbow_matrix->n[1][0];
    float e_old_left_z = elbow_matrix->n[1][2];
    float e_leftz_forwardx = (elbow_matrix->n[1][2] * elbow_matrix->n[0][0]);
    float e_forward_z = elbow_matrix->n[0][2];
    float e_lefty_forwardz = (elbow_matrix->n[1][1] * elbow_matrix->n[0][2]);
    float elbow_up_z = ((elbow_matrix->n[1][1] * elbow_matrix->n[0][0])
            - (elbow_matrix->n[1][0] * elbow_matrix->n[0][1]));
    elbow_matrix->n[2][2] = elbow_up_z;
    float elbow_up_y = ((e_old_left_x * e_forward_z) - e_leftz_forwardx);
    elbow_matrix->n[2][1] = elbow_up_y;
    float elbow_up_x = ((e_old_left_z * e_forward_y) - e_lefty_forwardz);
    elbow_matrix->n[2][0] = elbow_up_x;

    float elbow_up_length = __fsqrts(((elbow_up_x * elbow_up_x)
            + ((elbow_up_y * elbow_up_y) + (elbow_up_z * elbow_up_z))));
    if ( fabsf(elbow_up_length) >= 0.0001f )
    {
        float inv_elbow_up_length = ((float)1.0 / elbow_up_length);
        elbow_matrix->n[2][0] = inv_elbow_up_length * ((e_old_left_z * e_forward_y) - e_lefty_forwardz);
        elbow_matrix->n[2][1] = inv_elbow_up_length * elbow_up_y;
        elbow_matrix->n[2][2] = inv_elbow_up_length * elbow_up_z;
    }

    /* elbow row 1 (left) = up x forward; snapshot old up/forward cells first */
    float e_up_y = elbow_matrix->n[2][1];
    float e_forward_x = elbow_matrix->n[0][0];
    float e_up_z = elbow_matrix->n[2][2];
    float e_forward_z2 = elbow_matrix->n[0][2];
    float e_forwardy_upz = (elbow_matrix->n[0][1] * elbow_matrix->n[2][2]);
    float e_forwardz_upx = (elbow_matrix->n[0][2] * elbow_matrix->n[2][0]);
    elbow_matrix->n[1][2] = (elbow_matrix->n[0][1] * elbow_matrix->n[2][0])
            - (elbow_matrix->n[0][0] * elbow_matrix->n[2][1]);
    elbow_matrix->n[1][0] = (e_forward_z2 * e_up_y) - e_forwardy_upz;
    elbow_matrix->n[1][1] = (e_forward_x * e_up_z) - e_forwardz_upx;

    /* commit the staged elbow position and copy the (clamped) target into the hand matrix */
    elbow_matrix->n[3][0] = new_elbow_pos_x;
    elbow_matrix->n[3][1] = new_elbow_pos_y;
    elbow_matrix->n[3][2] = new_elbow_pos_z;
    memcpy(hand_matrix, desired_hand_matrix, sizeof(real_matrix4x3));
}
