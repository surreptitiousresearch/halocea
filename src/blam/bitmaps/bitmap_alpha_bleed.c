/* bitmap_alpha_bleed @0x8377DE60 — dispatches an alpha-bleed pass by bitmap type: 2D bitmaps go through
 * bitmap_2d_alpha_bleed, 3D (volume) bitmaps through bitmap_3d_alpha_bleed; cube maps aren't supported and
 * just warn (matching bitmap_cm_alpha_bleed.c's inline warning). No-op if `passes` isn't positive. */

#include <stdint.h>
#include "headers/bitmap_data.h"
#include "headers/bitmap_type.h"

#include <stdio.h>
typedef struct _iobuf _iobuf;

extern _iobuf *__iob_func(void);
/* fprintf provided by CRT via <stdio.h>; local extern removed (C28251: the local redeclaration drops the header's annotations) */
/* fflush provided by CRT via <stdio.h>; local extern removed (C28251: the local redeclaration drops the header's annotations) */
extern void bitmap_2d_alpha_bleed(bitmap_data *bitmap, int16_t passes);
extern void bitmap_3d_alpha_bleed(bitmap_data *bitmap, int16_t passes);

void bitmap_alpha_bleed(bitmap_data *bitmap, int16_t passes)
{
    if ( passes <= 0 )
        return;

    if ( !bitmap->type )
    {
        bitmap_2d_alpha_bleed(bitmap, passes);
    }
    else if ( bitmap->type == bitmap_type_3d )
    {
        bitmap_3d_alpha_bleed(bitmap, passes);
    }
    else if ( bitmap->type < bitmap_type_white )
    {
        fprintf(__iob_func() + 1, "### WARNING tried to alpha-bleed a cube map (skipping)");
        fflush(__iob_func() + 1);
    }
}
