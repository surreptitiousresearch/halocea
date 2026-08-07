/* bitmap_cm_height_map @0x8377C4A0 — cube maps can't be used as height maps; warn and bail. `bitmap`/
 * `bump_height` are unread (matches the DB's 2-argument signature; the warning doesn't need them). */

#include "headers/bitmap_data.h"

#include <stdio.h>
typedef struct _iobuf _iobuf;

extern _iobuf *__iob_func(void);
/* fprintf provided by CRT via <stdio.h>; local extern removed (C28251: the local redeclaration drops the header's annotations) */
/* fflush provided by CRT via <stdio.h>; local extern removed (C28251: the local redeclaration drops the header's annotations) */

void bitmap_cm_height_map(bitmap_data *bitmap, float bump_height)
{
    fprintf(__iob_func() + 1, "### WARNING tried to use a cube map as a height map\r\n");
    fflush(__iob_func() + 1);
}
