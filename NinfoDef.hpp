#ifndef NINFO_DEF
#define NINFO_DEF

namespace ninf
{

    enum BlockType
    {
         N_BOND,
         N_ANGL,
         N_DIHD,
         N_AICG13,
         N_AICGDIH,
         N_CONTACT,
         NO_BLOCK,
         N_BASE
    };

    enum LineType
    {
        BLOCK_START,
        NINFO_LINE,
        BLOCK_END,
        COMMENT,
        EMPTY,
        WRONG
    };

    
}
#endif
