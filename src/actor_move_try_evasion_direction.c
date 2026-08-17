/* actor_move_try_evasion_direction @0x837C76B0 — pick a 2D evasion vector for an actor relative to its
 * alignment (facing) vector according to an in/out direction code, then try to path it via
 * actor_move_try_evasion_vector, flipping to the opposite side and retrying up to attempt_limit times.
 *
 * The direction code (read from and written back through evade_direction_reference) selects the candidate:
 *   0 -> perpendicular (-alignment.j, +alignment.i)      [rotate +90]
 *   1 -> perpendicular ( alignment.j, -alignment.i)      [rotate -90]
 *   2 -> parallel      ( alignment.i,  alignment.j)      [straight ahead]
 *   3 -> opposite      (-alignment.i, -alignment.j)      [reverse]
 *   4 -> random: coin-flip between the two perpendiculars (0/1), attempt_limit becomes 2
 *   >4 -> pathological: uses the uninitialized scratch vector (reproduced faithfully; should not occur)
 * On success the surviving direction code is written back; on failure -1 is written. Returns whether any
 * candidate pathed successfully.
 *
 * DEVIATION 1 (FPR-shadow display bug, catalog class 1): Hex-Rays printed the r6 read/write as
 * `evasion_is_ledge->collision`, but disasm (0x837C76C4 `lhz r30,0(r6)`, 0x837C7858 `sth r11,0(r25)` with
 * r25=r6) shows it is *evade_direction_reference. The float args evade_distance (f1) and maximum_ledge_height
 * (f2) burn the r5/r7 GPR shadow slots, so the DB prototype's register-to-parameter mapping is otherwise
 * correct — r6=evade_direction_reference, r8=evasion_is_ledge, r9=result.
 * DEVIATION 2: the DB prototype types evade_direction_reference as unsigned __int8*, but the accesses are
 * halfword (lhz/sth); typed here as short* to match the binary.
 * DEVIATION 3: the actor_move_try_evasion_vector call passes evasion_is_ledge (r7) and result (r8); the
 * decompiler mislabeled these using the scrambled parent parameter names.
 * DEVIATION 4: the DB types evasion_is_ledge (r8) as path_collision_result*, but it is forwarded verbatim
 * as actor_move_try_evasion_vector's evasion_is_ledge_reference (0x837C77E4 mr r7,r27), through which that
 * callee stores a byte (0x837C7698 stb) — typed uint8_t* here. */

#include <stdint.h>
#include "headers/real_vector2d.h"
#include "headers/path_collision_result.h"

extern uint32_t *get_global_random_seed_address(void);
extern uint16_t seed_random(uint32_t *seed);
extern uint8_t actor_move_try_evasion_vector(int actor_index, real_vector2d *evasion_vector, float evade_distance, float maximum_ledge_height, uint8_t *evasion_is_ledge_reference, path_collision_result *result);

uint8_t actor_move_try_evasion_direction(int actor_index, real_vector2d *alignment_vector, float evade_distance, int16_t *evade_direction_reference, float maximum_ledge_height, uint8_t *evasion_is_ledge, path_collision_result *result)
{
    int16_t direction_code = *evade_direction_reference;
    int evasion_succeeded = 0;
    int16_t attempt_limit = 1;
    real_vector2d evasion_vector;
    float evasion_i, evasion_j;

    if ( (unsigned int)direction_code > 4 )
    {
        /* invalid direction code: proceed with the uninitialized scratch vector, as the binary does */
        evasion_j = evasion_vector.n[1];
        evasion_i = evasion_vector.n[0];
    }
    else if ( direction_code == 1 )
    {
        evasion_i = alignment_vector->n[1];
        evasion_j = -alignment_vector->n[0];
        evasion_vector.n[0] = alignment_vector->n[1];
        evasion_vector.n[1] = evasion_j;
    }
    else if ( direction_code == 2 )
    {
        evasion_i = alignment_vector->n[0];
        evasion_j = alignment_vector->n[1];
        evasion_vector.n[0] = alignment_vector->n[0];
        evasion_vector.n[1] = evasion_j;
    }
    else if ( direction_code == 3 )
    {
        evasion_i = -alignment_vector->n[0];
        evasion_j = -alignment_vector->n[1];
        evasion_vector.n[0] = evasion_i;
        evasion_vector.n[1] = evasion_j;
    }
    else if ( direction_code ) /* direction_code == 4: random perpendicular */
    {
        unsigned int *global_random_seed_address = get_global_random_seed_address();
        if ( seed_random(global_random_seed_address) <= 0x8000u )
        {
            direction_code = 1;
            evasion_i = alignment_vector->n[1];
            evasion_j = -alignment_vector->n[0];
            attempt_limit = 2;
            evasion_vector.n[0] = alignment_vector->n[1];
            evasion_vector.n[1] = evasion_j;
        }
        else
        {
            direction_code = 0;
            evasion_j = alignment_vector->n[0];
            evasion_i = -alignment_vector->n[1];
            attempt_limit = 2;
            evasion_vector.n[1] = alignment_vector->n[0];
            evasion_vector.n[0] = evasion_i;
        }
    }
    else /* direction_code == 0: perpendicular */
    {
        evasion_j = alignment_vector->n[0];
        evasion_i = -alignment_vector->n[1];
        evasion_vector.n[1] = alignment_vector->n[0];
        evasion_vector.n[0] = evasion_i;
    }

    int attempt_total = attempt_limit;
    int16_t attempt_index = 0;
    if ( attempt_limit > 0 )
    {
        while ( !actor_move_try_evasion_vector(actor_index, &evasion_vector, evade_distance,
                    maximum_ledge_height, evasion_is_ledge, result) )
        {
            evasion_i = -evasion_i;
            evasion_j = -evasion_j;
            evasion_vector.n[0] = evasion_i;
            evasion_vector.n[1] = evasion_j;
            attempt_index = (int16_t)(attempt_index + 1);
            direction_code ^= 1;
            if ( attempt_index >= attempt_total )
                goto write_back;
        }
        evasion_succeeded = 1;
    }

write_back:;
    int16_t result_direction_code = direction_code;
    if ( !(uint8_t)evasion_succeeded )
        result_direction_code = -1;
    *evade_direction_reference = result_direction_code;
    return evasion_succeeded;
}
