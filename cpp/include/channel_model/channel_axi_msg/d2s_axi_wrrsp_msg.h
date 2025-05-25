#ifndef D2S_AXI_WRRSP_MSG_H
#define D2S_AXI_WRRSP_MSG_H

#include "../valid_component.h"
#include "./axi_id_component.h"
#include "./axi_resp_component.h"
#include "./axi_user_component.h"
#include "../tools/print_tool.h"

class d2s_axi_wrrsp_msg : public valid_component, public axi_id_component, public axi_resp_component, public axi_user_component
{
  public:
    explicit d2s_axi_wrrsp_msg(u32 valid, u32 id, u32 resp, u64 user)
        : valid_component(valid), axi_id_component(id), axi_resp_component(resp), axi_user_component(user)
    {
    }
    explicit d2s_axi_wrrsp_msg() : d2s_axi_wrrsp_msg(false, 0, 0, 0)
    {
    }
    d2s_axi_wrrsp_msg(const d2s_axi_wrrsp_msg &other) = default;
    d2s_axi_wrrsp_msg &operator=(const d2s_axi_wrrsp_msg &other) = default;
    ~d2s_axi_wrrsp_msg() = default;

  public:
    void print(u32 tag) const
    {
        print_tool::start(print_tool::Direction::S2D, 0, tag, "wrrsp  :  ");
        print_tool::elecment("resp :", "[", "]", get_resp(), 3);
#ifndef SIMPLE_PRINT
        // to be added
#endif
        std::cout << std::endl;
    }
};

#endif
