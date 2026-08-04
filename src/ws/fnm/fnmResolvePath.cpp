#include "../../headers/ws/fnm/fnm.h"
#include "../../headers/ws/fnm/fnm_boundary.h"

// fnmResolvePath @ 0x82626490 -- if `name` starts with "./" or ".\\", seed dst with the process
// startup directory first (so fnmResolvePathWithGivenBase's "append name+2 onto dst" branch has a
// base to append to); then delegate to fnmResolvePathWithGivenBase, which does the actual
// splice/copy and bounds assert.
const char *fnmResolvePath(char *dst, const char *name)
{
    bool isDotRelative = (name[0] == '.') && (name[1] == '\\' || name[1] == '/');

    if (isDotRelative)
        osGetStartupDir(dst, 260);

    return fnmResolvePathWithGivenBase(dst, name);
}
