/* main_save_cancel @0x83689188 — cancel a pending map save. */

#include "headers/main_globals.h"

void main_save_cancel(void)
{
    main_globals.save_map = 0;
}
