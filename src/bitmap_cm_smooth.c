/* bitmap_cm_smooth @0x8377B650 — cube maps don't support smoothing; warn and bail. */

#include <stdint.h>
#include "headers/bitmap_data.h"

#include <stdio.h>
typedef struct _iobuf _iobuf;

extern _iobuf *__iob_func(void);
extern int fprintf(_iobuf *stream, const char *format, ...);
extern int fflush(_iobuf *stream);

void bitmap_cm_smooth(bitmap_data *bitmap, int16_t filter_size, const int16_t *filter_coefficients)
{
    fprintf(__iob_func() + 1, "### WARNING tried to smooth a cube map");
    fflush(__iob_func() + 1);
}
