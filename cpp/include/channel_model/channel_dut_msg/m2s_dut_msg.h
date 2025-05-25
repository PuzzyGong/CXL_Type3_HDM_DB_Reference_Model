#ifndef S2M_DUT_MSG_H
#define S2M_DUT_MSG_H

#include "../valid_component.h"

class m2s_dut_msg : public valid_component
{
  public:
    explicit m2s_dut_msg(u32 valid) : valid_component(valid)
    {
    }
    m2s_dut_msg(const m2s_dut_msg &other) = default;
    m2s_dut_msg &operator=(const m2s_dut_msg &other) = default;
    ~m2s_dut_msg() = default;

};

#endif
