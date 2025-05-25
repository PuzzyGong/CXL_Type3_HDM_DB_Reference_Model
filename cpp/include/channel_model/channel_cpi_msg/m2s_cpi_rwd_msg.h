#ifndef M2S_CPI_RWD_MSG_H
#define M2S_CPI_RWD_MSG_H

#include "../valid_component.h"
#include "./cpi_base_m2s_component.h"
#include "./cpi_meta_component.h"
#include "./cpi_snp_component.h"
#include "./cpi_tag_component.h"
#include "./cpi_address_component.h"
#include "./cpi_data_component.h"

#include "../tools/print_tool.h"

class m2s_cpi_rwd_msg : public valid_component,
                        public cpi_base_m2s_component,
                        public cpi_meta_component,
                        public cpi_snp_component,
                        public cpi_tag_component,
                        public cpi_address_component,
                        public cpi_data_component
{
  public:
    static constexpr unsigned int MemInv = 0b0000;
    static constexpr unsigned int MemWr = 0b0001;
    static constexpr unsigned int MemWrPtl = 0b0010;
    static constexpr unsigned int BIConflict = 0b0100;

  private:
    u32 memopcode;

  public:
    // 68B 模式构造函数
    explicit m2s_cpi_rwd_msg(u32 valid,
                             u32 flitmode,
                             u32 memopcode,
                             u32 metafield,
                             u32 metavalue,
                             u32 snptype,
                             u32 tag,
                             u64 address,
                             u32 ldid,
                             u512 databody,
                             u64 enable,
                             u32 poison,
                             u32 spid)
        : valid_component(valid), cpi_base_m2s_component(flitmode, spid, ldid), cpi_meta_component(metafield, metavalue),
          cpi_snp_component(snptype), cpi_tag_component(tag), cpi_address_component(address),
          cpi_data_component(databody, enable, poison), memopcode(memopcode)
    {
        CPP_ASSERT(flitmode == FLITMODE_68);
        CPP_ASSERT(memopcode == 0b0000 || memopcode == MemWr || memopcode == MemWrPtl || memopcode == BIConflict);
    }

    // 256B 模式构造函数，PBR 模式构造函数
    explicit m2s_cpi_rwd_msg(u32 valid,
                             u32 flitmode,
                             u32 memopcode,
                             u32 metafield,
                             u32 metavalue,
                             u32 snptype,
                             u32 tag,
                             u64 address,
                             u32 spid,
                             u32 dpid_ldid,
                             u512 databody,
                             u64 enable,
                             u32 poison,
                             u32 trp,
                             u32 emd)
        : valid_component(valid), cpi_base_m2s_component(flitmode, spid, dpid_ldid), cpi_meta_component(metafield, metavalue),
          cpi_snp_component(snptype), cpi_tag_component(tag), cpi_address_component(address),
          cpi_data_component(databody, enable, poison, trp, emd), memopcode(memopcode)
    {
        CPP_ASSERT(flitmode == FLITMODE_256 || flitmode == FLITMODE_PBR);
        CPP_ASSERT(memopcode == 0b0000 || memopcode == MemWr || memopcode == MemWrPtl || memopcode == BIConflict);
    }
    explicit m2s_cpi_rwd_msg() : m2s_cpi_rwd_msg(false, FLITMODE_68, MemInv, 0, 0, 0, 0, 0, 0, u512(), 0, 0, 0)
    {
    }
    m2s_cpi_rwd_msg(const m2s_cpi_rwd_msg &other) = default;
    m2s_cpi_rwd_msg &operator=(const m2s_cpi_rwd_msg &other) = default;
    ~m2s_cpi_rwd_msg() = default;

  public:
    u32 get_memopcode() const
    {
        return this->memopcode;
    }
    std::string get_memopcode_str() const
    {
        switch (this->memopcode) {
            case 0b0000:
                return "MemInv";
            case MemWr:
                return "MemWr";
            case MemWrPtl:
                return "MemWrPtl";
            case BIConflict:
                return "BIConflict";
            default:
                return "Unknown";
        }
    }

  private:  // Disabled
    void set_memopcode(u32 memopcode)
    {
        this->memopcode = memopcode;
        CPP_ASSERT(memopcode == 0b0000 || memopcode == MemWr || memopcode == MemWrPtl || memopcode == BIConflict);
    }

  public:
    void print() const
    {
        print_tool::start(print_tool::Direction::M2S, 0, get_tag(),
                          "rwd   :  " + get_memopcode_str() + " + " + get_snptype_str() + " + " + get_metafield_str() + " + "
                              + get_metavalue_str());
        print_tool::elecment("tag  :", "<", ">", get_tag(), 3);
        print_tool::elecment("host :", "$", "$", get_hostid(), 3);
        print_tool::elecment("addr :", "(", ")", get_address(), 3);
        print_tool::elecment("data :", "[", "]", get_databody().get(0), 3);
        print_tool::elecment("strb :", "[", "]", get_enable(), 3);
#ifndef SIMPLE_PRINT
        // to be added
#endif
        std::cout << std::endl;
    }
};

#endif
