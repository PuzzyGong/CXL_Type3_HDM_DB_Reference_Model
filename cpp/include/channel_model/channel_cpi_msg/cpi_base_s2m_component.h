#ifndef CPI_BASE_S2M_COMPONENT_H
#define CPI_BASE_S2M_COMPONENT_H

#include "cpi_base_component.h"

class cpi_base_s2m_component : public cpi_base_component
{
  private:
    u32 spid_ldid;
    u32 dpid;

  public:
    explicit cpi_base_s2m_component(u32 flitmode, u32 spid_ldid, u32 dpid)
        : cpi_base_component(flitmode), spid_ldid(spid_ldid), dpid(dpid)
    {
        check_dpid_validity();
        if (this->flitmode == FLITMODE_PBR) {
            check_spid_validity();
        } else if (this->flitmode == FLITMODE_68 || this->flitmode == FLITMODE_256) {
            check_ldid_validity();
        }
    }
    cpi_base_s2m_component(const cpi_base_s2m_component &other) = default;
    cpi_base_s2m_component &operator=(const cpi_base_s2m_component &other) = default;
    ~cpi_base_s2m_component() = default;

  public:
    u32 get_ldid() const
    {
        check_ldid_validity();
        return this->spid_ldid;
    }
    u32 get_spid() const
    {
        check_spid_validity();
        return this->spid_ldid;
    }
    u32 get_dpid() const
    {
        return this->dpid;
    }
    u32 get_hostid() const
    {
        return this->dpid;
    }
    u32 get_deviceid() const
    {
        return this->spid_ldid;
    }

  public:
    void set_dpid(u32 dpid)
    {
        this->dpid = dpid;
        check_dpid_validity();
    }

  private:  // Disabled
    void set_ldid(u32 ldid)
    {
        this->spid_ldid = ldid;
        check_ldid_validity();
    }
    void set_spid(u32 spid)
    {
        this->spid_ldid = spid;
        check_spid_validity();
    }

  private:
    void check_spid_validity() const
    {
        if (this->flitmode != FLITMODE_PBR) {
            CPP_WARNING("68 or 256 mode is not supported for a spid input");
        }
        CPP_ASSERT(this->spid_ldid < (1 << 12));
    }
    void check_dpid_validity() const
    {
        CPP_ASSERT(this->dpid < (1 << 12));
    }
    void check_ldid_validity() const
    {
        if (this->flitmode != FLITMODE_68 && this->flitmode != FLITMODE_256) {
            CPP_WARNING("PBR mode is not supported for a ldid input");
        }
        CPP_ASSERT(this->spid_ldid < (1 << 4));
    }
};

#endif
