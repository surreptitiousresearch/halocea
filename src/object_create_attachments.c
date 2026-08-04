/* object_create_attachments @ 0x836F05A8 — Blam engine.
 * Creates the widgets and attachments belonging to an object. */

extern void widgets_new(int object_index);
extern void attachments_new(int object_index);

void object_create_attachments(int object_index)
{
    widgets_new(object_index);
    attachments_new(object_index);
}
