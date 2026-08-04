/* tag_load @0x836F7078 — load a tag of the given group by name. In this build the loose-tag loader is
 * stubbed out (all tags come from the packed cache file), so it always reports failure (-1). */

int tag_load(unsigned int group_tag, const char *name, unsigned int flags)
{
    return -1;
}
