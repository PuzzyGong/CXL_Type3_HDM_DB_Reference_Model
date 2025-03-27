#include "s_top.h"

void S_Top_Type::product(
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
    for (auto s2m_snp_single : s2m_snp)
    {
        if (s2m_snp_single.valid)
        {
            mapper[tag_now] = s2m_snp_single;
            tag_now = (tag_now + 1) % (1 << 12);
        }
    }

    M2S_Rsp_type new_m2s_rsp;
    if (m2s_rsp.valid)
    {
        if (mapper.find(m2s_rsp.bitag) != mapper.end())
        {
            new_m2s_rsp.snp = mapper[m2s_rsp.bitag];
            mapper.erase(m2s_rsp.bitag);
        }
        else
        {
            CPP_ERROR("BITag_HostID_Mapper: No Such BITag");
        }
    }

    vsf.product(
        m2s_req,
        new_m2s_rsp,
        m2s_rwd,
        s2m_snp,
        s2m_ndr,
        s2m_drs,
        rd_data,
        rd_addr,
        wr,
        cxl_io_req,
        cxl_io_rsp,
        victim
    );
}