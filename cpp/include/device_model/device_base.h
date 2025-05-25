#ifndef DEVICE_BASE_H
#define DEVICE_BASE_H

#include "device_m2s_s2m/device_m2s_s2m_pid.h"
#include "device_m2s_s2m/device_m2s_s2m_base.h"
#include "device_d2s_s2d/device_d2s_s2d_interleaving.h"
#include "dcoh/device_dcoh.h"

class device_base
{
  private:
    shared_var_pool var_pool;
    device_m2s_s2m_pid pid_module;
    device_m2s_s2m_base decoder_module;
    device_m2s_s2m_base dcd_module;
    device_dcoh dcoh_module;
    device_d2s_s2d_interleaving interleaving_module;

  public:
    device_base(const shared_var_pool &var_pool)
        : var_pool(var_pool),                 //
          pid_module(var_pool),               //
          decoder_module(var_pool, 0, 1, 1),  //
          dcd_module(var_pool, 1, 1, 1),      //
          dcoh_module(var_pool),              //
          interleaving_module(var_pool)       //
    {};
    ~device_base();

    virtual void process(m2s_cpi_birsp_msg &cpi_birsp_msg,    // io
                         m2s_cpi_req_msg &cpi_req_msg,        // io
                         m2s_cpi_rwd_msg &cpi_rwd_msg,        // io
                         m2s_uio_rwaddr_msg &uio_rwaddr_msg,  // io
                         m2s_dut_msg &dut_msg,                // io

                         limited_list<s2m_cpi_bisnp_msg> &cpi_bisnp_msg_list,    // o
                         limited_list<s2m_cpi_ndr_msg> &cpi_ndr_msg_list,        // o
                         limited_list<s2m_cpi_drs_msg> &cpi_drs_msg_list,        // o
                         limited_list<s2m_uio_rddata_msg> &uio_rddata_msg_list,  // o
                         limited_list<s2m_uio_wrrsp_msg> &uio_wrrsp_msg_list,    // o

                         d2s_axi_rddata_msg &axi_rddata_msg,  // io
                         d2s_axi_wrrsp_msg &axi_wrrsp_msg,    // io

                         limited_list<s2d_axi_rdaddr_msg> &axi_rdaddr_msg_list,  // o
                         limited_list<s2d_axi_wraddr_msg> &axi_wraddr_msg_list   // o
    )
    {
        //
        interleaving_module.d2s_process(axi_rddata_msg, axi_wrrsp_msg);

        //
        limited_list<s2m_cpi_bisnp_msg> cpi_bisnp_msg_list__BY__pid_module;
        limited_list<s2m_cpi_ndr_msg> cpi_ndr_msg_list__BY__pid_module;
        limited_list<s2m_cpi_drs_msg> cpi_drs_msg_list__BY__pid_module;
        limited_list<s2m_uio_rddata_msg> uio_rddata_msg_list__BY__pid_module;
        limited_list<s2m_uio_wrrsp_msg> uio_wrrsp_msg_list__BY__pid_module;
        pid_module.m2s_process(cpi_birsp_msg,                        //
                               cpi_req_msg,                          //
                               cpi_rwd_msg,                          //
                               uio_rwaddr_msg,                       //
                               cpi_bisnp_msg_list__BY__pid_module,   //
                               cpi_ndr_msg_list__BY__pid_module,     //
                               cpi_drs_msg_list__BY__pid_module,     //
                               uio_rddata_msg_list__BY__pid_module,  //
                               uio_wrrsp_msg_list__BY__pid_module    //
        );

        limited_list<s2m_cpi_bisnp_msg> cpi_bisnp_msg_list__BY__decoder_module;
        limited_list<s2m_cpi_ndr_msg> cpi_ndr_msg_list__BY__decoder_module;
        limited_list<s2m_cpi_drs_msg> cpi_drs_msg_list__BY__decoder_module;
        limited_list<s2m_uio_rddata_msg> uio_rddata_msg_list__BY__decoder_module;
        limited_list<s2m_uio_wrrsp_msg> uio_wrrsp_msg_list__BY__decoder_module;
        decoder_module.m2s_process(cpi_birsp_msg,                            //
                                   cpi_req_msg,                              //
                                   cpi_rwd_msg,                              //
                                   uio_rwaddr_msg,                           //
                                   cpi_bisnp_msg_list__BY__decoder_module,   //
                                   cpi_ndr_msg_list__BY__decoder_module,     //
                                   cpi_drs_msg_list__BY__decoder_module,     //
                                   uio_rddata_msg_list__BY__decoder_module,  //
                                   uio_wrrsp_msg_list__BY__decoder_module    //
        );

        limited_list<s2m_cpi_bisnp_msg> cpi_bisnp_msg_list__BY__dcd_module;
        limited_list<s2m_cpi_ndr_msg> cpi_ndr_msg_list__BY__dcd_module;
        limited_list<s2m_cpi_drs_msg> cpi_drs_msg_list__BY__dcd_module;
        limited_list<s2m_uio_rddata_msg> uio_rddata_msg_list__BY__dcd_module;
        limited_list<s2m_uio_wrrsp_msg> uio_wrrsp_msg_list__BY__dcd_module;
        dcd_module.m2s_process(cpi_birsp_msg,                        //
                               cpi_req_msg,                          //
                               cpi_rwd_msg,                          //
                               uio_rwaddr_msg,                       //
                               cpi_bisnp_msg_list__BY__dcd_module,   //
                               cpi_ndr_msg_list__BY__dcd_module,     //
                               cpi_drs_msg_list__BY__dcd_module,     //
                               uio_rddata_msg_list__BY__dcd_module,  //
                               uio_wrrsp_msg_list__BY__dcd_module    //
        );

        limited_list<s2m_cpi_bisnp_msg> cpi_bisnp_msg_list_RETURN;
        limited_list<s2m_cpi_ndr_msg> cpi_ndr_msg_list_RETURN;
        limited_list<s2m_cpi_drs_msg> cpi_drs_msg_list_RETURN;
        limited_list<s2m_uio_rddata_msg> uio_rddata_msg_list_RETURN;
        limited_list<s2m_uio_wrrsp_msg> uio_wrrsp_msg_list_RETURN;
        limited_list<s2d_axi_rdaddr_msg> axi_rdaddr_msg_list_RETURN;
        limited_list<s2d_axi_wraddr_msg> axi_wraddr_msg_list_RETURN;
        dcoh_module.process(cpi_birsp_msg,               //
                            cpi_req_msg,                 //
                            cpi_rwd_msg,                 //
                            uio_rwaddr_msg,              //
                            dut_msg,                     //
                            cpi_bisnp_msg_list_RETURN,   //
                            cpi_ndr_msg_list_RETURN,     //
                            cpi_drs_msg_list_RETURN,     //
                            uio_rddata_msg_list_RETURN,  //
                            uio_wrrsp_msg_list_RETURN,   //
                            axi_rddata_msg,              //
                            axi_wrrsp_msg,               //
                            axi_rdaddr_msg_list_RETURN,  //
                            axi_wraddr_msg_list_RETURN   //
        );

        //
        interleaving_module.s2d_process(axi_rdaddr_msg_list_RETURN,  //
                                        axi_wraddr_msg_list_RETURN   //

        );

        //
        dcd_module.s2m_process(cpi_bisnp_msg_list_RETURN,   //
                               cpi_ndr_msg_list_RETURN,     //
                               cpi_drs_msg_list_RETURN,     //
                               uio_rddata_msg_list_RETURN,  //
                               uio_wrrsp_msg_list_RETURN    //
        );
        cpi_bisnp_msg_list_RETURN.push_back(cpi_bisnp_msg_list__BY__dcd_module);
        cpi_ndr_msg_list_RETURN.push_back(cpi_ndr_msg_list__BY__dcd_module);
        cpi_drs_msg_list_RETURN.push_back(cpi_drs_msg_list__BY__dcd_module);
        uio_rddata_msg_list_RETURN.push_back(uio_rddata_msg_list__BY__dcd_module);
        uio_wrrsp_msg_list_RETURN.push_back(uio_wrrsp_msg_list__BY__dcd_module);

        //
        decoder_module.s2m_process(cpi_bisnp_msg_list_RETURN,   //
                                   cpi_ndr_msg_list_RETURN,     //
                                   cpi_drs_msg_list_RETURN,     //
                                   uio_rddata_msg_list_RETURN,  //
                                   uio_wrrsp_msg_list_RETURN    //
        );
        cpi_bisnp_msg_list_RETURN.push_back(cpi_bisnp_msg_list__BY__decoder_module);
        cpi_ndr_msg_list_RETURN.push_back(cpi_ndr_msg_list__BY__decoder_module);
        cpi_drs_msg_list_RETURN.push_back(cpi_drs_msg_list__BY__decoder_module);
        uio_rddata_msg_list_RETURN.push_back(uio_rddata_msg_list__BY__decoder_module);
        uio_wrrsp_msg_list_RETURN.push_back(uio_wrrsp_msg_list__BY__decoder_module);

        //
        pid_module.s2m_process(cpi_bisnp_msg_list_RETURN,   //
                               cpi_ndr_msg_list_RETURN,     //
                               cpi_drs_msg_list_RETURN,     //
                               uio_rddata_msg_list_RETURN,  //
                               uio_wrrsp_msg_list_RETURN    //
        );
        cpi_bisnp_msg_list_RETURN.push_back(cpi_bisnp_msg_list__BY__pid_module);
        cpi_ndr_msg_list_RETURN.push_back(cpi_ndr_msg_list__BY__pid_module);
        cpi_drs_msg_list_RETURN.push_back(cpi_drs_msg_list__BY__pid_module);
        uio_rddata_msg_list_RETURN.push_back(uio_rddata_msg_list__BY__pid_module);
        uio_wrrsp_msg_list_RETURN.push_back(uio_wrrsp_msg_list__BY__pid_module);

        //
        cpi_bisnp_msg_list.clear();
        cpi_ndr_msg_list.clear();
        cpi_drs_msg_list.clear();
        uio_rddata_msg_list.clear();
        uio_wrrsp_msg_list.clear();

        cpi_bisnp_msg_list.push_back(cpi_bisnp_msg_list__BY__pid_module);
        cpi_ndr_msg_list.push_back(cpi_ndr_msg_list__BY__pid_module);
        cpi_drs_msg_list.push_back(cpi_drs_msg_list__BY__pid_module);
        uio_rddata_msg_list.push_back(uio_rddata_msg_list__BY__pid_module);
        uio_wrrsp_msg_list.push_back(uio_wrrsp_msg_list__BY__pid_module);
    }
};

#endif