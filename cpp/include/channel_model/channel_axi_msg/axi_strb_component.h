#ifndef AXI_STRB_COMPONENT_H
#define AXI_STRB_COMPONENT_H

#include "../../conf/global_conf.h"

class axi_strb_component
{
  private:
    static constexpr unsigned int STRB_WIDTH = 64;
    u64 strb;

  public:
    explicit axi_strb_component(u64 strb) : strb(strb)
    {
        CPP_ASSERT(this->strb < (1 << STRB_WIDTH));
    }
    axi_strb_component(const axi_strb_component &other) = default;
    axi_strb_component &operator=(const axi_strb_component &other) = default;
    ~axi_strb_component() = default;

  public:
    u64 get_strb() const
    {
        return this->strb;
    }
    u32 get_strb_high() const
    {
        return (u32)(this->strb >> 32);
    }
    u32 get_strb_low() const
    {
        return (u32)(this->strb & 0xFFFFFFFF);
    }

  private:  // Disabled
    void set_strb(u64 strb)
    {
        this->strb = strb;
        CPP_ASSERT(this->strb < (1 << STRB_WIDTH));
    }
};

#endif