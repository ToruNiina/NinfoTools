#ifndef NINFO_SPLITTER
#define NINFO_SPLITTER
#include <map>
#include <utility>
#include "NinfoReader.hpp"
#include "NinfoWriter.hpp"

namespace ninf
{
    typedef std::pair<int, int> iUnits;

    bool operator==(const iUnits& lhs, const iUnits& rhs)
    {
        bool forward(lhs.first == rhs.first && lhs.second == rhs.second);
        bool backward(lhs.first == rhs.second && lhs.second == rhs.first);
        return forward || backward;                                                               
    }

    bool operator<(const iUnits& lhs, const iUnits& rhs)
    {
        if(lhs.first != rhs.first)
        {
            return (lhs.first < rhs.first);
        }else{
            return (lhs.second < rhs.second);
        }
    }

    class NinfoSplitter
    {
        int simN;
        std::string filename;
        NinfoData all_in_one;

    public:
        NinfoSplitter(std::string _filename, NinfoData data)
            : simN(1), all_in_one(data)
        {
            if(_filename.substr(_filename.size()-6,6) == "\x2eninfo")
            {
                _filename.erase( _filename.end() - 6, _filename.end() );
            }
            filename = _filename;
        }

        NinfoSplitter(std::string _filename, NinfoData data, int N)
            : simN(N), all_in_one(data)
        {
            if(_filename.substr(_filename.size()-6,6) == "\x2eninfo")
            {
                _filename.erase( _filename.end() - 6, _filename.end() );
            }
            filename = _filename;
        }

        std::vector<WriterSptr> split();

    private:
        std::string gen_filename(iUnits iu);
        std::vector<std::pair<iUnits, BlockSptr> > get_subBlock(BlockSptr block);
        BlockSptr gen_sub_block(BlockSptr block);
        void write_inp(std::map<iUnits, WriterSptr> writers);
    };

    std::vector<WriterSptr> NinfoSplitter::split()
    {
        std::map<iUnits, WriterSptr> writer_map;

        //foreach block
        for(NinfoData::iterator iter = all_in_one.begin(); iter != all_in_one.end(); ++iter)
        {
            std::vector<std::pair<iUnits, BlockSptr> > sub_blocks( get_subBlock(*iter) );

            //for each pairof(iunits, blocksptr)
            for(std::vector<std::pair<iUnits, BlockSptr> >::iterator sbiter = sub_blocks.begin();
                sbiter != sub_blocks.end(); ++sbiter)
            {//search writer & add the block into the writer
                std::map<iUnits, WriterSptr>::iterator wmiter = writer_map.find(sbiter->first);

                if(wmiter != writer_map.end())
                {//writer exist
                    wmiter->second->push_back(sbiter->second);
                }
                else
                {//writer does not exist
                    WriterSptr new_writer(new NinfoWriter(gen_filename(sbiter->first) ) );
                    new_writer->push_back(sbiter->second);
                    writer_map[sbiter->first] = new_writer;
                }
            }
        }

        write_inp(writer_map);

        std::vector<WriterSptr> splitted( writer_map.size() );
        std::vector<WriterSptr>::iterator splitr(splitted.begin());

        for( std::map<iUnits, WriterSptr>::iterator iter = writer_map.begin();
             iter != writer_map.end(); ++iter )
        {
            *splitr = iter->second;
            ++splitr;
        }
        return splitted;
    }

    std::string NinfoSplitter::gen_filename(iUnits iu)
    {
        return filename + '_' + std::to_string(iu.first) +'_'+ std::to_string(iu.second) + "\x2eninfo";
    }
//TODO
    std::vector<std::pair<iUnits, BlockSptr> > NinfoSplitter::get_subBlock( BlockSptr block )
    {
        std::vector<std::pair<iUnits, BlockSptr> > subBlocks;
        iUnits iunit_now, iunit_prev(0,0), zero(0,0);

        BlockSptr sub_block( gen_sub_block(block) );

        for(std::vector<LineSptr>::iterator iter = block->begin(); iter != block->end(); ++iter)
        {
            iunit_now = std::make_pair( (*iter)->get_iunit1(), (*iter)->get_iunit2() );
            if(iunit_now == iunit_prev)
            {
                sub_block->push_back( *iter );
                if(iter == block->end() - 1)
                    subBlocks.push_back(std::make_pair(iunit_now, sub_block));
            }
            else
            {
                if( !(iunit_prev == zero) )
                    subBlocks.push_back(std::make_pair(iunit_prev, sub_block) );
                sub_block = gen_sub_block(block);
                sub_block->push_back( *iter );
            }
            iunit_prev = iunit_now;
        }
        return subBlocks;
    }

    BlockSptr NinfoSplitter::gen_sub_block(BlockSptr block)
    {
        BlockType type( block->get_BlockType() );
        switch(type)
        {
            case N_BOND:
                return BlockSptr(new BondBlock);
            case N_ANGL:
                return BlockSptr(new AnglBlock);
            case N_DIHD:
                return BlockSptr(new DihdBlock);
            case N_AICG13:
                return BlockSptr(new Aicg13Block);
            case N_AICGDIH:
                return BlockSptr(new AicgdihBlock);
            case N_CONTACT:
                return BlockSptr(new ContactBlock);
            default:
                throw std::invalid_argument("Unknown BlockType");
        }
    }


    void NinfoSplitter::write_inp(std::map<iUnits, WriterSptr> writers)
    {
        int counter(0);
        std::ofstream ofs("native_info_simN.inp");
        ofs << "<<<< native_info_sim" << simN << std::endl;

        for(std::map<iUnits, WriterSptr>::iterator iter = writers.begin();
                iter != writers.end(); ++iter)
        {
            ++counter;
            ofs << "NINFO(" << iter->first.first << '/' << iter->first.second << ") "
                << counter << std::endl;
        }

        counter = 0;
        for(std::map<iUnits, WriterSptr>::iterator iter = writers.begin();
                iter != writers.end(); ++iter)
        {
            ++counter;
            ofs << counter << " = " << iter->second->get_filename() << std::endl;
        }

        ofs << ">>>>" << std::endl;
        return;
    }

}
#endif
