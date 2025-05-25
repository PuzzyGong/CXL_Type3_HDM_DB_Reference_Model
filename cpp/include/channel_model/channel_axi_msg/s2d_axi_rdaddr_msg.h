#ifndef S2D_AXI_RDADDR_MSG_H
#define S2D_AXI_RDADDR_MSG_H

#include "../valid_component.h"
#include "./axi_id_component.h"
#include "./axi_address_component.h"
#include "./axi_user_component.h"

#include "../tools/print_tool.h"

class s2d_axi_rdaddr_msg : public valid_component, public axi_id_component, public axi_address_component, public axi_user_component
{
  public:  // 非硬件接口。因为由 Device Model 输出，所以可以增加一个成员用于外部打印，而不连接除了打印之外的其他硬件接口
    u32 tag;

  public:
    explicit s2d_axi_rdaddr_msg(u32 valid, u32 id, u64 address, u64 user, u32 tag)
        : valid_component(valid), axi_id_component(id), axi_address_component(address), axi_user_component(user), tag(tag)
    {
    }
    explicit s2d_axi_rdaddr_msg() : s2d_axi_rdaddr_msg(false, 0, 0, 0, 0)
    {
    }

    s2d_axi_rdaddr_msg(const s2d_axi_rdaddr_msg &other) = default;
    s2d_axi_rdaddr_msg &operator=(const s2d_axi_rdaddr_msg &other) = default;
    ~s2d_axi_rdaddr_msg() = default;

  public:
    void print() const
    {
        print_tool::start(print_tool::Direction::S2D, 0, tag, "rdaddr :  ");
        print_tool::elecment("addr :", "(", ")", get_address(), 3);
#ifndef SIMPLE_PRINT
        // to be added
#endif
        std::cout << std::endl;
    }
};

#endif
