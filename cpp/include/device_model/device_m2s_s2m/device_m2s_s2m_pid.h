#ifndef DEVICE_M2S_S2M_PID_H
#define DEVICE_M2S_S2M_PID_H

#include "device_m2s_s2m_base.h"

class device_m2s_s2m_pid
{
  private:
    shared_var_pool var_pool;

  public:
    explicit device_m2s_s2m_pid(const shared_var_pool &var_pool) : var_pool(var_pool)
    {
    }

    void m2s_process(m2s_cpi_birsp_msg &cpi_birsp_msg,    // io
                     m2s_cpi_req_msg &cpi_req_msg,        // io
                     m2s_cpi_rwd_msg &cpi_rwd_msg,        // io
                     m2s_uio_rwaddr_msg &uio_rwaddr_msg,  // io

                     limited_list<s2m_cpi_bisnp_msg> &bisnp_msg,    // o
                     limited_list<s2m_cpi_ndr_msg> &ndr_msg,        // o
                     limited_list<s2m_cpi_drs_msg> &drs_msg,        // o
                     limited_list<s2m_uio_rddata_msg> &rddata_msg,  // o
                     limited_list<s2m_uio_wrrsp_msg> &wrrsp_msg     // o
    )
    {
    }

    void s2m_process(limited_list<s2m_cpi_bisnp_msg> &bisnp_msg,    // io
                     limited_list<s2m_cpi_ndr_msg> &ndr_msg,        // io
                     limited_list<s2m_cpi_drs_msg> &drs_msg,        // io
                     limited_list<s2m_uio_rddata_msg> &rddata_msg,  // io
                     limited_list<s2m_uio_wrrsp_msg> &wrrsp_msg     // io
    )
    {
    }
};

#endif
