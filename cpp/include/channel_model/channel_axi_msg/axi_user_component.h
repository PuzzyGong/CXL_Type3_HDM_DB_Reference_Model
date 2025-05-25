#ifndef AXI_USER_COMPONENT_H
#define AXI_USER_COMPONENT_H

#include "../../conf/global_conf.h"

class axi_user_component
{
  private:
    static constexpr unsigned int USER_WIDTH = 42;
    u64 user;

  public:
    explicit axi_user_component(u64 user) : user(user)
    {
        CPP_ASSERT(this->user < (1 << USER_WIDTH));
    }
    axi_user_component(const axi_user_component &other) = default;
    axi_user_component &operator=(const axi_user_component &other) = default;
    ~axi_user_component() = default;

  public:
    u32 get_emd() const
    {
        return (u32)(this->user >> 10) & 0xFFFF;
    }
    u32 get_metadata() const
    {
        return (u32)(this->user & 0x3);
    }

    u64 get_user() const
    {
        return this->user;
    }
    u32 get_user_high() const
    {
        return (u32)(this->user >> 32);
    }
    u32 get_user_low() const
    {
        return (u32)(this->user & 0xFFFFFFFF);
    }

  private:  // Disabled
    void set_user(u64 user)
    {
        this->user = user;
        CPP_ASSERT(this->user < (1 << USER_WIDTH));
    }
};

#endif