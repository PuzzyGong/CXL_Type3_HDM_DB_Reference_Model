#ifndef S_VSF_H
#define S_VSF_H

#include "define.h"
#include "s_decoder.h"
#include "s_vsf_line.h"

class S_VSF_Type
{
private:
    std::list<M2S_Req_Type> m2s_req_list;
    std::unordered_map<u64, S_VSF_Line_Type> vsf_line;
    S_Decoder decoder;
    void main_product(
        const M2S_Rsp_type &m2s_rsp,
        const M2S_RwD_Type &m2s_rwd,
        std::list<S2M_Snp_type> &s2m_snp,
        std::list<S2M_NDR_Type> &s2m_ndr,
        std::list<S2M_DRS_Type> &s2m_drs,
        const Rd_Data_Type &rd_data,
        std::list<Rd_Addr_Type> &rd_addr,
        std::list<Wr_Type> &wr);

public:

    inline void product(
        const M2S_Req_Type &m2s_req,
        const M2S_Rsp_type &m2s_rsp,
        const M2S_RwD_Type &m2s_rwd,
        std::list<S2M_Snp_type> &s2m_snp,
        std::list<S2M_NDR_Type> &s2m_ndr,
        std::list<S2M_DRS_Type> &s2m_drs,

        const Rd_Data_Type &rd_data,
        std::list<Rd_Addr_Type> &rd_addr,
        std::list<Wr_Type> &wr,

        const CXL_io_Req_Type &cxl_io_req,
        std::list<CXL_io_Rsp_Type> &cxl_io_rsp,
        const Victim_Type &victim)
    {
        // pre_product(m2s_req, m2s_rsp, m2s_rwd, s2m_snp, s2m_ndr, s2m_drs, rd_data, rd_addr, wr, cxl_io_req, cxl_io_rsp, victim);
        main_product(m2s_req, m2s_rsp, m2s_rwd, s2m_snp, s2m_ndr, s2m_drs, rd_data, rd_addr, wr, cxl_io_req, cxl_io_rsp, victim);
        // post_product(m2s_req, m2s_rsp, m2s_rwd, s2m_snp, s2m_ndr, s2m_drs, rd_data, rd_addr, wr, cxl_io_req, cxl_io_rsp, victim);
    };
    void main_product(
        const M2S_Req_Type &m2s_req,
        const M2S_Rsp_type &m2s_rsp,
        const M2S_RwD_Type &m2s_rwd,
        std::list<S2M_Snp_type> &s2m_snp,
        std::list<S2M_NDR_Type> &s2m_ndr,
        std::list<S2M_DRS_Type> &s2m_drs,

        const Rd_Data_Type &rd_data,
        std::list<Rd_Addr_Type> &rd_addr,
        std::list<Wr_Type> &wr,

        const CXL_io_Req_Type &cxl_io_req,
        std::list<CXL_io_Rsp_Type> &cxl_io_rsp,
        const Victim_Type &victim);
};

#endif