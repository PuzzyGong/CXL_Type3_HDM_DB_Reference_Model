#ifndef DEVICE_DCOH_H
#define DEVICE_DCOH_H

#include "../../../conf/global_conf_with_channel.h"
#include "../../shared_var_pool/shared_var_pool.h"
#include "./device_snoop_filter.h"
#include "./device_dcoh_handler.h"

class device_dcoh
{
  private:
    shared_var_pool var_pool;

    // DUT 中 DCOH 的 buffer
    sram<u64, s2m_cpi_bisnp_msg> s2m_cpi_bisnp_sram;

    // DUT 中 Par&Gen 的 buffer
    sram<u32, m2s_cpi_req_msg> m2s_cpi_req_sram;
    sram<u32, m2s_cpi_rwd_msg> m2s_cpi_rwd_sram;
    sram<u32, m2s_uio_rwaddr_msg> m2s_uio_rwaddr_sram;

    // 理论上 DUT 要有此两 buffer，但是 DUT 中用 AXI.user 承载信息
    sram<u32, s2d_axi_rdaddr_msg> s2d_axi_rdaddr_sram;
    sram<u32, s2d_axi_wraddr_msg> s2d_axi_wraddr_sram;

    // DUT 中 DCOH 的 buffer
    limited_list<m2s_cpi_birsp_msg> m2s_cpi_birsp_buffer;
    limited_list<m2s_cpi_req_msg> m2s_cpi_req_buffer;
    limited_list<m2s_cpi_rwd_msg> m2s_cpi_rwd_buffer;
    limited_list<m2s_uio_rwaddr_msg> m2s_uio_rwaddr_buffer;

    device_snoop_filter sf;

  public:
    explicit device_dcoh(const shared_var_pool &var_pool)
        : var_pool(var_pool), s2m_cpi_bisnp_sram("in file device_dcoh, s2m_cpi_bisnp_sram"),
          m2s_cpi_req_sram("in file device_dcoh, m2s_cpi_req_sram"), m2s_cpi_rwd_sram("in file device_dcoh, m2s_cpi_rwd_sram"),
          m2s_uio_rwaddr_sram("in file device_dcoh, m2s_uio_rwaddr_sram"),
          s2d_axi_rdaddr_sram("in file device_dcoh, s2d_axi_rdaddr_sram"),
          s2d_axi_wraddr_sram("in file device_dcoh, s2d_axi_wraddr_sram"),
          m2s_cpi_birsp_buffer("in file device_dcoh, m2s_cpi_birsp_buffer", 50),
          m2s_cpi_req_buffer("in file device_dcoh, m2s_cpi_req_buffer", 50),
          m2s_cpi_rwd_buffer("in file device_dcoh, m2s_cpi_rwd_buffer", 50),
          m2s_uio_rwaddr_buffer("in file device_dcoh, m2s_uio_rwaddr_buffer", 50), sf()
    {
    }

