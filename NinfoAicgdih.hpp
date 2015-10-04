#ifndef NINFO_AICG_DIH
#define NINFO_AICG_DIH
#include "NinfoBase.hpp"

namespace ninf
{

    class AicgdihLine : public LineBase
    {
        int idih, iunit1, iunit2;
        int imp1, imp2, imp3, imp4;
        int imp1un, imp2un, imp3un, imp4un;
        double dih_nat, factor_aicg14, correct_dih_mgo, coef_dih_gauss, wid_dih_gauss;
        std::string dihtype;
    public:
        AicgdihLine(): LineBase(),
            idih(0), iunit1(0), iunit2(0), imp1(0), imp2(0), imp3(0), imp4(0),
            imp1un(0), imp2un(0), imp3un(0), imp4un(0), dih_nat(0e0), factor_aicg14(0e0),
            correct_dih_mgo(0e0), coef_dih_gauss(0e0), wid_dih_gauss(0e0), dihtype(){}
        LineType readline(std::ifstream& file);
        void read_stream(std::istream& is);
        void write_stream(std::ostream& os);
        int get_index() const {return idih;}
        int get_iunit1() const {return iunit1;}
        int get_iunit2() const {return iunit2;}
        std::vector<int> get_imps() const 
        {
            std::vector<int> retval{imp1, imp2, imp3, imp4};
            return retval;
        }
        std::vector<int> get_impuns() const 
        {
            std::vector<int> retval{imp1un, imp2un, imp3un, imp4un};
            return retval;
        }
        std::vector<double> get_coefs() const 
        {
            std::vector<double> retval{dih_nat, factor_aicg14, correct_dih_mgo,
                                       coef_dih_gauss, wid_dih_gauss};
            return retval;
        }
        BlockType get_BlockType() const {return N_AICGDIH;};

    };

    LineType AicgdihLine::readline(std::ifstream& file)
    {
        std::string line;
        std::getline(file, line);
        if(line.empty()) return EMPTY;
        if(line[0] == '\x2a') return COMMENT;

        std::string head;
        std::istringstream ls(line);

        ls >> head;
        if(head != "aicgdih")
        {
            if(head == ">>>>") return BLOCK_END;
            std::cout << "this line is not aicgdih line." << std::endl;
            std::cout << line << std::endl;
            return WRONG;
        }

        read_stream(ls);
        return NINFO_LINE;
    }

    void AicgdihLine::write_stream(std::ostream& os)
    {
        os << "aicgdih";
        os << std::setw(7) << this->idih << std::setw(7);
        os << this->iunit1 << std::setw(7) << this->iunit2;
        os << std::setw(7) << this->imp1 << std::setw(7) << this->imp2;
        os << std::setw(7) << this->imp3 << std::setw(7) << this->imp4;
        os << std::setw(7) << this->imp1un << std::setw(7) << this->imp2un;
        os << std::setw(7) << this->imp3un << std::setw(7) << this->imp4un;
        os << std::setw(13) << std::fixed << std::setprecision(4) << this->dih_nat;
        os << std::setw(13) << std::fixed << std::setprecision(4) << this->factor_aicg14;
        os << std::setw(13) << std::fixed << std::setprecision(4) << this->correct_dih_mgo;
        os << std::setw(13) << std::fixed << std::setprecision(4) << this->coef_dih_gauss;
        os << std::setw(13) << std::fixed << std::setprecision(4) << this->wid_dih_gauss;
        os << " " << this->dihtype;
        os << std::endl;
        return;
    }

    void AicgdihLine::read_stream(std::istream& is)
    {
        is >> this->idih >> this->iunit1 >> this->iunit2;
        is >> this->imp1 >> this->imp2 >> this->imp3 >> this->imp4;
        is >> this->imp1un >> this->imp2un >> this->imp3un >> this->imp4un;
        is >> this->dih_nat >> this->factor_aicg14 >> this->correct_dih_mgo;
        is >> this->coef_dih_gauss >> this->wid_dih_gauss;
        is >> this->dihtype;
        return;
    }


    class AicgdihBlock : public BlockBase
    {
    public:
        AicgdihBlock(): BlockBase(N_AICGDIH) {}
        void write_block(std::ofstream& os);
        void read_block(std::ifstream& is);
    };

    void AicgdihBlock::write_block(std::ofstream& os)
    {
        if(lines.empty())
        {
            std::cout << "Warning: trying to write ninfo block having no line" << std::endl;
            return;
        }

        os << std::endl;
        os << "<<<< 1-4 contacts with L_AICG2_PLUS" << std::endl;
        os << "** coef_dihgauss(kcal/mol) = ";
        os << "factor_aicg14 * correct_dih_mgo * coef_aicg14(kcal/mol) * energy_unit_protein" << std::endl;
        os << "**      idih iunit1-iunit2   imp1 - imp2 - imp3 - imp4";
        os << " imp1un-imp2un-imp3un-imp4un   ";
        os << "dih_nat factor_aicg14  correct_mgo  coef_dih_gauss  wid_dih_gauss" << std::endl;

        for(std::vector<LineSptr>::iterator iter = lines.begin(); iter != lines.end(); ++iter)
        {
            (*iter)->write_stream(os);
        }

        os << ">>>>" << std::endl;
        os << std::endl;
        return;
    }

    void AicgdihBlock::read_block(std::ifstream& is)
    {
        // it is expected that
        // this reads a file after checked that
        // the first line "<<<<" has valid block type.
        //
        // this may be completed by NinfoFileReader class.

        while(!is.eof())
        {
            LineSptr line(new AicgdihLine);

            switch( line->readline(is) )
            {
            case NINFO_LINE:
                lines.push_back(line);
                break;
            case BLOCK_END:
                return;
            case COMMENT:
                break;
            case EMPTY:
                break;
            case WRONG:
                throw std::invalid_argument("File Reading Error. aicgdih block in ninfo file has invalid line.");
            case BLOCK_START:
                throw std::invalid_argument("File Reading Error. aicgdih block in ninfo file is not closed by <<<<.");
            default:
                throw std::invalid_argument("File Reading Error. Unknown LineType.");
            }
        }
        throw std::invalid_argument("File Reading Error. aicgdih block in ninfo file did not end correctly.");
    }

}
#endif
