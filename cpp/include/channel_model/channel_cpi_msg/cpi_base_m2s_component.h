#ifndef CPI_BASE_M2S_COMPONENT_H
#define CPI_BASE_M2S_COMPONENT_H

#include "cpi_base_component.h"

class cpi_base_m2s_component : public cpi_base_component
{
  private:
    u32 spid;
    u32 dpid_ldid;

  public:
    explicit cpi_base_m2s_component(u32 flitmode, u32 spid, u32 dpid_ldid)
        : cpi_base_component(flitmode), spid(spid), dpid_ldid(dpid_ldid)
    {
        check_spid_validity();
        if (this->flitmode == FLITMODE_PBR) {
            check_dpid_validity();
        } else if (this->flitmode == FLITMODE_68 || this->flitmode == FLITMODE_256) {
            check_ldid_validity();
        }
    }
    cpi_base_m2s_component(const cpi_base_m2s_component &other) = default;
    cpi_base_m2s_component &operator=(const cpi_base_m2s_component &other) = default;
    ~cpi_base_m2s_component() = default;

  public:
    u32 get_ldid() const
    {
        check_ldid_validity();
        return this->dpid_ldid;
    }
    u32 get_spid() const
    {
        return this->spid;
    }
    u32 get_dpid() const
    {
        check_dpid_validity();
        return this->dpid_ldid;
    }
    u32 get_hostid() const
    {
        return this->spid;
    }
    u32 get_deviceid() const
    {
        return this->dpid_ldid;
    }

  public:
    void set_spid(u32 spid)
    {
        this->spid = spid;
        check_spid_validity();
    }

  private:  // Disabled
    void set_ldid(u32 ldid)
    {
        this->dpid_ldid = ldid;
        check_ldid_validity();
    }
    void set_dpid(u32 dpid)
    {
        this->dpid_ldid = dpid;
        check_dpid_validity();
    }

  private:
    void check_spid_validity() const
    {
        CPP_ASSERT(this->spid < (1 << 12));
    }
    void check_dpid_validity() const
    {
        if (this->flitmode != FLITMODE_PBR) {
            CPP_WARNING("68 or 256 mode is not supported for a dpid input");
        }
        CPP_ASSERT(this->dpid_ldid < (1 << 12));
    }
    void check_ldid_validity() const
    {
        if (this->flitmode != FLITMODE_68 && this->flitmode != FLITMODE_256) {
            CPP_WARNING("PBR mode is not supported for a ldid input");
        }
        CPP_ASSERT(this->dpid_ldid < (1 << 4));
    }
};

#endif
