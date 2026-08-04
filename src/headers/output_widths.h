#pragma once

/* output_widths @ metrics_globals.widths — column widths for the network-message
 * metrics text report. DB-verified layout (sizeof=52). The first eight fields are
 * const compile-time widths; the trailing five are runtime-computed name lengths. */
typedef struct output_widths
{
    const int message_count_width;         /* 0x00 */
    const int message_bytes_width;         /* 0x04 */
    const int message_average_bytes_width; /* 0x08 */
    const int iteration_count_width;       /* 0x0C */
    const int average_iterations_width;    /* 0x10 */
    const int average_bits_width;          /* 0x14 */
    const int type_count_width;            /* 0x18 */
    const int type_bits_width;             /* 0x1C */
    int longest_message_name;              /* 0x20 */
    int longest_mode_name;                 /* 0x24 */
    int longest_field_reference_name;      /* 0x28 */
    int longest_field_properties_name;     /* 0x2C */
    int longest_field_type_name;           /* 0x30 */
} output_widths; /* 52 bytes */
