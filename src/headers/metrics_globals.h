#pragma once
/* metrics_globals @ metrics_globals — network-message metrics accumulator.
 * Reconciled to authoritative layout (sizeof=0x150). output_widths (52B),
 * message_metrics (4B) and traffic_log (4B) are separate DB types; each is its
 * own ledger entry and belongs in its own header (not yet produced). */

#include "output_widths.h"
#include "message_metrics.h"
#include "traffic_log.h"

struct _iobuf;

struct metrics_globals
{
    struct output_widths   widths;                          /* 0x00 */
    int                    message_definition_packed_bit_size; /* 0x34 */
    unsigned char          write_message_log;               /* 0x38 */
    unsigned char          auto_write_metrics_files;         /* 0x39 */
    unsigned char          _pad3A[2];                        /* 0x3A */
    struct message_metrics metrics;                          /* 0x3C */
    struct _iobuf         *traffic_log_file;                 /* 0x40 */
    struct traffic_log     traffic_data;                     /* 0x44 */
    unsigned int           first_message_time;               /* 0x48 */
    char                   metrics_timestamp[260];           /* 0x4C */
}; /* 336 bytes */

#ifdef __cplusplus
extern "C" {
#endif

extern struct metrics_globals metrics_globals;

#ifdef __cplusplus
}
#endif
