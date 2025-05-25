#ifndef DEVICE_LDFAM_BASE_H
#define DEVICE_LDFAM_BASE_H

#include "../../../conf/global_conf_with_channel.h"
#include "../../shared_var_pool/shared_var_pool.h"

class device_ldfam_base
{
  protected:
    shared_var_pool shared_pool;

  public:
    explicit device_ldfam_base(const shared_var_pool &shared_pool) : shared_pool(shared_pool)
    {
    }
    ~device_ldfam_base() = default;
    virtual bool m2s_process(unsigned long long hpa, unsigned int ld_id, unsigned long long &dpa)
    {
        dpa = hpa - 1;
        if (hpa == 0 || hpa == 1)
            return false;
        else
            return true;
    }
    virtual bool s2m_process(unsigned long long dpa, unsigned int ld_id, unsigned long long &hpa)
    {
        hpa = dpa + 1;
        return true;
    }
};

#endif