    void process(m2s_cpi_birsp_msg &cpi_birsp,    // io
                 m2s_cpi_req_msg &cpi_req,        // io
                 m2s_cpi_rwd_msg &cpi_rwd,        // io
                 m2s_uio_rwaddr_msg &uio_rwaddr,  // io
                 m2s_dut_msg &dut,                // io

                 limited_list<s2m_cpi_bisnp_msg> &cpi_bisnp_list,    // o
                 limited_list<s2m_cpi_ndr_msg> &cpi_ndr_list,        // o
                 limited_list<s2m_cpi_drs_msg> &cpi_drs_list,        // o
                 limited_list<s2m_uio_rddata_msg> &uio_rddata_list,  // o
                 limited_list<s2m_uio_wrrsp_msg> &uio_wrrsp_list,    // o

                 d2s_axi_rddata_msg &axi_rddata,  // io
                 d2s_axi_wrrsp_msg &axi_wrrsp,    // io

                 limited_list<s2d_axi_rdaddr_msg> &axi_rdaddr_list,  // o
                 limited_list<s2d_axi_wraddr_msg> &axi_wraddr_list   // o
    )
    {
        //
        if (cpi_req.get_valid()) {
            m2s_cpi_req_sram.insert(cpi_req.get_tag(), cpi_req);
        }
        if (cpi_rwd.get_valid()) {
            m2s_cpi_rwd_sram.insert(cpi_rwd.get_tag(), cpi_rwd);
        }
        if (uio_rwaddr.get_valid()) {
            m2s_uio_rwaddr_sram.insert(uio_rwaddr.get_tagidx(), uio_rwaddr);
        }

        //
        std::list<std::tuple<bool, m2s_cpi_birsp_msg, s2m_cpi_bisnp_msg, m2s_cpi_req_msg, m2s_cpi_rwd_msg, m2s_uio_rwaddr_msg,
                             d2s_axi_rddata_msg, d2s_axi_wrrsp_msg>>
            tmp_list;
        all(cpi_birsp, cpi_req, cpi_rwd, uio_rwaddr, dut, axi_rddata, axi_wrrsp, tmp_list);

        //
        std::list<std::unique_ptr<device_dcoh_handler>> handler_list;

        //
        handler_list.push_back(std::make_unique<device_dcoh_handler_MemRd_SnpInv_A>());

        //
        for (const auto &elec : tmp_list) {
            bool flag = false;
            for (const auto &handler : handler_list) {
                
                bool axi_loop = false;
                bool i0 = std::get<0>(elec);
                m2s_cpi_birsp_msg cpi_birsp_TMP = std::get<1>(elec);
                s2m_cpi_bisnp_msg cpi_bisnp_TMP = std::get<2>(elec);
                m2s_cpi_req_msg cpi_req_TMP = std::get<3>(elec);
                m2s_cpi_rwd_msg cpi_rwd_TMP = std::get<4>(elec);
                m2s_uio_rwaddr_msg uio_rwaddr_TMP = std::get<5>(elec);
                d2s_axi_rddata_msg axi_rddata_TMP = std::get<6>(elec);
                d2s_axi_wrrsp_msg axi_wrrsp_TMP = std::get<7>(elec);
                limited_list<s2m_cpi_bisnp_msg> cpi_bisnp_list_TMP;
                limited_list<s2m_cpi_ndr_msg> cpi_ndr_list_TMP;
                limited_list<s2m_cpi_drs_msg> cpi_drs_list_TMP;
                limited_list<s2m_uio_rddata_msg> uio_rddata_list_TMP;
                limited_list<s2m_uio_wrrsp_msg> uio_wrrsp_list_TMP;
                limited_list<s2d_axi_rdaddr_msg> axi_rdaddr_list_TMP;
                limited_list<s2d_axi_wraddr_msg> axi_wraddr_list_TMP;

                if (handler->handle(axi_loop, i0, sf, cpi_birsp_TMP, cpi_bisnp_TMP, cpi_req_TMP, cpi_rwd_TMP, uio_rwaddr_TMP, axi_rddata_TMP,
                                    axi_wrrsp_TMP, cpi_bisnp_list_TMP, cpi_ndr_list_TMP, cpi_drs_list_TMP, uio_rddata_list_TMP,
                                    uio_wrrsp_list_TMP, axi_rdaddr_list_TMP, axi_wraddr_list_TMP)) {
                    flag = true;
                    cpi_bisnp_list.push_back(cpi_bisnp_list_TMP);
                    cpi_ndr_list.push_back(cpi_ndr_list_TMP);
                    cpi_drs_list.push_back(cpi_drs_list_TMP);
                    uio_rddata_list.push_back(uio_rddata_list_TMP);
                    uio_wrrsp_list.push_back(uio_wrrsp_list_TMP);
                    axi_rdaddr_list.push_back(axi_rdaddr_list_TMP);
                    axi_wraddr_list.push_back(axi_wraddr_list_TMP);
                    m2s_cpi_birsp_buffer.push_back(cpi_birsp_TMP);
                    m2s_cpi_req_buffer.push_back(cpi_req_TMP);
                    m2s_cpi_rwd_buffer.push_back(cpi_rwd_TMP);
                    m2s_uio_rwaddr_buffer.push_back(uio_rwaddr_TMP);

                    break;
                }
            }
            if(!flag) {
                CPP_ERROR("device_dcoh: no handler found");
            }
        }

        //
        for (const auto &cpi_bisnp : cpi_bisnp_list) {
            if (cpi_bisnp.get_valid()) {
                s2m_cpi_bisnp_sram.insert(cpi_bisnp.get_bitag() << 32 | cpi_bisnp.get_hostid(), cpi_bisnp);
            }
        }
        for (const auto &s2d_axi_rdaddr : axi_rdaddr_list) {
            if (s2d_axi_rdaddr.get_valid()) {
                s2d_axi_rdaddr_sram.insert(s2d_axi_rdaddr.get_id(), s2d_axi_rdaddr);
            }
        }
        for (const auto &s2d_axi_wraddr : axi_wraddr_list) {
            if (s2d_axi_wraddr.get_valid()) {
                s2d_axi_wraddr_sram.insert(s2d_axi_wraddr.get_id(), s2d_axi_wraddr);
            }
        }
    }

