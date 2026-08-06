/* apDefragMng @ 0x842CC648 (.data, 4 bytes)
 * DB applied_types: apDEFRAG_MNG *apDefragMng;
 * Image bytes (big-endian), decoded from the binary .data record:
 *   +0x0000 = 0x00000000
 * .data zero-fill. DB symbol ?apDefragMng@@3PAVapDEFRAG_MNG@@A
 * (a pointer to the optional video-memory defragmenting allocator; null unless the defrag allocator is
 * installed). Declared here as an opaque pointer - apDEFRAG_MNG is a ws/ap class, an extern boundary.
 */
struct apDEFRAG_MNG;
struct apDEFRAG_MNG *apDefragMng;
