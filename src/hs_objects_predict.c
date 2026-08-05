/* hs_objects_predict @0x837F8480 — request resource prediction (preload) for every object in a HaloScript
 * object list. */

extern int object_list_get_first(int object_list_index, int *reference_index);
extern int object_list_get_next(int object_list_index, int *reference_index);
extern void object_predict(int object_index);

void hs_objects_predict(int object_list_index)
{
    int iterator[4];
    for ( int object_index = object_list_get_first(object_list_index, iterator);
          object_index != -1;
          object_index = object_list_get_next(object_list_index, iterator) )
    {
        object_predict(object_index);
    }
}
