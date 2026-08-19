/* hs_object_to_object_list @0x8368D688 — HaloScript coercion: wrap a single object index into a freshly
 * allocated one-element object list; returns -1 for the none index (leaving no list allocated). */

extern int object_list_new(void);
extern void object_list_add(int object_list_index, int object_index);

int hs_object_to_object_list(int object_index)
{
    if (object_index == -1)
        return -1;

    int object_list = object_list_new();
    object_list_add(object_list, object_index);
    return object_list;
}
