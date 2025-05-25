#ifndef M2S_CPI_BIRSP_MSG_H
#define M2S_CPI_BIRSP_MSG_H

#include "../valid_component.h"
#include "./cpi_base_m2s_component.h"
#include "./cpi_bitag_component.h"
#include "./s2m_cpi_bisnp_msg.h"

#include "../tools/print_tool.h"

class m2s_cpi_birsp_msg : public valid_component, public cpi_base_m2s_component, public cpi_bitag_component
{
  public:
    static constexpr unsigned int BIRspI = 0b0000;
    static constexpr unsigned int BIRspS = 0b0001;
    static constexpr unsigned int BIRspE = 0b0010;
    static constexpr unsigned int BIRspIBlk = 0b0100;  // Disabled
    static constexpr unsigned int BIRspSBlk = 0b0101;  // Disabled
    static constexpr unsigned int BIRspEBlk = 0b0110;  // Disabled

  private:
    u32 opcode;

  public:
    // 无 68B 模式构造函数

    // 256B 模式构造函数，PBR 模式构造函数
    explicit m2s_cpi_birsp_msg(u32 valid, u32 flitmode, u32 opcode, u32 bitag, u32 spid, u32 dpid_ldid)
        : valid_component(valid), cpi_base_m2s_component(flitmode, spid, dpid_ldid), cpi_bitag_component(bitag), opcode(opcode)
    {
        CPP_ASSERT(flitmode == FLITMODE_256 || flitmode == FLITMODE_PBR);
        CPP_ASSERT(opcode == BIRspI || opcode == BIRspS || opcode == BIRspE || opcode == BIRspIBlk || opcode == BIRspSBlk
                   || opcode == BIRspEBlk);
    }
    explicit m2s_cpi_birsp_msg() : m2s_cpi_birsp_msg(0, FLITMODE_68, BIRspI, 0, 0, 0)
    {
    }
    m2s_cpi_birsp_msg(const m2s_cpi_birsp_msg &other) = default;
    m2s_cpi_birsp_msg &operator=(const m2s_cpi_birsp_msg &other) = default;
    ~m2s_cpi_birsp_msg() = default;

  public:
    u32 get_opcode() const
    {
        return this->opcode;
    }
    std::string get_opcode_str() const
    {
        switch (this->opcode) {
            case BIRspI:
                return "BIRspI";
            case BIRspS:
                return "BIRspS";
            case BIRspE:
                return "BIRspE";
            case BIRspIBlk:
                return "BIRspIBlk";
            case BIRspSBlk:
                return "BIRspSBlk";
            case BIRspEBlk:
                return "BIRspEBlk";
            default:
                return "Unknown";
        }
    }

  private:  // Disabled
    void set_opcode(u32 opcode)
    {
        this->opcode = opcode;
        CPP_ASSERT(opcode == BIRspI || opcode == BIRspS || opcode == BIRspE || opcode == BIRspIBlk || opcode == BIRspSBlk
                   || opcode == BIRspEBlk);
    }

  public:
    void print(u32 tag) const
    {
        print_tool::start(print_tool::Direction::M2S, 1, tag, "birsp :  " + get_opcode_str());
        print_tool::elecment("bitag:", "<", ">", get_bitag(), 3);
        print_tool::elecment("host :", "$", "$", get_hostid(), 3);
#ifndef SIMPLE_PRINT
        // to be added
#endif
        std::cout << std::endl;
    }
};

#endif
