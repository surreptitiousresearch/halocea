/* editor_preprocess_rendered_object 0x83737488 — editor hook that can veto/preprocess an object before it is
 * rendered. Compiled out in the release build: always reports "render it" (1). */

#include <stdint.h>
#include "headers/render_lighting.h"

uint8_t editor_preprocess_rendered_object(int object_index, render_lighting *lighting)
{
    return 1;
}
