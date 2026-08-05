/* apDefragMng @ 0x842CC648 - .data zero-fill. DB symbol ?apDefragMng@@3PAVapDEFRAG_MNG@@A
 * (a pointer to the optional video-memory defragmenting allocator; null unless the defrag allocator is
 * installed). Declared here as an opaque pointer - apDEFRAG_MNG is a ws/ap class, an extern boundary. */

struct apDEFRAG_MNG;
struct apDEFRAG_MNG *apDefragMng;
