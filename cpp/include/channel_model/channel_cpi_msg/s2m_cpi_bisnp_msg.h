#ifndef S2M_CPI_BISNP_MSG_H
#define S2M_CPI_BISNP_MSG_H

#include "../valid_component.h"
#include "./cpi_base_s2m_component.h"
#include "./cpi_bitag_component.h"
#include "./cpi_address_component.h"

#include "../tools/print_tool.h"

class s2m_cpi_bisnp_msg : public valid_component,
                          public cpi_base_s2m_component,
                          public cpi_bitag_component,
                          public cpi_address_component
{
  public:  // 非硬件接口。因为由 Device Model 输出，所以可以增加一个成员用于外部打印，而不连接除了打印之外的其他硬件接口
    u32 tag;

  public:
    static constexpr unsigned int NoOp = 0b000;
    static constexpr unsigned int SnpData = 0b001;
    static constexpr unsigned int SnpCur = 0b010;
    static constexpr unsigned int SnpInv = 0b011;

  private:
    u32 opcode;

  public:
    // 无 68B 模式构造函数

    // 256B 模式构造函数，PBR 模式构造函数
    explicit s2m_cpi_bisnp_msg(u32 valid, u32 flitmode, u32 opcode, u32 bitag, u32 address, u32 spid_ldid, u32 dpid, u32 tag)
        : valid_component(valid), cpi_base_s2m_component(flitmode, spid_ldid, dpid), cpi_bitag_component(bitag),
          cpi_address_component(address), opcode(opcode), tag(tag)
    {
        CPP_ASSERT(flitmode == FLITMODE_256 || flitmode == FLITMODE_PBR);
        CPP_ASSERT(opcode == NoOp || opcode == SnpData || opcode == SnpCur || opcode == SnpInv);
    }
    explicit s2m_cpi_bisnp_msg() : s2m_cpi_bisnp_msg(false, FLITMODE_68, NoOp, 0, 0, 0, 0, 0)
    {
    }
    s2m_cpi_bisnp_msg(const s2m_cpi_bisnp_msg &other) = default;
    s2m_cpi_bisnp_msg &operator=(const s2m_cpi_bisnp_msg &other) = default;
    ~s2m_cpi_bisnp_msg() = default;

  public:
    u32 get_opcode() const
    {
        return this->opcode;
    }
    std::string get_opcode_str() const
    {
        switch (this->opcode) {
            case NoOp:
                return "NoOp";
            case SnpData:
                return "SnpData";
            case SnpCur:
                return "SnpCur";
            case SnpInv:
                return "SnpInv";
            default:
                return "Unknown";
        }
    }

  private:  // Disabled
    void set_opcode(u32 opcode)
    {
        this->opcode = opcode;
        CPP_ASSERT(opcode == NoOp || opcode == SnpData || opcode == SnpCur || opcode == SnpInv);
    }

  public:
    void print() const
    {
        print_tool::start(print_tool::Direction::S2M, 1, tag, "bisnp :  " + get_opcode_str());
        print_tool::elecment("bitag:", "<", ">", get_bitag(), 3);
        print_tool::elecment("host :", "$", "$", get_hostid(), 3);
        print_tool::elecment("addr :", "(", ")", get_address(), 3);
#ifndef SIMPLE_PRINT
        // to be added
#endif
        std::cout << std::endl;
    }
};

#endif
