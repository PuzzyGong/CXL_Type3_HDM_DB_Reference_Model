#ifndef AXI_RESP_COMPONENT_H
#define AXI_RESP_COMPONENT_H

#include "../../conf/global_conf.h"

class axi_resp_component
{
  private:
    static constexpr unsigned int RESP_WIDTH = 2;
    u32 resp;

  public:
    explicit axi_resp_component(u32 resp) : resp(resp)
    {
        CPP_ASSERT(this->resp < (1 << RESP_WIDTH));
    }
    axi_resp_component(const axi_resp_component &other) = default;
    axi_resp_component &operator=(const axi_resp_component &other) = default;
    ~axi_resp_component() = default;

  public:
    u32 get_resp() const
    {
        return this->resp;
    }

  private:  // Disabled
    void set_resp(u32 resp)
    {
        this->resp = resp;
        CPP_ASSERT(this->resp < (1 << RESP_WIDTH));
    }
};

#endif