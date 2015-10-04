#ifndef NINFO_LINE_BASE
#define NINFO_LINE_BASE
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <stdexcept>
#include <memory>
#include "NinfoDef.hpp"

namespace ninf
{

    class LineBase
    {

    public:
        LineBase(){}

        virtual LineType readline(std::ifstream& file) = 0;
        virtual int get_index() const = 0;
        virtual int get_iunit1() const = 0;
        virtual int get_iunit2() const = 0;
        virtual std::vector<int> get_imps() const = 0;
        virtual std::vector<int> get_impuns() const = 0;
        virtual std::vector<double> get_coefs() const = 0;
        virtual BlockType get_BlockType() const = 0;
        virtual void read_stream(std::istream& is) = 0;
        virtual void write_stream(std::ostream& os) = 0;
    };

    typedef std::shared_ptr<LineBase> LineSptr;
    typedef std::weak_ptr<LineBase> LineWptr;


    class BlockBase
    {
    protected:
        BlockType type;

    public:
        std::vector<LineSptr> lines;

    public:
        BlockBase(BlockType T): type(T){}
        void push_back(LineSptr line);
        BlockType get_BlockType() const { return type; }
        int size() const { return lines.size(); }
        std::vector<LineSptr>::iterator begin(){ return lines.begin(); }
        std::vector<LineSptr>::iterator end(){ return lines.end(); }

        virtual void write_block(std::ofstream& os) = 0;
        virtual void read_block(std::ifstream& is) = 0;

    };

    void BlockBase::push_back(LineSptr line)
    {
        if( line->get_BlockType() != type )
            throw std::invalid_argument("Block must not contain other type line");

        lines.push_back(line);
        return;
    }


    typedef std::shared_ptr<BlockBase> BlockSptr;
    typedef std::weak_ptr<BlockBase> BlockWptr;
    typedef std::vector<BlockSptr> NinfoData;

}
#endif
