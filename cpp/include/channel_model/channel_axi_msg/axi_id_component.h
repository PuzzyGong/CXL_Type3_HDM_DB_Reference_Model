#ifndef AXI_ID_COMPONENT_H
#define AXI_ID_COMPONENT_H

#include "../../conf/global_conf.h"

class axi_id_component
{
  private:
    static constexpr unsigned int ID_WIDTH = 4;
    u32 id;

  public:
    explicit axi_id_component(u32 id) : id(id)
    {
        CPP_ASSERT(this->id < (1 << ID_WIDTH));
    }
    axi_id_component(const axi_id_component &other) = default;
    axi_id_component &operator=(const axi_id_component &other) = default;
    ~axi_id_component() = default;

  public:
    u32 get_id() const
    {
        return this->id;
    }

  private:  // Disabled
    void set_id(u32 id)
    {
        this->id = id;
        CPP_ASSERT(this->id < (1 << ID_WIDTH));
    }
};

#endif