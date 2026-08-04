/* .data init @0x84184CF4:
 * { &players_handle_deleted_object, &ai_handle_deleted_object, &objects_fix_for_deleted_object } */
extern void players_handle_deleted_object(int object_index);
extern void ai_handle_deleted_object(int deleted_object_index);
extern void objects_fix_for_deleted_object(int deleted_object_index);

void (*object_deleted_procs[3])(int) =
{
    players_handle_deleted_object,
    ai_handle_deleted_object,
    objects_fix_for_deleted_object,
};
