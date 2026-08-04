#include <stdint.h>
/* system_ensure_directory_tree_exists @0x83762E28 — on this console build the directory tree is assumed to
 * already exist (the title's storage layout is fixed), so this is a stub that always reports success. The
 * caller's return convention is unsigned __int8; the body just returns 0. */

uint8_t system_ensure_directory_tree_exists(const char *directory)
{
    return 0;
}
