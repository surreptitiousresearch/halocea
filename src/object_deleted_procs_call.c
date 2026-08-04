/* object_deleted_procs_call @ 0x837E2CA8 — notify each registered object-deleted callback (3 slots)
 * that an object is being torn down, so subsystems holding references can drop them. */

extern void (*object_deleted_procs[3])(int deleted_object_index);

void object_deleted_procs_call(int deleted_object_index)
{
    for ( __int16 i = 0; i < 3; ++i )
        object_deleted_procs[i](deleted_object_index);
}
