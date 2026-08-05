/* data_direction_strings @ 0x8211DEE4 — .rdata, 8 bytes = 2 x pointer. The net-graph
 * data-direction names, indexed by net_graph_data_direction. Initializer reconstructed from
 * the binary (big-endian words; string bytes read from the image):
 *   +0x0000 0x8211DE7C -> .rdata "sent"  (net_graph_data_direction_sent     = 0)
 *   +0x0004 0x8211DE74 -> .rdata "recv"  (net_graph_data_direction_received = 1)
 * The 2-element extent is exact: the next named .rdata address is the "%s %s" literal
 * @0x8211DEEC, and it agrees with NUMBER_OF_NET_GRAPH_DATA_DIRECTIONS (2) and with the bound
 * src/data_direction_from_string.c reconstructed for its scan loop.
 *
 * DEVIATION: the corpus declares this object two different ways — `const char *const` in
 * src/data_direction_to_string.c and src/update_for_resolution_change.c, but `const char *`
 * (mutable pointers) in src/data_direction_from_string.c and src/net_graph_change_display.c.
 * The object lives in .rdata (segment perm 4, read-only), which a mutable pointer array
 * cannot, so `const char *const` is the original spelling and is used here. The four
 * consumer declarations are left untouched.
 *
 * CAVEAT: the enum name for element 1 is net_graph_data_direction_received, but the string in
 * the image is the abbreviated "recv" — the string is reproduced verbatim from the image. */

const char *const data_direction_strings[2] =
{
    "sent", /* [0] net_graph_data_direction_sent */
    "recv", /* [1] net_graph_data_direction_received */
};
