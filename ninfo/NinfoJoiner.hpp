#ifndef NINFO_JOINER
#define NINFO_JOINER
#include "NinfoReader.hpp"
#include "NinfoWriter.hpp"
#include <algorithm>

namespace liberica 
{
    class NinfoJoiner
    {
        struct blockcomp
        {
            bool operator() (const BlockSptr& lhs, const BlockSptr& rhs)
            {
                int l = lhs->get_BlockType();
                int r = rhs->get_BlockType();
                return (l<r);
            }
        } compare_block;

        NinfoData datas;

    public:

        NinfoJoiner(){}
        ~NinfoJoiner(){}

        void push_back(const BlockSptr& block);

        void write_file(const std::string& filename);

    private:

        //lhs <- rhs
        void join(BlockSptr& lhs, const BlockSptr& rhs);

        NinfoData::iterator find(const BlockType b_type);

        void sort();

    };

    void NinfoJoiner::push_back(const BlockSptr& block)
    {
        for(auto iter = datas.begin(); iter != datas.end(); ++iter)
        {
            if(find((*iter)->get_BlockType()) != datas.end())
            {
                join((*iter), block);
            }
            else
            {
                datas.push_back(block);
            }
        }
    }

    void NinfoJoiner::write_file(const std::string& filename)
    {
        sort();
        NinfoWriter writer(filename, datas);
        writer.write();
        return;
    }

    //************************ private ************************//
    void NinfoJoiner::join(BlockSptr& lhs, const BlockSptr& rhs)
    {
        for(std::vector<LineSptr>::iterator iter = rhs->begin();
                iter != rhs->end(); ++iter)
            lhs->push_back(*iter);
    }

    NinfoData::iterator NinfoJoiner::find(const BlockType b_type) 
    {
        auto iter = datas.begin();
        for(; iter != datas.end(); ++iter)
        {
            if((*iter)->get_BlockType() == b_type)
                return iter;
        }
        return iter;
    }

    void NinfoJoiner::sort()
    {
        std::sort(datas.begin(), datas.end(), compare_block);
        return;
    }

}
#endif
