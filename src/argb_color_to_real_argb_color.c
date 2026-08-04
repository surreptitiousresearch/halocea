/* argb_color_to_real_argb_color @0x8377D73C — converts a packed 16-bit-per-channel ARGB color to a
 * normalized-float real_argb_color (each channel * 1/65536).
 *
 * DEVIATION: the decompiler rendered this as int64-register-pair (HIDWORD/LODWORD) punning artifacts from
 * the PPC soft-float calling convention; this is the straightforward per-channel reconstruction. */

#include "headers/argb_color.h"
#include "headers/real_argb_color.h"

real_argb_color *argb_color_to_real_argb_color(const argb_color *argb, real_argb_color *real_argb)
{
    real_argb->n[0] = (float)argb->n[0] * 0.000015259022f;
    real_argb->n[1] = (float)argb->n[1] * 0.000015259022f;
    real_argb->n[2] = (float)argb->n[2] * 0.000015259022f;
    real_argb->n[3] = (float)argb->n[3] * 0.000015259022f;
    return real_argb;
}
