#ifndef S_VSF_H
#define S_VSF_H

#include "define.h"
#include "s_decoder.h"
#include "s_vsf_line.h"

#define mapper_size 32
#define s2m_snp_not_alloc_bitag_size 32
#define m2s_req_list_size 16
#define vsf_line_size 1024
class S_VSF_Type
{
private:
    std::unordered_map<u32, S2M_Snp_type> mapper;
    u32 mapper_bitag_now;

    std::list<S2M_Snp_type> s2m_snp_not_alloc_bitag;
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
    S_VSF_Type() : mapper_bitag_now(0) {}
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
        const Victim_Type &victim,
        const S2M_Snp_type &s2m_snp_provide_bitag)
    {
        // pre_product(m2s_req, m2s_rsp, m2s_rwd, s2m_snp, s2m_ndr, s2m_drs, rd_data, rd_addr, wr, cxl_io_req, cxl_io_rsp, victim, s2m_snp_provide_bitag);
        main_product(m2s_req, m2s_rsp, m2s_rwd, s2m_snp, s2m_ndr, s2m_drs, rd_data, rd_addr, wr, cxl_io_req, cxl_io_rsp, victim, s2m_snp_provide_bitag);
        // post_product(m2s_req, m2s_rsp, m2s_rwd, s2m_snp, s2m_ndr, s2m_drs, rd_data, rd_addr, wr, cxl_io_req, cxl_io_rsp, victim, s2m_snp_provide_bitag);
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
        const Victim_Type &victim,
        const S2M_Snp_type &s2m_snp_provide_bitag);
};

#endif