  private:
    void all(const m2s_cpi_birsp_msg &cpi_birsp,    // i
             const m2s_cpi_req_msg &cpi_req,        // i
             const m2s_cpi_rwd_msg &cpi_rwd,        // i
             const m2s_uio_rwaddr_msg &uio_rwaddr,  // i
             const m2s_dut_msg &dut,                // i
             const d2s_axi_rddata_msg &axi_rddata,  // i
             const d2s_axi_wrrsp_msg &axi_wrrsp,    // i
             std::list<std::tuple<bool,
                                  m2s_cpi_birsp_msg,
                                  s2m_cpi_bisnp_msg,
                                  m2s_cpi_req_msg,
                                  m2s_cpi_rwd_msg,
                                  m2s_uio_rwaddr_msg,
                                  d2s_axi_rddata_msg,
                                  d2s_axi_wrrsp_msg>> &lst  // o
    )
    {
        lst.clear();

        m2s_cpi_req_msg cpi_req_TMP;
        m2s_cpi_rwd_msg cpi_rwd_TMP;
        m2s_uio_rwaddr_msg uio_rwaddr_TMP;

        // cpi_req_TMP, cpi_rwd_TMP, uio_rwaddr_TMP <= axi_rddata
        // lst[] <= cpi_req_TMP, cpi_rwd_TMP, uio_rwaddr_TMP, axi_rddata
        if (axi_rddata.get_valid()) {
            get(axi_rddata, cpi_req_TMP, cpi_rwd_TMP, uio_rwaddr_TMP);
            lst.push_back(std::make_tuple(1, m2s_cpi_birsp_msg(), s2m_cpi_bisnp_msg(), cpi_req_TMP, cpi_rwd_TMP, uio_rwaddr_TMP,
                                          axi_rddata, d2s_axi_wrrsp_msg()));
        }

        // cpi_req_TMP, cpi_rwd_TMP, uio_rwaddr_TMP <= axi_wrrsp
        // lst[] <= cpi_req_TMP, cpi_rwd_TMP, uio_rwaddr_TMP, axi_wrrsp
        if (axi_wrrsp.get_valid()) {
            get(axi_wrrsp, cpi_req_TMP, cpi_rwd_TMP, uio_rwaddr_TMP);
            lst.push_back(std::make_tuple(1, m2s_cpi_birsp_msg(), s2m_cpi_bisnp_msg(), cpi_req_TMP, cpi_rwd_TMP, uio_rwaddr_TMP,
                                          d2s_axi_rddata_msg(), axi_wrrsp));
        }

        // cpi_bisnp_TMP <= cpi_birsp
        // lst[] <= cpi_birsp, cpi_req, cpi_rwd, uio_rwaddr
        s2m_cpi_bisnp_msg cpi_bisnp_TMP;
        if (cpi_birsp.get_valid() || cpi_birsp.get_valid() || cpi_birsp.get_valid() || cpi_birsp.get_valid()) {
            s2m_cpi_bisnp_sram.find(cpi_birsp.get_bitag() << 32 | cpi_birsp.get_hostid(), cpi_bisnp_TMP);
            lst.push_back(
                std::make_tuple(0, cpi_birsp, cpi_bisnp_TMP, cpi_req, cpi_rwd, uio_rwaddr, d2s_axi_rddata_msg(), d2s_axi_wrrsp_msg()));
        }
    }

