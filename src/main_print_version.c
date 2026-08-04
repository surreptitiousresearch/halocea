/* main_print_version @0x8368AE68 — print the build version banner to the console. */

#include <stdint.h>

extern void console_printf(uint8_t clear, const char *format, ...);

void main_print_version(void)
{
    console_printf(0, "halo xbox 01.00.01.0563 Jun 24 2011 13:31:56");
}
