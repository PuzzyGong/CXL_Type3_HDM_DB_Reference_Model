#ifndef VALID_COMPONENT_H
#define VALID_COMPONENT_H

#include "../conf/global_conf.h"

class valid_component
{
  private:
    u32 valid;

  public:
    explicit valid_component(u32 valid) : valid(valid)
    {
        CPP_ASSERT(valid == 0 || valid == 1);
    }
    ~valid_component() = default;

    void set_valid(u32 valid)
    {
        this->valid = valid;
        CPP_ASSERT(valid == 0 || valid == 1);
    }

    u32 get_valid() const
    {
        return this->valid;
    }
};

#endif
