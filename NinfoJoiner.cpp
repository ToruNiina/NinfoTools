#include "inp/InpFileName.hpp"
#include "inp/InpNativeInfo.hpp"
#include "ninfo/NinfoJoiner.hpp"

using namespace liberica;

int main(int argc, char *argv[])
{
    if(argc != 2 && argc != 3)
    {
        std::cout << "Usage: ./njoiner <filename>.inp [output_filename]" << std::endl;
        std::cout << "default output filename is out.ninfo" << std::endl;
        return -1;
    }

    std::string inpfilename(argv[1]);
    InpFileName inputfilenames;

    inputfilenames.read_file(inpfilename);
    const std::string ninf_path = inputfilenames.get_ninf_path();
    const std::string out_path = inputfilenames.get_out_path();

    InpNativeInfo inputnativeinfo;

    inputnativeinfo.read_file(inpfilename);
    std::map<iUnits, std::string> ninfo_files
        = inputnativeinfo.get_files();

    std::pair<int, int> all_in_one = std::make_pair(0,0);
    if(ninfo_files.find(all_in_one) != ninfo_files.end())
    {
        std::cout << "this ninfo file is all-in-one style." << std::endl;
        return -1;
    }

    NinfoJoiner joiner;
    for(auto file_itr = ninfo_files.begin();
        file_itr != ninfo_files.end(); ++file_itr)
    {
        iUnits iunits = (*file_itr).first;
        std::string filename = ninf_path + (*file_itr).second;
        NinfoReader reader(filename);
        std::vector<BlockSptr> blocks_in_file = reader.read_file();

        for(auto block_itr = blocks_in_file.begin();
            block_itr != blocks_in_file.end(); ++block_itr)
        {
            (*block_itr)->set_iunits(iunits);
            joiner.push_back(*block_itr);
        }
    }

    std::string outfile;
    if(argc == 3)
    {
        outfile = argv[2];
    }
    else
    {
        outfile = "out.ninfo";
    }
    joiner.write_file(outfile);
    return 0;
}

