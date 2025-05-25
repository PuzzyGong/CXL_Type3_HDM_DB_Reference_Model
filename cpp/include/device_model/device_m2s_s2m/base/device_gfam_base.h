#ifndef DEVICE_GFAM_BASE_H
#define DEVICE_GFAM_BASE_H

#include "../../../conf/global_conf_with_channel.h"
#include "../../shared_var_pool/shared_var_pool.h"

class device_gfam_base
{
  protected:
    shared_var_pool shared_pool;

  public:
    explicit device_gfam_base(const shared_var_pool &shared_pool) : shared_pool(shared_pool)
    {
    }
    ~device_gfam_base() = default;
    virtual bool m2s_process(unsigned long long hpa, unsigned long long &dpa)
    {
        dpa = hpa - 1;
        if (hpa == 0 || hpa == 1)
            return false;
        else
            return true;
    }
    virtual bool s2m_process(unsigned long long dpa, unsigned int dpid_6bit, unsigned long long &hpa)
    {
        hpa = dpa + 1;
        return true;
    }
};

#endif