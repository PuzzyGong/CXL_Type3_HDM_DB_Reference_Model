#ifndef S_TOP_H
#define S_TOP_H

#include "define.h"
#include "s_vsf.h"

class S_Top_Type
{
private:
    std::unordered_map<unsigned int, S2M_Snp_type> mapper;
    u32 tag_now;
    S_VSF_Type vsf;

public:
    void product(
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