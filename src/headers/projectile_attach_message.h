#pragma once
/* projectile_attach_network_data — the stateless message-delta payload decoded by
 * projectile_attach_from_network: the projectile and parent object translated indices and the parent
 * node to attach to. DB-verified via types_members projectile_attach_network_data (12 bytes). */

typedef struct projectile_attach_network_data
{
    int     projectile_index;   /* 0x00 — translated object index */
    int     parent_index;       /* 0x04 — translated object index */
    __int16 parent_node_index;  /* 0x08 — parent node to attach to */
} projectile_attach_message; /* legacy local name for projectile_attach_network_data */
