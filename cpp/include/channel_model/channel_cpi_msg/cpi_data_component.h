#ifndef CPI_DATA_COMPONENT_H
#define CPI_DATA_COMPONENT_H

#include "../../conf/global_conf.h"

class cpi_data_component
{
  private:
    static constexpr unsigned int ENABLE_WIDTH = 6;
    static constexpr unsigned int POISON_WIDTH = 1;
    static constexpr unsigned int TRP_WIDTH = 1;
    static constexpr unsigned int EMD_WIDTH = 32;
    u512 databody;
    u64 enable;
    u32 poison;
    u32 trp;
    u32 emd;

  public:
    explicit cpi_data_component(u512 databody, u64 enable, u32 poison, u32 trp, u32 emd)
        : databody(databody), enable(enable), poison(poison), trp(trp), emd(emd)
    {
        CPP_ASSERT(enable < (1 << ENABLE_WIDTH));
        CPP_ASSERT(poison < (1 << POISON_WIDTH));
        CPP_ASSERT(trp < (1 << TRP_WIDTH));
        CPP_ASSERT(emd < (1 << EMD_WIDTH));
    }
    explicit cpi_data_component(u512 databody, u64 enable, u32 poison) : databody(databody), enable(enable), poison(poison)
    {
        CPP_ASSERT(enable < (1 << ENABLE_WIDTH));
        CPP_ASSERT(poison < (1 << POISON_WIDTH));
    }
    cpi_data_component(const cpi_data_component &other) = default;
    cpi_data_component &operator=(const cpi_data_component &other) = default;
    ~cpi_data_component() = default;

  public:
    u512 get_databody() const
    {
        return this->databody;
    }
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
    u32 get_poison() const
    {
        return this->poison;
    }
    u32 get_trp() const
    {
        return this->trp;
    }
    u32 get_emd() const
    {
        return this->emd;
    }

  private:  // Disabled
    void set_databody(u512 databody)
    {
        this->databody = databody;
    }
    void set_enable(u64 enable)
    {
        this->enable = enable;
        CPP_ASSERT(enable < (1 << ENABLE_WIDTH));
    }
    void set_poison(u32 poison)
    {
        this->poison = poison;
        CPP_ASSERT(poison < (1 << POISON_WIDTH));
    }
    void set_trp(u32 trp)
    {
        this->trp = trp;
        CPP_ASSERT(trp < (1 << TRP_WIDTH));
    }
    void set_emd(u32 emd)
    {
        this->emd = emd;
        CPP_ASSERT(emd < (1 << EMD_WIDTH));
    }

    // ------------------------ Disabled because it is in sv
  private:
    u32 data_parity;  // Disabled because it is in sv

  private:
    u32 get_data_parity() const
    {
        return this->data_parity;
    }
    void set_data_parity(u32 data_parity)
    {
        this->data_parity = data_parity;
        CPP_ASSERT(data_parity < (1 << 1));
    }
};

#endif