/* console_color @ 0x84177D44 — .data data-init (real_argb_color). 16-byte value
 * {a=1,r=1,g=0.3,b=1}. Ledger size 20 overcounts by 4 trailing zero bytes (separate global). */
#include "../headers/real_argb_color.h"
real_argb_color console_color = { 1.0f, 1.0f, 0.3f, 1.0f };
