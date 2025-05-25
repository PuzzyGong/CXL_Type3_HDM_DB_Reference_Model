#ifndef D2S_AXI_RDDATA_MSG_H
#define D2S_AXI_RDDATA_MSG_H

#include "../valid_component.h"
#include "axi_id_component.h"
#include "axi_data_component.h"
#include "axi_resp_component.h"
#include "axi_user_component.h"

#include "../tools/print_tool.h"

class d2s_axi_rddata_msg : public valid_component,
                           public axi_id_component,
                           public axi_data_component,
                           public axi_resp_component,
                           public axi_user_component
{
  public:
    explicit d2s_axi_rddata_msg(u32 valid, u32 id, u512 data, u32 resp, u64 user)
        : valid_component(valid), axi_id_component(id), axi_data_component(data), axi_resp_component(resp), axi_user_component(user)
    {
    }
    explicit d2s_axi_rddata_msg() : d2s_axi_rddata_msg(false, 0, u512(0), 0, 0)
    {
    }
    d2s_axi_rddata_msg(const d2s_axi_rddata_msg &other) = default;
    d2s_axi_rddata_msg &operator=(const d2s_axi_rddata_msg &other) = default;
    ~d2s_axi_rddata_msg() = default;

  public:
    void print(u32 tag) const
    {
        print_tool::start(print_tool::Direction::S2D, 0, tag, "rddata :  ");
        print_tool::elecment("data :", "[", "]", get_data().get(0), 3);
#ifndef SIMPLE_PRINT
        // to be added
#endif
        std::cout << std::endl;
    }
};

#endif
