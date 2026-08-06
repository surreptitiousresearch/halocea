/* fudge_vectors @ 0x82125F50 (.rdata, 324 bytes)
 * DB applied_types: const real_vector3d fudge_vectors[27];
 * Image bytes (big-endian), decoded from the binary .rdata record:
 *   [ 0] +0x00 n                          = 0x000000000000000000000000
 *        +0x00 __s1                       = 0x000000000000000000000000
 *   [ 1] +0x00 n                          = 0x3F8000000000000000000000
 *        +0x00 __s1                       = 0x3F8000000000000000000000
 *   [ 2] +0x00 n                          = 0x00000000000000003F800000
 *        +0x00 __s1                       = 0x00000000000000003F800000
 *   [ 3] +0x00 n                          = 0x3F3504F3000000003F3504F3
 *        +0x00 __s1                       = 0x3F3504F3000000003F3504F3
 *   [ 4] +0x00 n                          = 0x3F13CD3A3F13CD3A3F13CD3A
 *        +0x00 __s1                       = 0x3F13CD3A3F13CD3A3F13CD3A
 *   [ 5] +0x00 n                          = 0x3F13CD3ABF13CD3A3F13CD3A
 *        +0x00 __s1                       = 0x3F13CD3ABF13CD3A3F13CD3A
 *   [ 6] +0x00 n                          = 0x3F3504F33F3504F300000000
 *        +0x00 __s1                       = 0x3F3504F33F3504F300000000
 *   [ 7] +0x00 n                          = 0x3F3504F3BF3504F300000000
 *        +0x00 __s1                       = 0x3F3504F3BF3504F300000000
 *   [ 8] +0x00 n                          = 0x000000003F3504F33F3504F3
 *        +0x00 __s1                       = 0x000000003F3504F33F3504F3
 *   [ 9] +0x00 n                          = 0x00000000BF3504F33F3504F3
 *        +0x00 __s1                       = 0x00000000BF3504F33F3504F3
 *   [10] +0x00 n                          = 0xBF8000000000000000000000
 *        +0x00 __s1                       = 0xBF8000000000000000000000
 *   [11] +0x00 n                          = 0x000000003F80000000000000
 *        +0x00 __s1                       = 0x000000003F80000000000000
 *   [12] +0x00 n                          = 0x00000000BF80000000000000
 *        +0x00 __s1                       = 0x00000000BF80000000000000
 *   [13] +0x00 n                          = 0xBF3504F3BF3504F300000000
 *        +0x00 __s1                       = 0xBF3504F3BF3504F300000000
 *   [14] +0x00 n                          = 0xBF3504F33F3504F300000000
 *        +0x00 __s1                       = 0xBF3504F33F3504F300000000
 *   [15] +0x00 n                          = 0xBF3504F3000000003F3504F3
 *        +0x00 __s1                       = 0xBF3504F3000000003F3504F3
 *   [16] +0x00 n                          = 0xBF13CD3ABF13CD3A3F13CD3A
 *        +0x00 __s1                       = 0xBF13CD3ABF13CD3A3F13CD3A
 *   [17] +0x00 n                          = 0xBF13CD3A3F13CD3A3F13CD3A
 *        +0x00 __s1                       = 0xBF13CD3A3F13CD3A3F13CD3A
 *   [18] +0x00 n                          = 0x0000000000000000BF800000
 *        +0x00 __s1                       = 0x0000000000000000BF800000
 *   [19] +0x00 n                          = 0x3F3504F300000000BF3504F3
 *        +0x00 __s1                       = 0x3F3504F300000000BF3504F3
 *   ... 7 further elements elided; full hex in .sweep/data_image.tsv
 * .rdata, 324 bytes = 27 x real_vector3d (12 each), no padding.
 * The 3x3x3 lattice of nudge directions biped_fix_position walks when a biped's physics pill is
 * stuck: entry [0] is the null nudge, [1..17] are the 17 directions with a non-negative z (the
 * `fudge_count = 18` case), and [18..26] add the nine downward ones (`fudge_count = 27`, taken
 * only when fix_below_new_position is set). Each direction is unit length: axis vectors are +/-1,
 * edge vectors 0x3F3504F3 = 0.70710677 (1/sqrt 2), corner vectors 0x3F13CD3A = 0.57735026
 * (1/sqrt 3). Initializer reconstructed from the binary (big-endian words).
 * The 27-element extent is corroborated three ways: 324/12, the consumer's `fudge_count` of 27,
 * and the DB applied type `const real_vector3d fudge_vectors[27]`.
 * DEVIATION: src/biped_fix_position.c declares this `extern const real_vector3d fudge_vectors[]`
 * (incomplete type); the bound is 27.
 */
#include "../headers/real_vector3d.h"

const real_vector3d fudge_vectors[27] =
{
    { {  0.0f,         0.0f,         0.0f        } },   /* [ 0] +0x000 */
    { {  1.0f,         0.0f,         0.0f        } },   /* [ 1] +0x00C */
    { {  0.0f,         0.0f,         1.0f        } },   /* [ 2] +0x018 */
    { {  0.70710677f,  0.0f,         0.70710677f } },   /* [ 3] +0x024 */
    { {  0.57735026f,  0.57735026f,  0.57735026f } },   /* [ 4] +0x030 */
    { {  0.57735026f, -0.57735026f,  0.57735026f } },   /* [ 5] +0x03C */
    { {  0.70710677f,  0.70710677f,  0.0f        } },   /* [ 6] +0x048 */
    { {  0.70710677f, -0.70710677f,  0.0f        } },   /* [ 7] +0x054 */
    { {  0.0f,         0.70710677f,  0.70710677f } },   /* [ 8] +0x060 */
    { {  0.0f,        -0.70710677f,  0.70710677f } },   /* [ 9] +0x06C */
    { { -1.0f,         0.0f,         0.0f        } },   /* [10] +0x078 */
    { {  0.0f,         1.0f,         0.0f        } },   /* [11] +0x084 */
    { {  0.0f,        -1.0f,         0.0f        } },   /* [12] +0x090 */
    { { -0.70710677f, -0.70710677f,  0.0f        } },   /* [13] +0x09C */
    { { -0.70710677f,  0.70710677f,  0.0f        } },   /* [14] +0x0A8 */
    { { -0.70710677f,  0.0f,         0.70710677f } },   /* [15] +0x0B4 */
    { { -0.57735026f, -0.57735026f,  0.57735026f } },   /* [16] +0x0C0 */
    { { -0.57735026f,  0.57735026f,  0.57735026f } },   /* [17] +0x0CC */
    { {  0.0f,         0.0f,        -1.0f        } },   /* [18] +0x0D8 */
    { {  0.70710677f,  0.0f,        -0.70710677f } },   /* [19] +0x0E4 */
    { { -0.70710677f,  0.0f,        -0.70710677f } },   /* [20] +0x0F0 */
    { {  0.0f,         0.70710677f, -0.70710677f } },   /* [21] +0x0FC */
    { {  0.0f,        -0.70710677f, -0.70710677f } },   /* [22] +0x108 */
    { {  0.57735026f,  0.57735026f, -0.57735026f } },   /* [23] +0x114 */
    { {  0.57735026f, -0.57735026f, -0.57735026f } },   /* [24] +0x120 */
    { { -0.57735026f, -0.57735026f, -0.57735026f } },   /* [25] +0x12C */
    { { -0.57735026f,  0.57735026f, -0.57735026f } },   /* [26] +0x138 */
};
