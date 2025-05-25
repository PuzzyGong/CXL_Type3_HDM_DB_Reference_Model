#ifndef AXI_ADDRESS_COMPONENT_H
#define AXI_ADDRESS_COMPONENT_H

#include "../../conf/global_conf.h"

class axi_address_component
{
  private:
    static constexpr unsigned int ADDRESS_WIDTH = 64;
    u64 address;

  public:
    explicit axi_address_component(u64 address) : address(address)
    {
        CPP_ASSERT(this->address < (1 << ADDRESS_WIDTH));
    }
    axi_address_component(const axi_address_component &other) = default;
    axi_address_component &operator=(const axi_address_component &other) = default;
    ~axi_address_component() = default;

  public:
    u64 get_address() const
    {
        return this->address;
    }
    u32 get_address_high() const
    {
        return (u32)(this->address >> 32);
    }
    u32 get_address_low() const
    {
        return (u32)(this->address & 0xFFFFFFFF);
    }

  public:  // Enabled
    void set_address(u64 address)
    {
        this->address = address;
        CPP_ASSERT(this->address < (1 << ADDRESS_WIDTH));
    }
};

#endif