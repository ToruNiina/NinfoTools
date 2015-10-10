#ifndef LIBERICA_INP_BASE
#define LIBERICA_INP_BASE
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

namespace liberica
{
    class InpBase
    {
    protected:

        bool already_read;

    public:

        InpBase(): already_read(false){}
        ~InpBase(){}
    };

}
#endif //INP_BASE
