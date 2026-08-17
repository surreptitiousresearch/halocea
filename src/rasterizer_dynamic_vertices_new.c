/* rasterizer_dynamic_vertices_new @0x836A5DF0 */
#include <stdint.h>
extern int _rasterizer_dynamic_vertices_new(int16_t type, int count);

int rasterizer_dynamic_vertices_new(int16_t type, int count)
{
    return _rasterizer_dynamic_vertices_new(type, count);
}
