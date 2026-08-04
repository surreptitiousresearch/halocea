#include "headers/detail_object_view_data.h"

extern void _rasterizer_detail_objects_rebuild_vertices(const detail_object_view_data *detail_object_view_data);

void rasterizer_detail_objects_rebuild_vertices(const detail_object_view_data *detail_object_view_data)
{
    _rasterizer_detail_objects_rebuild_vertices(detail_object_view_data);
}
