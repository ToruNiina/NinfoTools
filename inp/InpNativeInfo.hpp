#ifndef INP_NINFO
#define INP_NINFO
#include "InpBase.hpp"

namespace liberica
{
//  <<<< Native_info_simN
//  NINFO(all/all) filename.ninfo
//
//  or
//
//  NINFO(1/1) 1
//  NINFO(1/2) 2
//  ...
//  NINFO(M/M)
//  1 = file1.ninfo
//  ...
//  M^2 = fileM^2.ninfo
//  >>>> 

    class InpNativeInfo : public InpBase
    {
    public:
        
        enum STYLE
        {
            ALL_IN_ONE,
            ONE_BY_ONE
        }

    private:

        int simN;
        STYLE style;
        std::map<std::pair<int, int>, std::string> ninfo_files;
       //if (all/all) -> pair(0,0)

    public:

        InpNativeInfo(int N = 1): InpBase(), simN(N) {}
        ~InpNativeInfo(){}

        bool read_file(std::string file_name);
        std::map<std::pair<int, int>, std::string> const get_files(){return ninfo_files;}

    private:

        void read_block(std::ifstream& ifs);
    };

    bool InpNativeInfo::read_file(std::string file_name)
    {
        if(already_read)
            std::cout << "Warning: trying to substitute NativeInfo for already initiated object." << std::endl;
        already_read = true;
        std::ifstream inpfile( file_name.c_str() );
        bool read(false);
        
        while(!inpfile.eof() )
        {
            std::string line;
            std::getline(inpfile, line);

            if(line.empty()) continue;
            if(line[0] == '\x2a') continue;

            if(line.substr(0,20) == "<<<< Native_info_sim")
            {
                read = true;
                simN = atoi( line[21] );
                read_block(inpfile);
            }
        }
        return read;
    }

    void InpNativeInfo::read_block(std::ifstream& ifs)
    {
        return;
    }
}
#endif
