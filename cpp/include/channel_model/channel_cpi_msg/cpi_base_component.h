#ifndef CPI_BASE_COMPONENT_H
#define CPI_BASE_COMPONENT_H

#include "../../conf/global_conf.h"

#define FLITMODE_68 0b00   // 68BYTE_FLIT
#define FLITMODE_256 0b01  // 256BYTE_FLIT
#define FLITMODE_PBR 0b10  // PBR_FLIT

class cpi_base_component
{
  protected:
    u32 flitmode;

  public:
    explicit cpi_base_component(u32 flitmode) : flitmode(flitmode)
    {
        CPP_ASSERT(this->flitmode == FLITMODE_68 || this->flitmode == FLITMODE_256 || this->flitmode == FLITMODE_PBR);
    }
    cpi_base_component(const cpi_base_component &other) = default;
    cpi_base_component &operator=(const cpi_base_component &other) = default;
    ~cpi_base_component() = default;

  public:
    u32 get_flitmode() const
    {
        return this->flitmode;
    }
    std::string get_flitmode_str() const
    {
        if (this->flitmode == FLITMODE_68) {
            return "68BYTE_FLIT";
        } else if (this->flitmode == FLITMODE_256) {
            return "256BYTE_FLIT";
        } else if (this->flitmode == FLITMODE_PBR) {
            return "PBR_FLIT";
        } else {
            return "UNKNOWN_FLITMODE";
        }
    }

  private:  // Disabled
    void set_flitmode(u32 flitmode)
    {
        this->flitmode = flitmode;
        CPP_ASSERT(this->flitmode == FLITMODE_68 || this->flitmode == FLITMODE_256 || this->flitmode == FLITMODE_PBR);
    }

    // ------------------------ Disabled because it is in sv
  private:
    u32 address_parity;  // Disabled because it is in sv
    u32 cmd_parity;      // Disabled because it is in sv

  private:
    u32 get_address_parity() const
    {
        return this->address_parity;
    }
    u32 get_cmd_parity() const
    {
        return this->cmd_parity;
    }

    void set_address_parity(u32 address_parity)
    {
        this->address_parity = address_parity;
        CPP_ASSERT(this->address_parity < (1 << 1));
    }
    void set_cmd_parity(u32 cmd_parity)
    {
        this->cmd_parity = cmd_parity;
        CPP_ASSERT(this->address_parity < (1 << 1));
    }
};

#endif
