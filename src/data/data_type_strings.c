/* data_type_strings @ 0x8211DEDC — .rdata, 8 bytes = 2 x pointer. The net-graph data-type
 * names, indexed by net_graph_data_type. Initializer reconstructed from the binary
 * (big-endian words; string bytes read from the image):
 *   +0x0000 0x8211DE8C -> .rdata "bytes"    (net_graph_data_type_bytes   = 0)
 *   +0x0004 0x8211DE84 -> .rdata "packets"  (net_graph_data_type_packets = 1)
 * The 2-element extent is exact: the next named .rdata symbol is data_direction_strings
 * @0x8211DEE4, and it agrees with NUMBER_OF_NET_GRAPH_DATA_TYPES (2) and with the bound
 * src/data_type_from_string.c reconstructed for its scan loop.
 *
 * DEVIATION: the corpus declares this object two different ways — `const char *const` in
 * src/data_type_to_string.c and src/update_for_resolution_change.c, but `const char *`
 * (mutable pointers) in src/data_type_from_string.c and src/net_graph_change_display.c.
 * The object lives in .rdata (segment perm 4, read-only), which a mutable pointer array
 * cannot, so `const char *const` is the original spelling and is used here. The four
 * consumer declarations are left untouched. */

const char *const data_type_strings[2] =
{
    "bytes",   /* [0] net_graph_data_type_bytes */
    "packets", /* [1] net_graph_data_type_packets */
};
