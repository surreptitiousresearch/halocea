/* bitmap_cm_sharpen @0x8377BB58 — cube maps don't support sharpening; warn and bail. */

#include <stdint.h>
#include "headers/bitmap_data.h"

#include <stdio.h>
typedef struct _iobuf _iobuf;

extern _iobuf *__iob_func(void);
/* fprintf provided by CRT via <stdio.h>; local extern removed (C28251: the local redeclaration drops the header's annotations) */
/* fflush provided by CRT via <stdio.h>; local extern removed (C28251: the local redeclaration drops the header's annotations) */

void bitmap_cm_sharpen(bitmap_data *bitmap, float sharpen_amount, const int16_t *positive_table, const int16_t *negative_table)
{
    fprintf(__iob_func() + 1, "### WARNING tried to sharpen a cube map");
    fflush(__iob_func() + 1);
}
