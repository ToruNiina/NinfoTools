#include "NinfoReader.hpp"
#include "NinfoWriter.hpp"

using namespace liberica;

int main(int argc, char *argv[])
{
    if(argc != 2)
    {
        std::cout << "NinfoTester:" << std::endl;
        std::cout << "Usage      : ./tester <filename>.ninfo" << std::endl;
        std::cout << "           : this is a test program to confirm whether the program" << std::endl;
        std::cout << "           : can read your ninfo file and write a ninfo file correctly or not." << std::endl;
        std::cout << "           : this reads an input ninfo file and outputs same ninfo file" << std::endl;
        std::cout << "           : by the different name. (possibly there are some points of " << std::endl;
        std::cout << "           : difference between input and output files. ex. some comments)" << std::endl;
    }

    std::string file_name(argv[1]);

    NinfoReader reader(file_name);

    NinfoData data(reader.read_file());

    std::string out_file_name("out_" + file_name);

    NinfoWriter writer(out_file_name, data);

    writer.write();

    return 0;
}
