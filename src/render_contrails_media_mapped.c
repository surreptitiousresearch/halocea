/* render_contrails_media_mapped @0x837EB2A8 — tail-call forwarder (`li r3, 4; b render_contrails`;
 * confirmed via disasm, not a bare blr). render_contrails itself is not yet decompiled — declared extern
 * with its own DB prototype. */

#include "headers/contrail_render_type.h"

extern void render_contrails(unsigned int render_type_flags);

void render_contrails_media_mapped(void)
{
    render_contrails(CONTRAIL_RENDER_TYPE_MEDIA_FLAGS);
}
