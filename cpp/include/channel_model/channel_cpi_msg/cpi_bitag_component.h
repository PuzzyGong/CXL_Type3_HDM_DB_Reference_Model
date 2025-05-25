#ifndef CPI_BITAG_COMPONENT_H
#define CPI_BITAG_COMPONENT_H

#include "../../conf/global_conf.h"

class cpi_bitag_component
{
  private:
    static constexpr unsigned int BITAG_WIDTH = 12;
    u32 bitag;

  public:
    explicit cpi_bitag_component(u32 bitag) : bitag(bitag)
    {
        CPP_ASSERT(this->bitag < (1 << BITAG_WIDTH));
    }
    cpi_bitag_component(const cpi_bitag_component &other) = default;
    cpi_bitag_component &operator=(const cpi_bitag_component &other) = default;
    ~cpi_bitag_component() = default;

  public:
    u32 get_bitag() const
    {
        return this->bitag;
    }

  private:  // Disabled
    void set_bitag(u32 bitag)
    {
        this->bitag = bitag;
        CPP_ASSERT(this->bitag < (1 << BITAG_WIDTH));
    }
};

#endif