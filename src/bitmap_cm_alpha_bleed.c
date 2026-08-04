/* bitmap_cm_alpha_bleed @0x8377BFF8 — cube maps don't support alpha-bleed; warn and bail. `bitmap`/`passes`
 * are unread (matches the DB's 2-argument signature; the warning doesn't need them). */

#include <stdint.h>
#include "headers/bitmap_data.h"

#include <stdio.h>
typedef struct _iobuf _iobuf;

extern _iobuf *__iob_func(void);
extern int fprintf(_iobuf *stream, const char *format, ...);
extern int fflush(_iobuf *stream);

void bitmap_cm_alpha_bleed(bitmap_data *bitmap, int16_t passes)
{
    fprintf(__iob_func() + 1, "### WARNING tried to alpha-bleed a cube map (skipping)");
    fflush(__iob_func() + 1);
}
