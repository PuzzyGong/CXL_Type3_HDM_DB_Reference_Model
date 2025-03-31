#include "port.h"

// Print_Parameters
#define COLORFUL_PRINT
#define DONT_PRINT_LD_PID
#define S2M_SPACE "                                                                                     "
#define MEM_SPACE "                                                                                                                                                 "
// -------------------------

#define RESET           "\033[0m"
#define RED             "\033[31m"
#define GREEN           "\033[32m"
#define YELLOW          "\033[33m"
#define BLUE            "\033[34m"
#define MAGENTA         "\033[35m"
#define CYAN            "\033[36m"
#define LIGHT_RED       "\033[91m"
#define LIGHT_GREEN     "\033[92m"
#define LIGHT_YELLOW    "\033[93m"
#define LIGHT_BLUE      "\033[94m"
#define LIGHT_MAGENTA   "\033[95m"
#define LIGHT_CYAN      "\033[96m"
const std::string color_array[12] = {RED, GREEN, YELLOW, BLUE, MAGENTA, CYAN, LIGHT_RED, LIGHT_GREEN, LIGHT_YELLOW, LIGHT_BLUE, LIGHT_MAGENTA, LIGHT_CYAN};

class Colorful_Print_Manager
{
private:
    class Elecment
    {
    public:
        unsigned int color_index;
        u64 address;
        u32 tag;
        std::list<u32> bi_tag_list;
        bool had_been_ndr_drs;

        Elecment(unsigned int color_index, u64 address, u32 tag) : color_index(color_index), address(address), tag(tag), had_been_ndr_drs(false){}
    };

public:
    std::list<Elecment> list;
    unsigned int color_array_index = 0;

    std::string req_get_color(u64 address, u32 tag)
    {
        color_array_index = (color_array_index == 6 - 1) ? 0 : color_array_index + 1;
        list.push_back(Elecment(color_array_index, address, tag));
        if (list.size() > 6)
            list.pop_front(); // prevent Heap Overflow / Out Of Memory
        return color_array[color_array_index];
    }

    std::string bisnp_get_color(u64 address, u32 bi_tag)
    {
        for (auto it = list.begin(); it != list.end(); ++it)
        {
            if (it->address == address && !it->had_been_ndr_drs)
            {
                it->bi_tag_list.push_back(bi_tag);
                if (it->bi_tag_list.size() > 10)
                    it->bi_tag_list.pop_back(); // prevent Heap Overflow / Out Of Memory
                return color_array[it->color_index + 6];
            }
        }
        return RESET;
    }

    std::string birsp_get_color(u32 bi_tag)
    {
        for (auto it = list.begin(); it != list.end(); ++it)
        {
            for (auto i : it->bi_tag_list)
            {
                if (i == bi_tag && !it->had_been_ndr_drs)
                    return color_array[it->color_index + 6];
            }
        }
        return RESET;
    }

    std::string ndr_drs_get_color(u32 tag)
    {
        for (auto it = list.begin(); it != list.end(); ++it)
        {
            if (it->tag == tag){
                it->had_been_ndr_drs = true;
                return color_array[it->color_index];
            }
        }
        return RESET;
    }
    std::string others_get_color(u64 address)
    {
        for (auto it = list.begin(); it != list.end(); ++it)
        {
            if (it->address == address && !it->had_been_ndr_drs)
                return color_array[it->color_index];
        }
        return RESET;
    }
};
static Colorful_Print_Manager colorful_print_manager;

void S2M_Snp_type::print() const
{
    std::string color = RESET;
#ifdef COLORFUL_PRINT
    color = colorful_print_manager.bisnp_get_color(address, bitag);
#endif

    std::cout << std::left << std::setfill(' ') << color
              << S2M_SPACE << "S2M_BISnp :  " << std::setw(10) << get_opcode_name()

              << std::right << std::setfill('0')
              << "BITag:" << RESET << "<" << color << std::setw(3) << std::to_string(bitag % 1000) << RESET << "> " << color
              << "DPID :" << RESET << "$" << color << std::setw(3) << std::to_string(dpid % 1000) << RESET << "$ " << color
              << "Addr :" << RESET << "(" << color << std::setw(3) << std::to_string(address % 1000) << RESET << ") " << color
#ifndef DONT_PRINT_LD_PID
              << "SPID :" << std::setw(4) << (spid % 1000)
#endif
              << RESET << std::endl;
}

void S2M_NDR_Type::print() const
{
    std::string color = RESET;
#ifdef COLORFUL_PRINT
    color = colorful_print_manager.ndr_drs_get_color(tag);
#endif

    std::cout << std::left << std::setfill(' ') << color
              << S2M_SPACE << "S2M_NDR   :  " << std::setw(10) << get_opcode_name()

              << std::right << std::setfill('0')
              << "Tag  :" << RESET << "<" << color << std::setw(3) << std::to_string(tag % 1000) << RESET << "> " << color
              << "DPID :" << RESET << "$" << color << std::setw(3) << std::to_string(dpid % 1000) << RESET << "$ " << color
              << RESET << std::endl;
}

