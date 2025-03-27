#ifndef S_DECODER_H
#define S_DECODER_H

#include "define.h"
#include "port.h"

class S_Decoder
{
public:
    S_Decoder();
    inline void product(const CXL_io_Req_Type &cxl_io_req, std::list<CXL_io_Rsp_Type> &cxl_io_rsp) { }
    inline void get_host_pid(std::list<u32> &host_id_list) const { host_id_list.assign({1, 3, 5, 7, 11, 13, 17, 19, 23, 29}); }
    inline u32 get_device_pid(void) const { return 999; }
    inline void decode(const M2S_Req_Type &i_m2s_req, M2S_Req_Type &o_m2s_req) { o_m2s_req = i_m2s_req; }
    inline void decode(const M2S_Rsp_type &i_m2s_rsp, M2S_Rsp_type &o_m2s_rsp) { o_m2s_rsp = i_m2s_rsp; }
    inline void decode(const M2S_RwD_Type &i_m2s_rwd, M2S_RwD_Type &o_m2s_rwd) { o_m2s_rwd = i_m2s_rwd; }
    inline void decode(const S2M_Snp_type &i_s2m_snp, S2M_Snp_type &o_s2m_snp) { o_s2m_snp = i_s2m_snp; }
    inline void decode(const S2M_NDR_Type &i_s2m_ndr, S2M_NDR_Type &o_s2m_ndr) { o_s2m_ndr = i_s2m_ndr; }
    inline void decode(const S2M_DRS_Type &i_s2m_drs, S2M_DRS_Type &o_s2m_drs) { o_s2m_drs = i_s2m_drs; }
};

#endif