#ifndef M2S_CPI_REQ_MSG_H
#define M2S_CPI_REQ_MSG_H

#include "../valid_component.h"
#include "./cpi_base_m2s_component.h"
#include "./cpi_tag_component.h"
#include "./cpi_snp_component.h"
#include "./cpi_meta_component.h"
#include "./cpi_address_component.h"

#include "../tools/print_tool.h"

class m2s_cpi_req_msg : public valid_component,
                        public cpi_base_m2s_component,
                        public cpi_tag_component,
                        public cpi_snp_component,
                        public cpi_meta_component,
                        public cpi_address_component
{
  public:
    static constexpr unsigned int MemInv = 0b0000;
    static constexpr unsigned int MemRd = 0b0001;
    static constexpr unsigned int MemRdData = 0b0010;
    static constexpr unsigned int MemRdFwd = 0b0011;   // Disabled
    static constexpr unsigned int MemWrFwd = 0b0100;   // Disabled
    static constexpr unsigned int MemSpecRd = 0b1000;  // Disabled
    static constexpr unsigned int MemInvNT = 0b1001;
    static constexpr unsigned int MemClnEvct = 0b1010;

  private:
    static constexpr unsigned int ADDRESS5_WIDTH = 1;
    u32 memopcode;
    u32 address5;

  public:
    // 68B 模式构造函数
    explicit m2s_cpi_req_msg(u32 valid,
                             u32 flitmode,
                             u32 memopcode,
                             u32 tag,
                             u32 snptype,
                             u32 address5,
                             u32 metafield,
                             u32 metavalue,
                             u64 address,
                             u32 ldid,
                             u32 spid)
        : valid_component(valid), cpi_base_m2s_component(flitmode, spid, ldid), cpi_tag_component(tag), cpi_snp_component(snptype),
          cpi_meta_component(metafield, metavalue), cpi_address_component(address), memopcode(memopcode), address5(address5)
    {
        CPP_ASSERT(flitmode == FLITMODE_68);
        CPP_ASSERT(memopcode == MemInv || memopcode == MemRd || memopcode == MemRdData || memopcode == MemWrFwd
                   || memopcode == MemSpecRd || memopcode == MemInvNT || memopcode == MemClnEvct);
        CPP_ASSERT(address5 == 0 || address5 == 1);
    }

    // 256B 模式构造函数，PBR 模式构造函数
    explicit m2s_cpi_req_msg(u32 valid,
                             u32 flitmode,
                             u32 memopcode,
                             u32 tag,
                             u32 snptype,
                             u32 metafield,
                             u32 metavalue,
                             u64 address,
                             u32 spid,
                             u32 dpid_ldid)
        : valid_component(valid), cpi_base_m2s_component(flitmode, spid, dpid_ldid), cpi_tag_component(tag),
          cpi_snp_component(snptype), cpi_meta_component(metafield, metavalue), cpi_address_component(address), memopcode(memopcode)
    {
        CPP_ASSERT(flitmode == FLITMODE_256 || flitmode == FLITMODE_PBR);
        CPP_ASSERT(memopcode == MemInv || memopcode == MemRd || memopcode == MemRdData || memopcode == MemWrFwd
                   || memopcode == MemSpecRd || memopcode == MemInvNT || memopcode == MemClnEvct);
    }
    explicit m2s_cpi_req_msg() : m2s_cpi_req_msg(false, FLITMODE_68, MemInv, 0, 0, 0, 0, 0, 0, 0, 0)
    {
    }
    m2s_cpi_req_msg(const m2s_cpi_req_msg &other) = default;
    m2s_cpi_req_msg &operator=(const m2s_cpi_req_msg &other) = default;
    ~m2s_cpi_req_msg() = default;

  public:
    u32 get_memopcode() const
    {
        return this->memopcode;
    }
    std::string get_memopcode_str() const
    {
        switch (this->memopcode) {
            case MemInv:
                return "MemInv";
            case MemRd:
                return "MemRd";
            case MemRdData:
                return "MemRdData";
            case MemRdFwd:
                return "MemRdFwd";
            case MemWrFwd:
                return "MemWrFwd";
            case MemSpecRd:
                return "MemSpecRd";
            case MemInvNT:
                return "MemInvNT";
            case MemClnEvct:
                return "MemClnEvct";
            default:
                return "Unknown";
        }
    }
    u32 get_address5() const
    {
        return this->address5;
    }

  private:  // Disabled
    void set_memopcode(u32 memopcode)
    {
        this->memopcode = memopcode;
        CPP_ASSERT(memopcode == MemInv || memopcode == MemRd || memopcode == MemRdData || memopcode == MemWrFwd
                   || memopcode == MemSpecRd || memopcode == MemInvNT || memopcode == MemClnEvct);
    }
    void set_address5(u32 address5)
    {
        this->address5 = address5;
        CPP_ASSERT(address5 == 0 || address5 == 1);
    }

  public:
    void print() const
    {
        print_tool::start(print_tool::Direction::M2S, 0, get_tag(),
                          "req   :  " + get_memopcode_str() + " + " + get_snptype_str() + " + " + get_metafield_str() + " + "
                              + get_metavalue_str());
        print_tool::elecment("tag  :", "<", ">", get_tag(), 3);
        print_tool::elecment("host :", "$", "$", get_hostid(), 3);
        print_tool::elecment("addr :", "(", ")", get_address(), 3);
#ifndef SIMPLE_PRINT
        // to be added
#endif
        std::cout << std::endl;
    }
};

#endif
