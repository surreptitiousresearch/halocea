/* bitmap_smooth @0x8377DB90 — apply a binomial (Pascal's-triangle) smoothing filter to a bitmap.
 * filter_size is floored to an integer half-width; if positive, builds a 10-entry filter_coefficients
 * kernel by repeated adjacent-sum convolution passes (2*filter_size+1 passes over a zeroed kernel seeded
 * with a leading 1 each pass — the standard iterative binomial-coefficient construction), then dispatches
 * to bitmap_3d_smooth/bitmap_2d_smooth depending on bitmap type (warns and does nothing for cube maps).
 * No-op if filter_size floors to <= 0.
 *
 * The Hex-Rays decompiler flagged this function's local variable allocation as failed; verified against
 * disasm (the float-to-int conversion, kernel zero/build loops, and dispatch all check out) rather than
 * trusted as-is. */

#include <stdint.h>
#include "headers/bitmap_data.h"
#include "headers/bitmap_type.h"
#include "headers/blam_data_globals.h"

typedef struct _iobuf _iobuf;

extern _iobuf *__iob_func(void);
extern int fprintf(_iobuf *stream, const char *format, ...);
extern int fflush(_iobuf *stream);

extern float floor(float x);
extern void bitmap_3d_smooth(bitmap_data *bitmap, int16_t filter_size, const int16_t *filter_coefficients);
extern void bitmap_2d_smooth(bitmap_data *bitmap, int16_t filter_size, const int16_t *filter_coefficients);

void bitmap_smooth(bitmap_data *bitmap, float filter_size)
{
    int16_t half_width = (int16_t)(int)floor(filter_size);
    if ( half_width <= 0 )
        return;

    for ( int i = 0; i < 10; ++i )
        filter_coefficients[i] = 0;

    for ( int16_t pass = (int16_t)(2 * half_width); pass >= 0; --pass )
    {
        for ( int i = 9; i > 0; --i )
            filter_coefficients[i] += filter_coefficients[i - 1];
        filter_coefficients[0] = 1;
    }

    uint16_t type = (uint16_t)bitmap->type;

    if ( bitmap->type == bitmap_type_2d )
    {
        bitmap_2d_smooth(bitmap, half_width, filter_coefficients);
    }
    else if ( type == bitmap_type_3d )
    {
        bitmap_3d_smooth(bitmap, half_width, filter_coefficients);
    }
    else if ( type < bitmap_type_white )
    {
        fprintf(__iob_func() + 1, "### WARNING tried to smooth a cube map");
        fflush(__iob_func() + 1);
    }
}
