/* bitmap_3d_sharpen @0x8377BB10 — 3D bitmaps don't support sharpening; warn and bail. */

#include <stdint.h>
#include "headers/bitmap_data.h"

#include <stdio.h>
typedef struct _iobuf _iobuf;

extern _iobuf *__iob_func(void);
extern int fprintf(_iobuf *stream, const char *format, ...);
extern int fflush(_iobuf *stream);

void bitmap_3d_sharpen(bitmap_data *bitmap, float sharpen_amount, const int16_t *positive_table, const int16_t *negative_table)
{
    fprintf(__iob_func() + 1, "### WARNING tried to sharpen a 3d bitmap");
    fflush(__iob_func() + 1);
}
