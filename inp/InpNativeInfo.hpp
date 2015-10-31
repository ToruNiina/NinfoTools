#ifndef INP_NINFO
#define INP_NINFO
#include "InpBase.hpp"
#include <cstdlib>
#include <map>

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
//  M^2 = fileM_M.ninfo
//  >>>> 

    typedef std::pair<int, int> iUnits;

    class InpNativeInfo : public InpBase
    {
    public:
        
        enum STYLE
        {
            ALL_IN_ONE,
            ONE_BY_ONE
        };

    private:

        int simN;
        STYLE style;
        std::map<std::pair<int, int>, std::string> ninfo_files;
       //if (all/all) -> pair(0,0)

    public:

        InpNativeInfo(int N = 1): InpBase(), simN(N) {}
        ~InpNativeInfo(){}

        bool read_file(std::string file_name);

        std::map<std::pair<int, int>, std::string>& get_files()
        {
            return ninfo_files;
        }

    private:

        void read_block(std::ifstream& ifs);

    };

    bool InpNativeInfo::read_file(std::string file_name)
    {
        if(already_read)
            std::cout << "Warning: trying to substitute ninfo for already initiated object." 
                      << std::endl;
        already_read = true;
        std::ifstream inpfile(file_name.c_str());
        bool read(false);
        
        while(!inpfile.eof())
        {
            std::string line;
            std::getline(inpfile, line);

            if(line.empty()) continue;
            if(line[0] == '\x2a') continue;

            if(eq_ignorecase(line.substr(0,20), "<<<< native_info_sim"))
            {
                read = true;
                std::string temp(line, 20);
                simN = std::stoi(temp);
                read_block(inpfile);
            }
        }
        return read;
    }

    void InpNativeInfo::read_block(std::ifstream& ifs)
    {
//         bool read(false);
        while(!ifs.eof())
        {
            std::string line;
            std::getline(ifs, line);
            if(line.empty()) continue;
            if(line[0] == '\x2a') continue;

            if(eq_ignorecase(line.substr(0,14), "NINFO(all/all)"))
            {
                style = ALL_IN_ONE;
                iUnits iunits = std::make_pair(0,0);
                std::vector<std::string> subline = stringsplit(line, ' ');
                for(auto iter = subline.begin(); iter != subline.end(); ++iter)
                {
                    if(iter->substr(iter->size()-6, 6) == "\x2eninfo")
                    {
//                         read = true;
                        ninfo_files[iunits] = *iter;
                        return;
                    }
                }
                throw std::invalid_argument("read_block: cant find ninfo file");
            }
            else if(line.substr(0,6) == "NINFO(")
            {
                if(style != ONE_BY_ONE)
                    style = ONE_BY_ONE;
                std::vector<std::string> iunit1_str
                    = stringsplit(line, '/', 6);
                std::vector<std::string> iunit2_str
                    = stringsplit(iunit1_str.at(1), ')');

                int iunit1(std::stoi(iunit1_str.at(0)));
                int iunit2(std::stoi(iunit2_str.at(0)));

                std::vector<std::string> filename
                    = stringsplit(line, ' ');
                iUnits iunits = std::make_pair(iunit1, iunit2);
                for(auto itr = filename.begin(); itr != filename.end(); ++itr)
                {
                    if(itr->substr(itr->size()-6, 6) == "\x2eninfo")
                    {
//                         read = true;
                        ninfo_files[iunits] = *itr;
                        break;
                    }
                }
            }
            else if(line.substr(0,4) == ">>>>")
            {
//                 read = true;
                break;
            }
            else
            {
                std::cout << "Error: Unknown line" << std::endl;
                std::cout << line << std::endl;
            }
        }
        return;
    }

}
#endif
