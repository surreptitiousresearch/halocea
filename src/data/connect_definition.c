/* connect_definition @ 0x8211C664 — hs_function_table entry for the HaloScript "connect" function.
 * Initializer reconstructed from the binary .rdata record (32 bytes, big-endian):
 *   +0x00 return_type            = 0x0004 -> hs_type_void
 *   +0x04 name                   = 0x8210157C -> "connect"
 *   +0x08 parse                  = 0x837787E0 -> hs_macro_function_parse
 *   +0x0C evaluate               = 0x8372E400 -> network_game_client_attempt_connection_ascii_password_evaluate
 *   +0x10 documentation          = 0x82116018
 *   +0x14 parameters             = 0x00000000 (NULL)
 *   +0x18 console_flags          = 0x5F
 *   +0x1A formal_parameter_count = 2  (+0x1C formal_parameters = hs_type_string, hs_type_string)
 */
#include "../headers/hs_function_definition.h"
#include "../headers/hs_type.h"
#include "../headers/hs_console_flags.h"

/* The parse/evaluate externs carry the funcptr-SLOT signature from
 * hs_function_definition +0x08/+0x0C, not the concrete attested one — they exist only
 * to take an address for this dispatch table. */
extern unsigned char hs_macro_function_parse(int16_t function_index, int expression_index);
extern void network_game_client_attempt_connection_ascii_password_evaluate(int16_t function_index, int thread_index, unsigned char initialize);

const hs_function_definition connect_definition =
{
    hs_type_void,                /* return_type */
    { 0, 0 },                    /* _pad02 */
    "connect",                   /* name */
    hs_macro_function_parse,     /* parse */
    network_game_client_attempt_connection_ascii_password_evaluate, /* evaluate */
    "Attempt to connect to server - use ip:port password as parameters", /* documentation */
    0,                           /* parameters */
    (1u << _hs_console_allowed) | (1u << _hs_console_listen_server) | (1u << _hs_console_dedicated_server) | (1u << _hs_console_ingame) | (1u << _hs_console_idle) | (1u << _hs_console_ship_game), /* console_flags = 0x5F */
    { 0 },                       /* _pad19 */
    2,                           /* formal_parameter_count */
    { hs_type_string, hs_type_string },  /* formal_parameters */
};