void S2M_DRS_Type::print() const
{
    std::string color = RESET;
#ifdef COLORFUL_PRINT
    color = colorful_print_manager.ndr_drs_get_color(tag);
#endif

    std::cout << std::left << std::setfill(' ') << color
              << S2M_SPACE << "S2M_DRS   :  " << std::setw(10) << get_opcode_name()

              << std::right << std::setfill('0')
              << "Tag  :" << RESET << "<" << color << std::setw(3) << std::to_string(tag % 1000) << RESET << "> " << color
              << "DPID :" << RESET << "$" << color << std::setw(3) << std::to_string(dpid % 1000) << RESET << "$ " << color
              << "Data :" << RESET << "[" << color << std::setw(3) << std::to_string(data_body % 1000) << RESET << "] " << color
              << RESET << std::endl;
}

void M2S_Req_Type::print() const
{
    std::string color = RESET;
#ifdef COLORFUL_PRINT
    color = colorful_print_manager.req_get_color(address, tag);
#endif

    std::cout << std::left << std::setfill(' ') << color
              << "M2S_Req   :  " << std::setw(22) << get_memopcode_name() + " + " + get_snptype_name() + " + " + get_metavalue_name()

              << std::right << std::setfill('0')
              << "Tag  :" << RESET << "<" << color << std::setw(3) << std::to_string(tag % 1000) << RESET << "> " << color
              << "SPID :" << RESET << "$" << color << std::setw(3) << std::to_string(spid % 1000) << RESET << "$ " << color
              << "Addr :" << RESET << "(" << color << std::setw(3) << std::to_string(address % 1000) << RESET << ") " << color
#ifndef DONT_PRINT_LD_PID
              << "DPID :" << std::setw(4) << (dpid % 1000)
#endif
              << RESET << std::endl;
}

void M2S_Rsp_type::print() const
{
    std::string color = RESET;
#ifdef COLORFUL_PRINT
    color = colorful_print_manager.birsp_get_color(bitag);
#endif

    std::cout << std::left << std::setfill(' ') << color
              << "M2S_BIRsp :  " << std::setw(22) << get_opcode_name()

              << std::right << std::setfill('0')
              << "BITag:" << RESET << "<" << color << std::setw(3) << std::to_string(bitag % 1000) << RESET << "> " << color
#ifndef DONT_PRINT_LD_PID
              << "DPID :" << std::setw(4) << (dpid % 1000)
#endif
              << RESET << std::endl;
}

void M2S_RwD_Type::print() const
{
    std::string color = RESET;
#ifdef COLORFUL_PRINT
    color = colorful_print_manager.others_get_color(address);
#endif

    std::cout << std::left << std::setfill(' ') << color
              << "M2S_RwD   :  " << std::setw(22) << get_memopcode_name() + " + " + get_snptype_name() + " + " + get_metavalue_name()

              << std::right << std::setfill('0')
              << "Tag  :" << RESET << "<" << color << std::setw(3) << std::to_string(tag % 1000) << RESET << "> " << color
              << "SPID :" << RESET << "$" << color << std::setw(3) << std::to_string(spid % 1000) << RESET << "$ " << color
              << "Addr :" << RESET << "(" << color << std::setw(3) << std::to_string(address % 1000) << RESET << ") " << color
              << "Data :" << RESET << "[" << color << std::setw(3) << std::to_string(data_body % 1000) << RESET << "] " << color
#ifndef DONT_PRINT_LD_PID
              << "DPID :" << std::setw(4) << (dpid % 1000)
#endif
              << RESET << std::endl;
}

void Rd_Addr_Type::print() const
{
    std::string color = RESET;
#ifdef COLORFUL_PRINT
    color = colorful_print_manager.others_get_color(address);
#endif

    std::cout << std::left << std::setfill(' ') << color
              << MEM_SPACE << "Rd_Addr   :  "

              << std::right << std::setfill('0')
              << "Addr :" << RESET << "(" << color << std::setw(3) << std::to_string(address % 1000) << RESET << ") " << color
              << RESET << std::endl;
}

void Rd_Data_Type::print() const
{
    std::string color = RESET;
#ifdef COLORFUL_PRINT
    color = colorful_print_manager.others_get_color(address);
#endif

    std::cout << std::left << std::setfill(' ') << color
              << MEM_SPACE << "Rd_Data   :  "

              << std::right << std::setfill('0')
              << "Addr :" << RESET << "(" << color << std::setw(3) << std::to_string(address % 1000) << RESET << ") " << color
              << "Data :" << RESET << "[" << color << std::setw(3) << std::to_string(data_body % 1000) << RESET << "] " << color
              << RESET << std::endl;
}

void Wr_Type::print() const
{
    std::string color = RESET;
#ifdef COLORFUL_PRINT
    color = colorful_print_manager.others_get_color(address);
#endif

    std::cout << std::left << std::setfill(' ') << color
              << MEM_SPACE << "Wr_Addr   :  "

              << std::right << std::setfill('0')
              << "Addr :" << RESET << "(" << color << std::setw(3) << std::to_string(address % 1000) << RESET << ") " << color
              << "Data :" << RESET << "[" << color << std::setw(3) << std::to_string(data_body % 1000) << RESET << "] " << color
              << RESET << std::endl;
}
