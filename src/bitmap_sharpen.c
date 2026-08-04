/* bitmap_sharpen @0x8377DCF0 — build the shared 256-entry positive/negative sharpen lookup tables for
 * `sharpen_amount` (0..1, clamped/scaled to an integer strength 0..100) and dispatch to the type-specific
 * sharpener: bitmap_2d_sharpen for 2D bitmaps, or a warn-and-bail for 3D bitmaps / cube maps. No-op for
 * sharpen_amount <= 0. */

#include <stdint.h>
#include "headers/bitmap_data.h"
#include "headers/bitmap_type.h"
#include "headers/blam_data_globals.h"

extern __int16 bitmap_sharpen_negative_table[256];

extern void bitmap_2d_sharpen(bitmap_data *bitmap, const int16_t *positive_table, const int16_t *negative_table, float sharpen_amount);
extern void bitmap_3d_sharpen(bitmap_data *bitmap, float sharpen_amount, const int16_t *positive_table, const int16_t *negative_table);
extern void bitmap_cm_sharpen(bitmap_data *bitmap, float sharpen_amount, const int16_t *positive_table, const int16_t *negative_table);

void bitmap_sharpen(bitmap_data *bitmap, float sharpen_amount)
{
    if ( sharpen_amount <= 0.0f )
        return;

    __int16 strength = (__int16)(sharpen_amount * 100.0f);
    if ( strength < 0 )
        strength = 0;
    else if ( strength > 100 )
        strength = 100;

    __int16 falloff = 100 - strength;
    if ( falloff < 1 )
        falloff = 1;

    for ( __int16 i = 0; i < 256; ++i )
    {
        bitmap_sharpen_negative_table[i] = (__int16)(i * strength / 8 / falloff);
        positive_table[i] = (__int16)(100 * i / falloff);
    }

    if ( bitmap->type == bitmap_type_2d )
        bitmap_2d_sharpen(bitmap, positive_table, bitmap_sharpen_negative_table, sharpen_amount);  /* arg order corrected 2026-07-31: float is the last param (f1), pos/neg in r4/r5 */
    else if ( bitmap->type == bitmap_type_3d )
        bitmap_3d_sharpen(bitmap, sharpen_amount, positive_table, bitmap_sharpen_negative_table);
    else if ( bitmap->type < bitmap_type_white )
        bitmap_cm_sharpen(bitmap, sharpen_amount, positive_table, bitmap_sharpen_negative_table);
}
