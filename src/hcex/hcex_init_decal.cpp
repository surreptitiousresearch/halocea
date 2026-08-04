/* hcex_init_decal @0x836822E0 — bridge a Blam decal definition + world point across to the ws-engine decal
 * renderer by name. Wrapped in a PIX event for GPU capture. */

#include <stdint.h>
#include "../headers/real_point3d.h"
#include "../headers/hcex/hcex_float3.h"

extern char *tag_get_name(int16_t tag_index);
/* hcex render-bridge primitives — boundary into the ws-engine decal/scorch system, not reversed here. */
extern void hcex_pix_begin_event(const char *name);
extern void hcex_pix_end_event(void);
extern void hcex_create_decal(const char *decal_name, const hcex_float3 *position);

extern "C" void hcex_init_decal(int decal_definition_index, const real_point3d *point)
{
    const char *decal_name = tag_get_name(decal_definition_index);

    hcex_float3 position;
    position.x = point->n[0];
    position.y = point->n[1];
    position.z = point->n[2];

    hcex_pix_begin_event("hcex_create_decal");
    hcex_create_decal(decal_name, &position);
    hcex_pix_end_event();
}
