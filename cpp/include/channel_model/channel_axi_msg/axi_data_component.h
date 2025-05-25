#ifndef AXI_DATA_COMPONENT_H
#define AXI_DATA_COMPONENT_H

#include "../../conf/global_conf.h"

class axi_data_component
{
  private:
    static constexpr unsigned int DATA_WIDTH = 512;
    u512 data;

  public:
    explicit axi_data_component(u512 data) : data(data)
    {
        // CPP_ASSERT(this->data < (1 << DATA_WIDTH));
    }
    axi_data_component(const axi_data_component &other) = default;
    axi_data_component &operator=(const axi_data_component &other) = default;
    ~axi_data_component() = default;

  public:
    u512 get_data() const
    {
        return this->data;
    }

  private:  // Disabled
    void set_data(u512 data)
    {
        this->data = data;
    }
};

#endif