/* field_properties_player_index_definition @ 0x84183AC8 — data-init _field_properties_definition
 * (104 bytes) describing the "player_index" translated-index network field type.
 * Initializer reconstructed from the binary (big-endian):
 *   type            = 0x0000000D  -> _field_type_translated_index
 *   name            = "player_index"
 *   encode          = 0x8379BA30  -> default_translated_index_encoder
 *   decode          = 0x8379BA80  -> default_translated_index_decoder
 *   parameters      = 0x84183A98  -> &field_properties_player_index_parameters
 *   maximum_size    = 0xFFFFFFFF  -> -1
 *   overhead_bits   = 0
 *   initialized     = 0
 */
#include "../headers/field_properties_definition.h"
#include "../headers/_field_type_translated_index_parameters.h"

/* Both externs carry the funcptr-SLOT signature from field_properties_definition +0x50/+0x54,
 * NOT the concrete attested one — they exist only to take an address for this dispatch table,
 * and the concrete spelling mismatches the slot (C4113). */
extern int default_translated_index_encoder(const struct _field_properties_definition *, void *, void *, void *); /* 0x8379BA30 */
extern int default_translated_index_decoder(const struct _field_properties_definition *, void *, void *, void *); /* 0x8379BA80 */
extern _field_type_translated_index_parameters field_properties_player_index_parameters;                          /* 0x84183A98 */

_field_properties_definition field_properties_player_index_definition =
{
    _field_type_translated_index,                  /* type */
    "player_index",                                /* name[75] */
    { 0 },                                          /* _pad4F */
    default_translated_index_encoder,              /* encode */
    default_translated_index_decoder,              /* decode */
    &field_properties_player_index_parameters,     /* parameters (void *const) */
    -1,                                             /* maximum_size */
    0,                                              /* overhead_bits */
    0                                               /* initialized */
};
