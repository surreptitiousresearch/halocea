#include <stdint.h>
extern int global_screenshot_count;
extern int16_t global_screenshot_size;

BOOL screenshot_in_progress(void)
{
    return global_screenshot_count > 1 || (global_screenshot_count == 1 && global_screenshot_size > 1);
}