    void dut(const m2s_cpi_birsp_msg &cpi_birsp,                                                                     // i
             const m2s_cpi_req_msg &cpi_req,                                                                         // i
             const m2s_cpi_rwd_msg &cpi_rwd,                                                                         // i
             const m2s_uio_rwaddr_msg &uio_rwaddr,                                                                   // i
             const m2s_dut_msg &dut,                                                                                 // i
             std::list<std::tuple<m2s_cpi_birsp_msg, m2s_cpi_req_msg, m2s_cpi_rwd_msg, m2s_uio_rwaddr_msg>> &lst  // o
    )
    {
        lst.clear();

        m2s_cpi_birsp_buffer.push_back(cpi_birsp);
        m2s_cpi_req_buffer.push_back(cpi_req);
        m2s_cpi_rwd_buffer.push_back(cpi_rwd);
        m2s_uio_rwaddr_buffer.push_back(uio_rwaddr);

        lst.push_back(std::make_tuple(cpi_birsp, cpi_req, cpi_rwd, uio_rwaddr));

        m2s_cpi_birsp_buffer.clear();
        m2s_cpi_req_buffer.clear();
        m2s_cpi_rwd_buffer.clear();
        m2s_uio_rwaddr_buffer.clear();
    }

    void get(const d2s_axi_rddata_msg &axi_rddata,  // i
             m2s_cpi_req_msg &cpi_req,              // o
             m2s_cpi_rwd_msg &cpi_rwd,              // o
             m2s_uio_rwaddr_msg &uio_rwaddr         // o
    ) const
    {
        // axi_rdaddr_TMP <= axi_rddata
        s2d_axi_rdaddr_msg axi_rdaddr_TMP;
        s2d_axi_rdaddr_sram.find(axi_rddata.get_id(), axi_rdaddr_TMP);

        // cpi_req, cpi_rwd, uio_rwaddr <= axi_rdaddr_TMP
        if (axi_rdaddr_TMP.tag >> 31 == 0) {
            m2s_cpi_req_sram.find(axi_rdaddr_TMP.tag, cpi_req);
            m2s_cpi_rwd_sram.find(axi_rdaddr_TMP.tag, cpi_rwd);
        } else {
            m2s_uio_rwaddr_sram.find(axi_rdaddr_TMP.tag, uio_rwaddr);
        }
    }

    void get(const d2s_axi_wrrsp_msg &axi_wrrsp,  // i
             m2s_cpi_req_msg &cpi_req,            // o
             m2s_cpi_rwd_msg &cpi_rwd,            // o
             m2s_uio_rwaddr_msg &uio_rwaddr       // o
    ) const
    {
        // axi_wraddr_TMP <= axi_wrrsp
        s2d_axi_wraddr_msg axi_wraddr_TMP;
        s2d_axi_wraddr_sram.find(axi_wrrsp.get_id(), axi_wraddr_TMP);

        // cpi_req, cpi_rwd, uio_rwaddr <= axi_wraddr_TMP
        if (axi_wraddr_TMP.tag >> 31 == 0) {
            m2s_cpi_req_sram.find(axi_wraddr_TMP.tag, cpi_req);
            m2s_cpi_rwd_sram.find(axi_wraddr_TMP.tag, cpi_rwd);
        } else {
            m2s_uio_rwaddr_sram.find(axi_wraddr_TMP.tag, uio_rwaddr);
        }
    }
};

#endif
