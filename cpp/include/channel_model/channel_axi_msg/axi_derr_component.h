#ifndef AXI_DERR_COMPONENT_H
#define AXI_DERR_COMPONENT_H

#include "../../conf/global_conf.h"

class axi_derr_component
{
  private:
    static constexpr unsigned int ENABLE_WIDTH = 64;
    u64 enable;

  public:
    explicit axi_derr_component(u64 enable) : enable(enable)
    {
        CPP_ASSERT(this->enable < (1 << ENABLE_WIDTH));
    }
    axi_derr_component(const axi_derr_component &other) = default;
    axi_derr_component &operator=(const axi_derr_component &other) = default;
    ~axi_derr_component() = default;

  public:
    u64 get_enable() const
    {
        return this->enable;
    }
    u32 get_enable_high() const
    {
        return (u32)(this->enable >> 32);
    }
    u32 get_enable_low() const
    {
        return (u32)(this->enable & 0xFFFFFFFF);
    }

  private:  // Disabled
    void set_enable(u64 enable)
    {
        this->enable = enable;
        CPP_ASSERT(this->enable < (1 << ENABLE_WIDTH));
    }
};

#endif