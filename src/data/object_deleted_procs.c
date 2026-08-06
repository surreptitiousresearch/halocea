/* object_deleted_procs @ 0x84184CF4 (.data, 12 bytes)
 * DB applied_types: void (__fastcall *object_deleted_procs[3])(int);
 * Per-subsystem "an object was deleted" callback table, walked in order.
 * Image bytes (big-endian), decoded from the binary .data record:
 *   +0x0000 = 0x836EC718 -> objects_fix_for_deleted_object
 *   +0x0004 = 0x836E9258 -> ai_handle_deleted_object
 *   +0x0008 = 0x836AA590 -> players_handle_deleted_object
 * DEVIATION: the previous reconstruction had slots 0 and 2 transposed — it ran
 * players_handle_deleted_object first and objects_fix_for_deleted_object last, where the image
 * does the reverse. The table is walked in order, so the object-graph fixup has to happen before
 * the player and AI subsystems observe the deletion.
 * Caught 2026-08-06 by data_provenance.py --verify against the .data record. */
extern void players_handle_deleted_object(int object_index);
extern void ai_handle_deleted_object(int deleted_object_index);
extern void objects_fix_for_deleted_object(int deleted_object_index);

void (*object_deleted_procs[3])(int) =
{
    objects_fix_for_deleted_object,
    ai_handle_deleted_object,
    players_handle_deleted_object,
};
