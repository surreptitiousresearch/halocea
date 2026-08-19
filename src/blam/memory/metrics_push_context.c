#include <stdint.h>
/* metrics_push_context @0x83818D40 — retail stub: always returns true, the `context_name` argument is
 * never read. Full body confirmed via disasm (`li r3, 1; blr`) — the real metrics-context-stack push
 * logic is compiled out in this build, leaving only the "success" return value. */

uint8_t metrics_push_context(const char *const context_name)
{
    return 1;
}
