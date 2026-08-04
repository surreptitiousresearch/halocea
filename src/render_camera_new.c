/* render_camera_new @0x8376B728 — tail-call forwarder (`li r5, 0x54; li r4, 0; b memset`; confirmed via
 * disasm, not a bare blr). Zero-initializes a render_camera (0x54 = sizeof(render_camera)). */

#include "headers/render_camera.h"

extern void *memset(void *dst, int value, unsigned int size);

void render_camera_new(render_camera *camera)
{
    memset(camera, 0, sizeof(render_camera));
}
