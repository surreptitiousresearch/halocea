/* render_contrails_normal @0x837EB2B0 — tail-call: render contrails with the "normal" render-type flag
 * set (-13 = 0xFFFFFFF3). */

#include "headers/contrail_render_type.h"

extern void render_contrails(unsigned int render_type_flags);

void render_contrails_normal(void)
{
    render_contrails((unsigned int)CONTRAIL_RENDER_TYPE_NORMAL_FLAGS);
}
