#ifndef S2D_AXI_WRADDR_MSG_H
#define S2D_AXI_WRADDR_MSG_H

#include "../valid_component.h"
#include "./axi_id_component.h"
#include "./axi_address_component.h"
#include "./axi_data_component.h"
#include "./axi_derr_component.h"
#include "./axi_strb_component.h"
#include "./axi_user_component.h"

#include "../tools/print_tool.h"

class s2d_axi_wraddr_msg : public valid_component,
                           public axi_id_component,
                           public axi_address_component,
                           public axi_data_component,
                           public axi_derr_component,
                           public axi_strb_component,
                           public axi_user_component

{
  public:  // 非硬件接口。因为由 Device Model 输出，所以可以增加一个成员用于外部打印，而不连接除了打印之外的其他硬件接口
    u32 tag;

  public:
    explicit s2d_axi_wraddr_msg(u32 valid, u32 id, u64 address, u512 data, u64 derr, u64 strb, u64 user, u32 tag)
        : valid_component(valid), axi_id_component(id), axi_address_component(address), axi_data_component(data),
          axi_derr_component(derr), axi_strb_component(strb), axi_user_component(user), tag(tag)
    {
    }
    explicit s2d_axi_wraddr_msg() : s2d_axi_wraddr_msg(false, 0, 0, u512(), 0, 0, 0, 0)
    {
    }

    s2d_axi_wraddr_msg(const s2d_axi_wraddr_msg &other) = default;
    s2d_axi_wraddr_msg &operator=(const s2d_axi_wraddr_msg &other) = default;
    ~s2d_axi_wraddr_msg() = default;

  public:
    void print() const
    {
        print_tool::start(print_tool::Direction::S2D, 0, tag, "wraddr :  ");
        print_tool::elecment("addr :", "(", ")", get_address(), 3);
        print_tool::elecment("data :", "[", "]", get_data().get(0), 3);
        print_tool::elecment("strb :", "[", "]", get_strb(), 3);
#ifndef SIMPLE_PRINT
        // to be added
#endif
        std::cout << std::endl;
    }
};

#endif
