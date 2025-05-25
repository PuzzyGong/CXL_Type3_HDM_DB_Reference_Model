#ifndef S2M_CPI_NDR_MSG_H
#define S2M_CPI_NDR_MSG_H

#include "../valid_component.h"
#include "./cpi_base_s2m_component.h"
#include "./cpi_meta_component.h"
#include "./cpi_tag_component.h"

#include "../tools/print_tool.h"

class s2m_cpi_ndr_msg : public valid_component, public cpi_base_s2m_component, public cpi_meta_component, public cpi_tag_component
{
  public:
    static constexpr unsigned int Cmp = 0b000;
    static constexpr unsigned int CmpS = 0b001;
    static constexpr unsigned int CmpE = 0b010;
    static constexpr unsigned int BIConflictAck = 0b100;

  private:
    u32 opcode;

  public:
    // 68B 模式构造函数，256B 模式构造函数，PBR 模式构造函数
    explicit s2m_cpi_ndr_msg(u32 valid, u32 flitmode, u32 opcode, u32 metafield, u32 metadata, u32 tag, u32 spid_ldid, u32 dpid)
        : valid_component(valid), cpi_base_s2m_component(flitmode, spid_ldid, dpid), cpi_meta_component(metafield, metadata),
          cpi_tag_component(tag), opcode(opcode)
    {
        CPP_ASSERT(flitmode == FLITMODE_68 || flitmode == FLITMODE_256 || flitmode == FLITMODE_PBR);
        CPP_ASSERT(opcode == Cmp || opcode == CmpS || opcode == CmpE || opcode == BIConflictAck);
    }
    explicit s2m_cpi_ndr_msg() : s2m_cpi_ndr_msg(false, FLITMODE_68, 0, 0, 0, 0, 0, 0)
    {
    }
    s2m_cpi_ndr_msg(const s2m_cpi_ndr_msg &other) = default;
    s2m_cpi_ndr_msg &operator=(const s2m_cpi_ndr_msg &other) = default;
    ~s2m_cpi_ndr_msg() = default;

  public:
    u32 get_opcode() const
    {
        return this->opcode;
    }
    std::string get_opcode_str() const
    {
        switch (this->opcode) {
            case Cmp:
                return "Cmp";
            case CmpS:
                return "CmpS";
            case CmpE:
                return "CmpE";
            case BIConflictAck:
                return "BIConflictAck";
            default:
                return "Unknown";
        }
    }

  private:  // Disabled
    void set_opcode(u32 opcode)
    {
        this->opcode = opcode;
        CPP_ASSERT(opcode == Cmp || opcode == CmpS || opcode == CmpE || opcode == BIConflictAck);
    }

  public:
    void print() const
    {
        print_tool::start(print_tool::Direction::S2M, 0, get_tag(), "ndr   :  " + get_opcode_str());
        print_tool::elecment("tag  :", "<", ">", get_tag(), 3);
        print_tool::elecment("host :", "$", "$", get_hostid(), 3);
#ifndef SIMPLE_PRINT
        // to be added
#endif
        std::cout << std::endl;
    }
};

#endif
